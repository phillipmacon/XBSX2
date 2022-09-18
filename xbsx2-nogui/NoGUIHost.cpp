/*  XBSX2 - PS2 Emulator for Xbox Consoles
 *  Copyright (C) 2002-2022  PCSX2 Dev Team
 * 
 *  XBSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  XBSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with XBSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"

#include <chrono>
#include <cmath>
#include <csignal>
#include <condition_variable>
#include <mutex>
#include <thread>

#ifdef _WIN32
#include "common/RedtapeWindows.h"
#include <KnownFolders.h>
#include <ShlObj.h>
#endif

#include "fmt/core.h"

#include "common/Assertions.h"
#include "common/Console.h"
#include "common/CrashHandler.h"
#include "common/Exceptions.h"
#include "common/FileSystem.h"
#include "common/Path.h"
#include "common/SettingsWrapper.h"
#include "common/StringUtil.h"

#include "xbsx2/CDVD/CDVD.h"
#include "xbsx2/Frontend/CommonHost.h"
#include "xbsx2/Frontend/FullscreenUI.h"
#include "xbsx2/Frontend/GameList.h"
#include "xbsx2/Frontend/InputManager.h"
#include "xbsx2/Frontend/ImGuiFullscreen.h"
#include "xbsx2/Frontend/ImGuiManager.h"
#include "xbsx2/Frontend/INISettingsInterface.h"
#include "xbsx2/Frontend/LogSink.h"
#include "xbsx2/GS.h"
#include "xbsx2/GS/GS.h"
#include "xbsx2/GSDumpReplayer.h"
#include "xbsx2/HostDisplay.h"
#include "xbsx2/HostSettings.h"
#include "xbsx2/PAD/Host/PAD.h"
#include "xbsx2/PerformanceMetrics.h"
#include "xbsx2/VMManager.h"

#include "NoGUIHost.h"
#include "NoGUIPlatform.h"
#include "svnrev.h"

#include "xbsx2/DebugTools/Debug.h"
#include <include/IconsFontAwesome5.h>

using namespace std::chrono_literals;

static constexpr u32 SETTINGS_VERSION = 1;
static constexpr auto CPU_THREAD_POLL_INTERVAL = 8ms; // how often we'll poll controllers when paused

std::unique_ptr<NoGUIPlatform> g_nogui_window;

//////////////////////////////////////////////////////////////////////////
// Local function declarations
//////////////////////////////////////////////////////////////////////////
namespace NoGUIHost
{
#ifndef _UWP
	static void PrintCommandLineVersion();
	static void PrintCommandLineHelp(const char* progname);
	static std::shared_ptr<VMBootParameters>& AutoBoot(std::shared_ptr<VMBootParameters>& autoboot);
	static bool ParseCommandLineOptions(int argc, char* argv[], std::shared_ptr<VMBootParameters>& autoboot);
#endif
	static bool InitializeConfig();
	static void HookSignals();
	static void StartCPUThread();
	static void StopCPUThread();
	static void ProcessCPUThreadEvents(bool block);
	static void ProcessCPUThreadPlatformMessages();
	static void CPUThreadEntryPoint();
	static void CPUThreadMainLoop();
	static std::unique_ptr<NoGUIPlatform> CreatePlatform();
	static std::string GetWindowTitle(const std::string& game_title);
	static void UpdateWindowTitle(const std::string& game_title);
	static void CancelAsyncOp();
	static void StartAsyncOp(std::function<void(ProgressCallback*)> callback);
	static void AsyncOpThreadEntryPoint(std::function<void(ProgressCallback*)> callback);
} // namespace NoGUIHost

//////////////////////////////////////////////////////////////////////////
// Local variable declarations
//////////////////////////////////////////////////////////////////////////
static std::unique_ptr<INISettingsInterface> s_base_settings_interface;
static Threading::KernelSemaphore s_platform_window_updated;
alignas(16) static SysMtgsThread s_mtgs_thread;
static std::atomic_bool s_running{false};
static bool s_batch_mode = false;
static bool s_is_fullscreen = false;
static bool s_save_state_on_shutdown = false;
static bool s_was_paused_by_focus_loss = false;

static Threading::Thread s_cpu_thread;
static std::mutex s_cpu_thread_events_mutex;
static std::condition_variable s_cpu_thread_event_done;
static std::condition_variable s_cpu_thread_event_posted;
static std::deque<std::pair<std::function<void()>, bool>> s_cpu_thread_events;
static u32 s_blocking_cpu_events_pending = 0; // TODO: Token system would work better here.

static std::mutex s_async_op_mutex;
static std::thread s_async_op_thread;
static FullscreenUI::ProgressCallback* s_async_op_progress = nullptr;

//////////////////////////////////////////////////////////////////////////
// Initialization/Shutdown
//////////////////////////////////////////////////////////////////////////

bool NoGUIHost::Initialize()
{
	if (!InitializeConfig())
	{
		g_nogui_window->ReportError("Error", "Failed to initialize config.");
		return false;
	}

	// the rest of initialization happens on the CPU thread.
	HookSignals();
	StartCPUThread();
	return true;
}

void NoGUIHost::Shutdown()
{
	StopCPUThread();
}

bool NoGUIHost::InitializeConfig()
{
	if (!CommonHost::InitializeCriticalFolders())
	{
		g_nogui_window->ReportError("XBSX2", "One or more critical directories are missing, your installation may be incomplete.");
		return false;
	}

	const std::string path(Path::Combine(EmuFolders::Settings, "XBSX2.ini"));
	Console.WriteLn("Loading config from %s.", path.c_str());

	s_base_settings_interface = std::make_unique<INISettingsInterface>(std::move(path));
	Host::Internal::SetBaseSettingsLayer(s_base_settings_interface.get());
	if (!s_base_settings_interface->Load() || !CommonHost::CheckSettingsVersion())
	{
		CommonHost::SetDefaultSettings(*s_base_settings_interface, true, true, true, true, true);
		s_base_settings_interface->Save();
	}

	CommonHost::LoadStartupSettings();
	Host::UpdateLogging(false);
	return true;
}

#ifndef _UWP
void Host::SetDefaultUISettings(SettingsInterface& si)
{
	g_nogui_window->SetDefaultConfig(si);
}
#endif

bool Host::RequestResetSettings(bool folders, bool core, bool controllers, bool hotkeys, bool ui)
{
	if (folders)
		return false;

	RunOnCPUThread([folders, core, controllers, hotkeys, ui]() {
		{
			auto lock = Host::GetSettingsLock();
			CommonHost::SetDefaultSettings(*s_base_settings_interface.get(), folders, core, controllers, hotkeys, ui);
		}
		Host::CommitBaseSettingChanges();
		VMManager::ApplySettings();
	});

	return true;
}

void Host::CommitBaseSettingChanges()
{
	auto lock = Host::GetSettingsLock();
	if (s_base_settings_interface->IsDirty() && !s_base_settings_interface->Save())
		Console.Error("Failed to save settings.");
}

bool NoGUIHost::InBatchMode()
{
	return s_batch_mode;
}

void NoGUIHost::SetBatchMode(bool enabled)
{
	s_batch_mode = enabled;
	if (enabled)
		GameList::Refresh(false, true);
}

void NoGUIHost::StartVM(std::shared_ptr<VMBootParameters> params)
{
	Host::RunOnCPUThread([params = std::move(params)]() {
		if (!VMManager::Initialize(*params))
			return;

		VMManager::SetState(VMState::Running);
	});
}

void NoGUIHost::ProcessPlatformWindowResize(s32 width, s32 height, float scale)
{
	Host::RunOnCPUThread([width, height, scale]() { GetMTGS().ResizeDisplayWindow(width, height, scale); });
}

void NoGUIHost::ProcessPlatformMouseMoveEvent(float x, float y)
{
	InputManager::UpdatePointerAbsolutePosition(0, x, y);
}

void NoGUIHost::ProcessPlatformMouseButtonEvent(s32 button, bool pressed)
{
	Host::RunOnCPUThread(
		[button, pressed]() { InputManager::InvokeEvents(InputManager::MakePointerButtonKey(0, button), pressed ? 1.0f : 0.0f); });
}

void NoGUIHost::ProcessPlatformMouseWheelEvent(float x, float y)
{
	if (x != 0.0f)
		InputManager::UpdatePointerRelativeDelta(0, InputPointerAxis::WheelX, x);
	if (y != 0.0f)
		InputManager::UpdatePointerRelativeDelta(0, InputPointerAxis::WheelY, y);
}

void NoGUIHost::ProcessPlatformKeyEvent(s32 key, bool pressed)
{
	Host::RunOnCPUThread([key, pressed]() { InputManager::InvokeEvents(InputManager::MakeHostKeyboardKey(key), pressed ? 1.0f : 0.0f); });
}

void NoGUIHost::ProcessPlatformTextEvent(const char* text)
{
	if (!ImGuiManager::WantsTextInput())
		return;

	Host::RunOnCPUThread([text = std::string(text)]() { ImGuiManager::AddTextInput(std::move(text)); });
}

void NoGUIHost::PlatformWindowFocusGained()
{
	Host::RunOnCPUThread([]() {
		if (VMManager::GetState() != VMState::Paused || !Host::GetBaseBoolSettingValue("UI", "PauseOnFocusLoss", false))
			return;

		VMManager::SetPaused(false);
		s_was_paused_by_focus_loss = false;
	});
}

void NoGUIHost::PlatformWindowFocusLost()
{
	Host::RunOnCPUThread([]() {
		if (VMManager::GetState() != VMState::Running || !Host::GetBaseBoolSettingValue("UI", "PauseOnFocusLoss", false))
			return;

		s_was_paused_by_focus_loss = true;
		VMManager::SetPaused(true);
	});
}

bool NoGUIHost::GetSavedPlatformWindowGeometry(s32* x, s32* y, s32* width, s32* height)
{
	auto lock = Host::GetSettingsLock();

	bool result = s_base_settings_interface->GetIntValue("NoGUI", "WindowX", x);
	result = result && s_base_settings_interface->GetIntValue("NoGUI", "WindowY", y);
	result = result && s_base_settings_interface->GetIntValue("NoGUI", "WindowWidth", width);
	result = result && s_base_settings_interface->GetIntValue("NoGUI", "WindowHeight", height);
	return result;
}

void NoGUIHost::SavePlatformWindowGeometry(s32 x, s32 y, s32 width, s32 height)
{
	if (s_is_fullscreen)
		return;

	auto lock = Host::GetSettingsLock();
	s_base_settings_interface->SetIntValue("NoGUI", "WindowX", x);
	s_base_settings_interface->SetIntValue("NoGUI", "WindowY", y);
	s_base_settings_interface->SetIntValue("NoGUI", "WindowWidth", width);
	s_base_settings_interface->SetIntValue("NoGUI", "WindowHeight", height);
	s_base_settings_interface->Save();
}

std::string NoGUIHost::GetAppNameAndVersion()
{
	std::string ret;
	if constexpr (!XBSX2_isReleaseVersion && GIT_TAGGED_COMMIT)
	{
		ret = "XBSX2 Nightly - " GIT_TAG;
	}
	else if constexpr (XBSX2_isReleaseVersion)
	{
#define APPNAME_STRINGIZE(x) #x
		ret = "XBSX2 " APPNAME_STRINGIZE(XBSX2_VersionHi) "." APPNAME_STRINGIZE(XBSX2_VersionMid) "." APPNAME_STRINGIZE(XBSX2_VersionLo);
#undef APPNAME_STRINGIZE
	}
	else
	{
		return "XBSX2 " GIT_REV;
	}

	return ret;
}

std::string NoGUIHost::GetAppConfigSuffix()
{
#if defined(XBSX2_DEBUG)
	return " [Debug]";
#elif defined(XBSX2_DEVBUILD)
	return " [Devel]";
#else
	return std::string();
#endif
}

void NoGUIHost::StartCPUThread()
{
	s_running.store(true, std::memory_order_release);
	s_cpu_thread.Start(CPUThreadEntryPoint);
}

void NoGUIHost::StopCPUThread()
{
	if (!s_cpu_thread.Joinable())
		return;

	{
		std::unique_lock lock(s_cpu_thread_events_mutex);
		s_running.store(false, std::memory_order_release);
		s_cpu_thread_event_posted.notify_one();
	}
	s_cpu_thread.Join();
}

void NoGUIHost::ProcessCPUThreadPlatformMessages()
{
	// This is lame. On Win32, we need to pump messages, even though *we* don't have any windows
	// on the CPU thread, because SDL creates a hidden window for raw input for some game controllers.
	// If we don't do this, we don't get any controller events.
#if defined(_WIN32) && !defined(_UWP)
	MSG msg;
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
#endif
}

void NoGUIHost::ProcessCPUThreadEvents(bool block)
{
	std::unique_lock lock(s_cpu_thread_events_mutex);

	for (;;)
	{
		if (s_cpu_thread_events.empty())
		{
			if (!block || !s_running.load(std::memory_order_acquire))
				return;

			// we still need to keep polling the controllers when we're paused
			do
			{
				ProcessCPUThreadPlatformMessages();
				InputManager::PollSources();
			} while (!s_cpu_thread_event_posted.wait_for(lock, CPU_THREAD_POLL_INTERVAL, []() { return !s_cpu_thread_events.empty(); }));
		}

		// return after processing all events if we had one
		block = false;

		auto event = std::move(s_cpu_thread_events.front());
		s_cpu_thread_events.pop_front();
		lock.unlock();
		event.first();
		lock.lock();

		if (event.second)
		{
			s_blocking_cpu_events_pending--;
			s_cpu_thread_event_done.notify_one();
		}
	}
}

void NoGUIHost::CPUThreadEntryPoint()
{
	Threading::SetNameOfCurrentThread("CPU Thread");
	PerformanceMetrics::SetCPUThread(Threading::ThreadHandle::GetForCallingThread());

	if (VMManager::Internal::InitializeGlobals() && VMManager::Internal::InitializeMemory())
	{
		// we want settings loaded so we choose the correct renderer
		// this also sorts out input sources.
		VMManager::LoadSettings();

		// start the GS thread up and get it going
		if (GetMTGS().WaitForOpen())
		{
			// kick a game list refresh if we're not in batch mode
			if (!InBatchMode())
				Host::RefreshGameListAsync(false);

			CPUThreadMainLoop();

			CancelAsyncOp();
			GetMTGS().WaitForClose();
		}
		else
		{
			g_nogui_window->ReportError("Error", "MTGS open failed.");
		}

		InputManager::CloseSources();
	}
	else
	{
		g_nogui_window->ReportError("Error", "Failed to initalize globals/memory.");
	}

	VMManager::Internal::ReleaseMemory();

	PerformanceMetrics::SetCPUThread(Threading::ThreadHandle());
	g_nogui_window->QuitMessageLoop();
}

void NoGUIHost::CPUThreadMainLoop()
{
	for (;;)
	{
		switch (VMManager::GetState())
		{
			case VMState::Running:
				VMManager::Execute();
				break;

			case VMState::Paused:
				ProcessCPUThreadEvents(true);
				break;

			case VMState::Stopping:
				VMManager::Shutdown(s_save_state_on_shutdown);
				break;

			case VMState::Shutdown:
			{
				ProcessCPUThreadEvents(true);
				if (!s_running.load(std::memory_order_acquire))
					return;
			}
			break;

			default:
				break;
		}
	}
}

std::optional<std::vector<u8>> Host::ReadResourceFile(const char* filename)
{
	const std::string path(Path::Combine(EmuFolders::Resources, filename));
	std::optional<std::vector<u8>> ret(FileSystem::ReadBinaryFile(path.c_str()));
	if (!ret.has_value())
		Console.Error("Failed to read resource file '%s'", filename);
	return ret;
}

std::optional<std::string> Host::ReadResourceFileToString(const char* filename)
{
	const std::string path(Path::Combine(EmuFolders::Resources, filename));
	std::optional<std::string> ret(FileSystem::ReadFileToString(path.c_str()));
	if (!ret.has_value())
		Console.Error("Failed to read resource file to string '%s'", filename);
	return ret;
}

std::optional<time_t> Host::GetResourceFileTimestamp(const char* filename)
{
	const std::string path(Path::Combine(EmuFolders::Resources, filename));
	FILESYSTEM_STAT_DATA sd;
	if (!FileSystem::StatFile(filename, &sd))
		return std::nullopt;

	return sd.ModificationTime;
}

void Host::ReportErrorAsync(const std::string_view& title, const std::string_view& message)
{
	Console.Error(fmt::format("ReportErrorAsync: {}: {}", title, message));

	if (FullscreenUI::IsInitialized())
	{
		RunOnCPUThread([title = std::string(title), message = std::string(message)]() {
			GetMTGS().RunOnGSThread([title = std::move(title), message = std::move(message)]() {
				ImGuiFullscreen::OpenInfoMessageDialog(std::move(title), std::move(message));
			});
		});

		return;
	}

	g_nogui_window->ReportError(title, message);
}

#ifndef _UWP
bool Host::ConfirmMessage(const std::string_view& title, const std::string_view& message)
{
	if (!title.empty() && !message.empty())
	{
		Console.Warning(
			"ConfirmMessage: %.*s: %.*s", static_cast<int>(title.size()), title.data(), static_cast<int>(message.size()), message.data());
	}
	else if (!message.empty())
	{
		Console.Warning("ConfirmMessage: %.*s", static_cast<int>(message.size()), message.data());
	}

	return g_nogui_window->ConfirmMessage(title, message);
}
#endif

void Host::OnInputDeviceConnected(const std::string_view& identifier, const std::string_view& device_name)
{
	Host::AddIconOSDMessage(fmt::format("{} Connected.", identifier), ICON_FA_GAMEPAD,
		fmt::format("{} Connected.", device_name, identifier), 5.0f);
}

void Host::OnInputDeviceDisconnected(const std::string_view& identifier)
{
	Host::AddIconOSDMessage(
		fmt::format("{} Connected.", identifier), ICON_FA_GAMEPAD, fmt::format("{} Disconnected.", identifier), 5.0f);
}

bool Host::AcquireHostDisplay(HostDisplay::RenderAPI api)
{
	g_nogui_window->ExecuteInMessageLoop([api]() {
		if (g_nogui_window->CreatePlatformWindow(NoGUIHost::GetWindowTitle(VMManager::GetGameName())))
		{
			const std::optional<WindowInfo> wi(g_nogui_window->GetPlatformWindowInfo());
			if (wi.has_value())
			{
				g_host_display = HostDisplay::CreateDisplayForAPI(api);
				if (g_host_display)
				{
					if (!g_host_display->CreateRenderDevice(wi.value(), Host::GetStringSettingValue("EmuCore/GS", "Adapter", ""),
							EmuConfig.GetEffectiveVsyncMode(), Host::GetBoolSettingValue("EmuCore/GS", "ThreadedPresentation", false),
							Host::GetBoolSettingValue("EmuCore/GS", "UseDebugDevice", false)))
					{
						g_host_display.reset();
					}
				}
			}

			if (g_host_display)
				g_host_display->DoneRenderContextCurrent();
			else
				g_nogui_window->DestroyPlatformWindow();
		}

		s_platform_window_updated.Post();
	});

	s_platform_window_updated.Wait();

	if (!g_host_display)
	{
		g_nogui_window->ReportError("Error", "Failed to create host display.");
		return false;
	}

	if (!g_host_display->MakeRenderContextCurrent() || !g_host_display->InitializeRenderDevice(EmuFolders::Cache, false) ||
		!ImGuiManager::Initialize())
	{
		g_nogui_window->ReportError("Error", "Failed to initialize render device.");
		ReleaseHostDisplay();
		return false;
	}

	Console.WriteLn(Color_StrongGreen, "%s Graphics Driver Info:", HostDisplay::RenderAPIToString(g_host_display->GetRenderAPI()));
	Console.Indent().WriteLn(g_host_display->GetDriverInfo());

	if (!FullscreenUI::Initialize())
	{
		g_nogui_window->ReportError("Error", "Failed to initialize fullscreen UI");
		ReleaseHostDisplay();
		return false;
	}

	return true;
}

void Host::ReleaseHostDisplay()
{
	if (!g_host_display)
		return;

	ImGuiManager::Shutdown();

	g_host_display.reset();

	// Need to block here, otherwise the recreation message associates with the old window.
	g_nogui_window->ExecuteInMessageLoop([]() {
		g_nogui_window->DestroyPlatformWindow();
		s_platform_window_updated.Post();
	});
	s_platform_window_updated.Wait();
}

bool Host::BeginPresentFrame(bool frame_skip)
{
	if (g_host_display->BeginPresent(frame_skip))
		return true;

	// don't render imgui
	ImGuiManager::NewFrame();
	return false;
}

void Host::EndPresentFrame()
{
	if (GSDumpReplayer::IsReplayingDump())
		GSDumpReplayer::RenderUI();

	FullscreenUI::Render();
	ImGuiManager::RenderOSD();
	g_host_display->EndPresent();
	ImGuiManager::NewFrame();
}

void Host::ResizeHostDisplay(u32 new_window_width, u32 new_window_height, float new_window_scale)
{
	g_host_display->ResizeRenderWindow(new_window_width, new_window_height, new_window_scale);
	ImGuiManager::WindowResized();
}

void Host::RequestResizeHostDisplay(s32 width, s32 height)
{
	g_nogui_window->RequestRenderWindowSize(width, height);
}

#ifndef _UWP
void Host::OpenURL(const std::string_view& url)
{
	g_nogui_window->OpenURL(url);
}

bool Host::CopyTextToClipboard(const std::string_view& text)
{
	return g_nogui_window->CopyTextToClipboard(text);
}
#endif

void Host::BeginTextInput()
{
	g_nogui_window->BeginTextInput();
}

void Host::EndTextInput()
{
	g_nogui_window->EndTextInput();
}

void Host::UpdateHostDisplay()
{
	// not used in nogui, except if we do exclusive fullscreen
}

std::unique_ptr<NoGUIPlatform> NoGUIHost::CreatePlatform()
{
	std::unique_ptr<NoGUIPlatform> ret;

#if defined(_WIN32)
	ret = NoGUIPlatform::CreateWin32Platform();
#elif defined(__APPLE__)
	// nothing yet
#else
	// linux
	const char* platform = std::getenv("PCSX1_NOGUI_PLATFORM");
#ifdef NOGUI_PLATFORM_WAYLAND
	if (!ret && (!platform || StringUtil::Strcasecmp(platform, "wayland") == 0) && std::getenv("WAYLAND_DISPLAY"))
		ret = NoGUIPlatform::CreateWaylandPlatform();
#endif
#ifdef NOGUI_PLATFORM_X11
	if (!ret && (!platform || StringUtil::Strcasecmp(platform, "x11") == 0) && std::getenv("DISPLAY"))
		ret = NoGUIPlatform::CreateX11Platform();
#endif
#ifdef NOGUI_PLATFORM_VTY
	if (!ret && (!platform || StringUtil::Strcasecmp(platform, "vty") == 0))
		ret = NoGUIPlatform::CreateVTYPlatform();
#endif
#endif

	return ret;
}

std::string NoGUIHost::GetWindowTitle(const std::string& game_title)
{
	std::string suffix(GetAppConfigSuffix());
	std::string window_title;
	if (!VMManager::HasValidVM() || game_title.empty())
		window_title = GetAppNameAndVersion() + suffix;
	else
		window_title = game_title;

	return window_title;
}

void NoGUIHost::UpdateWindowTitle(const std::string& game_title)
{
	g_nogui_window->SetPlatformWindowTitle(GetWindowTitle(game_title));
}

void Host::OnVMStarting()
{
	Console.WriteLn("Host::OnVMStarting()");
	s_save_state_on_shutdown = false;
}

void Host::OnVMStarted()
{
	Console.WriteLn("Host::OnVMStarted()");
}

void Host::OnVMDestroyed()
{
	Console.WriteLn("Host::OnVMDestroyed()");
}

void Host::OnVMPaused()
{
	Console.WriteLn("Host::OnVMPaused()");
}

void Host::OnVMResumed()
{
	Console.WriteLn("Host::OnVMResumed()");
}

void Host::OnGameChanged(const std::string& disc_path, const std::string& game_serial, const std::string& game_name, u32 game_crc)
{
	Console.WriteLn("Host::OnGameChanged(%s, %s, %s, %08X)", disc_path.c_str(), game_serial.c_str(), game_name.c_str(), game_crc);
	NoGUIHost::UpdateWindowTitle(game_name);
}

void Host::OnPerformanceMetricsUpdated()
{
}

void Host::OnSaveStateLoading(const std::string_view& filename)
{
}

void Host::OnSaveStateLoaded(const std::string_view& filename, bool was_successful)
{
}

void Host::OnSaveStateSaved(const std::string_view& filename)
{
}

void Host::InvalidateSaveStateCache()
{
}

void Host::PumpMessagesOnCPUThread()
{
	NoGUIHost::ProcessCPUThreadPlatformMessages();
	NoGUIHost::ProcessCPUThreadEvents(false);
}

void Host::RunOnCPUThread(std::function<void()> function, bool block /* = false */)
{
	std::unique_lock lock(s_cpu_thread_events_mutex);
	s_cpu_thread_events.emplace_back(std::move(function), block);
	s_cpu_thread_event_posted.notify_one();
	if (block)
		s_cpu_thread_event_done.wait(lock, []() { return s_blocking_cpu_events_pending == 0; });
}

void NoGUIHost::StartAsyncOp(std::function<void(ProgressCallback*)> callback)
{
	CancelAsyncOp();
	s_async_op_thread = std::thread(AsyncOpThreadEntryPoint, std::move(callback));
}

void NoGUIHost::CancelAsyncOp()
{
	std::unique_lock lock(s_async_op_mutex);
	if (!s_async_op_thread.joinable())
		return;

	if (s_async_op_progress)
		s_async_op_progress->SetCancelled();

	lock.unlock();
	s_async_op_thread.join();
}

void NoGUIHost::AsyncOpThreadEntryPoint(std::function<void(ProgressCallback*)> callback)
{
	Threading::SetNameOfCurrentThread("Async Op");

	FullscreenUI::ProgressCallback fs_callback("async_op");
	std::unique_lock lock(s_async_op_mutex);
	s_async_op_progress = &fs_callback;

	lock.unlock();
	callback(&fs_callback);
	lock.lock();

	s_async_op_progress = nullptr;
}

void Host::RefreshGameListAsync(bool invalidate_cache)
{
	NoGUIHost::StartAsyncOp([invalidate_cache](ProgressCallback* progress) { GameList::Refresh(invalidate_cache, false, progress); });
}

void Host::CancelGameListRefresh()
{
	NoGUIHost::CancelAsyncOp();
}

bool Host::IsFullscreen()
{
	return s_is_fullscreen;
}

void Host::SetFullscreen(bool enabled)
{
	if (s_is_fullscreen == enabled)
		return;

	s_is_fullscreen = enabled;
	g_nogui_window->SetFullscreen(enabled);
}

void Host::RequestExit(bool save_state_if_running)
{
	if (VMManager::HasValidVM())
	{
		s_save_state_on_shutdown = save_state_if_running;
		VMManager::SetState(VMState::Stopping);
	}

	// clear the running flag, this'll break out of the main CPU loop once the VM is shutdown.
	s_running.store(false, std::memory_order_release);
}

void Host::RequestVMShutdown(bool allow_confirm, bool allow_save_state, bool default_save_state)
{
	if (VMManager::HasValidVM())
	{
		s_save_state_on_shutdown = allow_save_state && default_save_state;
		VMManager::SetState(VMState::Stopping);
	}
}

std::optional<u32> InputManager::ConvertHostKeyboardStringToCode(const std::string_view& str)
{
	return g_nogui_window->ConvertHostKeyboardStringToCode(str);
}

std::optional<std::string> InputManager::ConvertHostKeyboardCodeToString(u32 code)
{
	return g_nogui_window->ConvertHostKeyboardCodeToString(code);
}

SysMtgsThread& GetMTGS()
{
	return s_mtgs_thread;
}

// ------------------------------------------------------------------------
// Hotkeys
// ------------------------------------------------------------------------

BEGIN_HOTKEY_LIST(g_host_hotkeys)
END_HOTKEY_LIST()


//////////////////////////////////////////////////////////////////////////
// Interface Stuff
//////////////////////////////////////////////////////////////////////////

const IConsoleWriter* PatchesCon = &Console;

static void SignalHandler(int signal)
{
	// First try the normal (graceful) shutdown/exit.
	static bool graceful_shutdown_attempted = false;
	if (!graceful_shutdown_attempted)
	{
		std::fprintf(stderr, "Received CTRL+C, attempting graceful shutdown. Press CTRL+C again to force.\n");
		graceful_shutdown_attempted = true;

		// This could be a bit risky invoking from a signal handler... hopefully it's okay.
		// FIXME
		//QMetaObject::invokeMethod(g_main_window, &MainWindow::requestExit, Qt::QueuedConnection);
		return;
	}

	std::signal(signal, SIG_DFL);

	// MacOS is missing std::quick_exit() despite it being C++11...
#ifndef __APPLE__
	std::quick_exit(1);
#else
	_Exit(1);
#endif
}

void NoGUIHost::HookSignals()
{
	std::signal(SIGINT, SignalHandler);
	std::signal(SIGTERM, SignalHandler);
}

#ifndef _UWP

void NoGUIHost::PrintCommandLineVersion()
{
	Host::InitializeEarlyConsole();
	std::fprintf(stderr, "%s\n", (GetAppNameAndVersion() + GetAppConfigSuffix()).c_str());
	std::fprintf(stderr, "https://pcsx2.net/\n");
	std::fprintf(stderr, "\n");
}

void NoGUIHost::PrintCommandLineHelp(const char* progname)
{
	PrintCommandLineVersion();
	std::fprintf(stderr, "Usage: %s [parameters] [--] [boot filename]\n", progname);
	std::fprintf(stderr, "\n");
	std::fprintf(stderr, "  -help: Displays this information and exits.\n");
	std::fprintf(stderr, "  -version: Displays version information and exits.\n");
	std::fprintf(stderr, "  -batch: Enables batch mode (exits after shutting down).\n");
	std::fprintf(stderr, "  -elf <file>: Overrides the boot ELF with the specified filename.\n");
	std::fprintf(stderr, "  -disc <path>: Uses the specified host DVD drive as a source.\n");
	std::fprintf(stderr, "  -bios: Starts the BIOS (System Menu/OSDSYS).\n");
	std::fprintf(stderr, "  -fastboot: Force fast boot for provided filename.\n");
	std::fprintf(stderr, "  -slowboot: Force slow boot for provided filename.\n");
	std::fprintf(stderr, "  -state <index>: Loads specified save state by index.\n");
	std::fprintf(stderr, "  -statefile <filename>: Loads state from the specified filename.\n");
	std::fprintf(stderr, "  -fullscreen: Enters fullscreen mode immediately after starting.\n");
	std::fprintf(stderr, "  -nofullscreen: Prevents fullscreen mode from triggering if enabled.\n");
	std::fprintf(stderr, "  --: Signals that no more arguments will follow and the remaining\n"
						 "    parameters make up the filename. Use when the filename contains\n"
						 "    spaces or starts with a dash.\n");
	std::fprintf(stderr, "\n");
}

std::shared_ptr<VMBootParameters>& NoGUIHost::AutoBoot(std::shared_ptr<VMBootParameters>& autoboot)
{
	if (!autoboot)
		autoboot = std::make_shared<VMBootParameters>();

	return autoboot;
}

bool NoGUIHost::ParseCommandLineOptions(int argc, char* argv[], std::shared_ptr<VMBootParameters>& autoboot)
{
	bool no_more_args = false;

	for (int i = 1; i < argc; i++)
	{
		if (!no_more_args)
		{
#define CHECK_ARG(str) !std::strcmp(argv[i], str)
#define CHECK_ARG_PARAM(str) (!std::strcmp(argv[i], str) && ((i + 1) < argc))

			if (CHECK_ARG("-help"))
			{
				PrintCommandLineHelp(argv[0]);
				return false;
			}
			else if (CHECK_ARG("-version"))
			{
				PrintCommandLineVersion();
				return false;
			}
			else if (CHECK_ARG("-batch"))
			{
				NoGUIHost::SetBatchMode(true);
				continue;
			}
			else if (CHECK_ARG("-fastboot"))
			{
				AutoBoot(autoboot)->fast_boot = true;
				continue;
			}
			else if (CHECK_ARG("-slowboot"))
			{
				AutoBoot(autoboot)->fast_boot = false;
				continue;
			}
			else if (CHECK_ARG_PARAM("-state"))
			{
				AutoBoot(autoboot)->state_index = std::atoi(argv[++i]);
				continue;
			}
			else if (CHECK_ARG_PARAM("-statefile"))
			{
				AutoBoot(autoboot)->save_state = argv[++i];
				continue;
			}
			else if (CHECK_ARG_PARAM("-elf"))
			{
				AutoBoot(autoboot)->elf_override = argv[++i];
				continue;
			}
			else if (CHECK_ARG_PARAM("-disc"))
			{
				AutoBoot(autoboot)->source_type = CDVD_SourceType::Disc;
				AutoBoot(autoboot)->filename = argv[++i];
				continue;
			}
			else if (CHECK_ARG("-bios"))
			{
				AutoBoot(autoboot)->source_type = CDVD_SourceType::NoDisc;
				continue;
			}
			else if (CHECK_ARG("-fullscreen"))
			{
				AutoBoot(autoboot)->fullscreen = true;
				continue;
			}
			else if (CHECK_ARG("-nofullscreen"))
			{
				AutoBoot(autoboot)->fullscreen = false;
				continue;
			}
			else if (CHECK_ARG("--"))
			{
				no_more_args = true;
				continue;
			}
			else if (argv[i][0] == '-')
			{
				Host::InitializeEarlyConsole();
				std::fprintf(stderr, "Unknown parameter: '%s'", argv[i]);
				return false;
			}

#undef CHECK_ARG
#undef CHECK_ARG_PARAM
		}

		if (!AutoBoot(autoboot)->filename.empty())
			AutoBoot(autoboot)->filename += ' ';

		AutoBoot(autoboot)->filename += argv[i];
	}

	// check autoboot parameters, if we set something like fullscreen without a bios
	// or disc, we don't want to actually start.
	if (autoboot && !autoboot->source_type.has_value() && autoboot->filename.empty() && autoboot->elf_override.empty())
	{
		Host::InitializeEarlyConsole();
		Console.Warning("Skipping autoboot due to no boot parameters.");
		autoboot.reset();
	}

	// if we don't have autoboot, we definitely don't want batch mode (because that'll skip
	// scanning the game list).
	if (NoGUIHost::InBatchMode() && !autoboot)
	{
		Host::InitializeEarlyConsole();
		Console.Warning("Disabling batch mode, because we have no autoboot.");
		SetBatchMode(false);
	}

	return true;
}

int main(int argc, char* argv[])
{
	CrashHandler::Install();

	std::shared_ptr<VMBootParameters> autoboot;
	if (!NoGUIHost::ParseCommandLineOptions(argc, argv, autoboot))
		return EXIT_FAILURE;

	g_nogui_window = NoGUIHost::CreatePlatform();
	if (!g_nogui_window)
		return EXIT_FAILURE;

	if (!NoGUIHost::InitializeConfig())
	{
		g_nogui_window->ReportError("Error", "Failed to initialize config.");
		return false;
	}

	// the rest of initialization happens on the CPU thread.
	NoGUIHost::HookSignals();
	NoGUIHost::StartCPUThread();

	if (autoboot)
		NoGUIHost::StartVM(std::move(autoboot));

	g_nogui_window->RunMessageLoop();

	NoGUIHost::StopCPUThread();
	g_nogui_window.reset();
	return EXIT_SUCCESS;
}

#ifdef _WIN32

#include "common/RedtapeWindows.h"
#include <shellapi.h>

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	std::vector<std::string> argc_strings;
	argc_strings.reserve(1);

	// CommandLineToArgvW() only adds the program path if the command line is empty?!
	argc_strings.push_back(FileSystem::GetProgramPath());

	if (std::wcslen(lpCmdLine) > 0)
	{
		int argc;
		LPWSTR* argv_wide = CommandLineToArgvW(lpCmdLine, &argc);
		if (argv_wide)
		{
			for (int i = 0; i < argc; i++)
				argc_strings.push_back(StringUtil::WideStringToUTF8String(argv_wide[i]));

			LocalFree(argv_wide);
		}
	}

	std::vector<char*> argc_pointers;
	argc_pointers.reserve(argc_strings.size());
	for (std::string& arg : argc_strings)
		argc_pointers.push_back(arg.data());

	return main(static_cast<int>(argc_pointers.size()), argc_pointers.data());
}

#endif

#endif