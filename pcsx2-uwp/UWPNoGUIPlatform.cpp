/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2022  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"

#include "UWPNoGUIPlatform.h"
#include "UWPKeyNames.h"
#include "NoGUIHost.h"

#include "common/FileSystem.h"
#include "common/Path.h"
#include "common/StringUtil.h"
#include "common/Threading.h"

#include "pcsx2/HostSettings.h"
#include "pcsx2/windows/resource.h"

#include <gamingdeviceinformation.h>
#include <winrt/Windows.Graphics.Display.Core.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.System.Profile.h>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::Display;
using namespace winrt::Windows::Graphics::Display::Core;
using namespace winrt::Windows::System;
using namespace winrt::Windows::System::Profile;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Popups;
using namespace winrt::Windows::UI::ViewManagement;

static bool IsRunningOnXbox()
{
	const auto version_info = AnalyticsInfo::VersionInfo();
	const auto device_family = version_info.DeviceFamily();
	return (device_family == L"Windows.Xbox");
}

UWPNoGUIPlatform::UWPNoGUIPlatform() = default;

UWPNoGUIPlatform::~UWPNoGUIPlatform() = default;

winrt::Windows::ApplicationModel::Core::IFrameworkView UWPNoGUIPlatform::CreateView()
{
	return *this;
}

void UWPNoGUIPlatform::Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView& a)
{
	CoreApplication::UnhandledErrorDetected({this, &UWPNoGUIPlatform::OnUnhandledErrorDetected});
	CoreApplication::Suspending({this, &UWPNoGUIPlatform::OnSuspending});
	CoreApplication::Resuming({this, &UWPNoGUIPlatform::OnResuming});
}

void UWPNoGUIPlatform::Load(const winrt::hstring&) {}

void UWPNoGUIPlatform::Uninitialize() {}

void UWPNoGUIPlatform::Run()
{
	// this is a bit lame, because it's a unique_ptr..
	g_nogui_window = std::unique_ptr<UWPNoGUIPlatform>(this);

	if (!NoGUIHost::Initialize())
	{
		CoreApplication::Exit();
		g_nogui_window.release();
		return;
	}

	RunMessageLoop();

	g_nogui_window.release();
}

void UWPNoGUIPlatform::SetWindow(const winrt::Windows::UI::Core::CoreWindow& window)
{
	m_window = window;
	m_dispatcher = m_window.Dispatcher();

	window.Closed({this, &UWPNoGUIPlatform::OnClosed});
	window.SizeChanged({this, &UWPNoGUIPlatform::OnSizeChanged});
	window.KeyDown({this, &UWPNoGUIPlatform::OnKeyDown});
	window.KeyUp({this, &UWPNoGUIPlatform::OnKeyUp});
	window.CharacterReceived({this, &UWPNoGUIPlatform::OnCharacterReceived});
	window.PointerPressed({this, &UWPNoGUIPlatform::OnPointerPressed});
	window.PointerReleased({this, &UWPNoGUIPlatform::OnPointerReleased});
	window.PointerMoved({this, &UWPNoGUIPlatform::OnPointerMoved});
	window.PointerWheelChanged({this, &UWPNoGUIPlatform::OnPointerWheelChanged});
}

void UWPNoGUIPlatform::OnUnhandledErrorDetected(const IInspectable&, const winrt::Windows::ApplicationModel::Core::UnhandledErrorDetectedEventArgs& args)
{
	/*Console.WriteLn("foo");
	ReportError("Foo", "bar");*/
}

void UWPNoGUIPlatform::OnSuspending(const IInspectable&, const winrt::Windows::ApplicationModel::SuspendingEventArgs& args)
{
	// TODO: This is where we would save state.
}

void UWPNoGUIPlatform::OnResuming(const IInspectable&, const IInspectable&)
{
}

void UWPNoGUIPlatform::OnClosed(const IInspectable&, const winrt::Windows::UI::Core::CoreWindowEventArgs& args)
{
	Host::RequestExit(EmuConfig.SaveStateOnShutdown);
	args.Handled(true);
}

void UWPNoGUIPlatform::OnSizeChanged(const IInspectable&, const winrt::Windows::UI::Core::WindowSizeChangedEventArgs& args)
{
	const auto size = args.Size();
	const float width = size.Width;
	const float height = size.Height;

	NoGUIHost::ProcessPlatformWindowResize(static_cast<s32>(width), static_cast<s32>(height), m_window_info.surface_scale);
}

void UWPNoGUIPlatform::OnKeyDown(const IInspectable&, const winrt::Windows::UI::Core::KeyEventArgs& args)
{
	// Ignore Gamepad input as that's handled by XInput
	if (args.VirtualKey() <= VirtualKey::GoHome)
	{
		const auto status = args.KeyStatus();
		if (!status.WasKeyDown && !status.IsKeyReleased)
			NoGUIHost::ProcessPlatformKeyEvent(static_cast<s32>(args.VirtualKey()), true);
	}

	args.Handled(true);
}

void UWPNoGUIPlatform::OnKeyUp(const IInspectable&, const winrt::Windows::UI::Core::KeyEventArgs& args)
{
	// Ignore Gamepad input as that's handled by XInput
	if (args.VirtualKey() <= VirtualKey::GoHome)
	{
		const auto status = args.KeyStatus();
		if (status.WasKeyDown && status.IsKeyReleased)
			NoGUIHost::ProcessPlatformKeyEvent(static_cast<s32>(args.VirtualKey()), false);
	}

	args.Handled(true);
}

void UWPNoGUIPlatform::OnCharacterReceived(const IInspectable&, const winrt::Windows::UI::Core::CharacterReceivedEventArgs& args)
{
}

void UWPNoGUIPlatform::OnPointerPressed(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args)
{
	const auto pointer = args.CurrentPoint();
	if (pointer.PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Mouse)
		UpdateMouseButtonState(pointer);

	args.Handled(true);
}

void UWPNoGUIPlatform::OnPointerReleased(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args)
{
	const auto pointer = args.CurrentPoint();
	if (pointer.PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Mouse)
		UpdateMouseButtonState(pointer);

	args.Handled(true);
}

void UWPNoGUIPlatform::OnPointerMoved(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args)
{
	const auto pointer = args.CurrentPoint();
	if (pointer.PointerDevice().PointerDeviceType() == winrt::Windows::Devices::Input::PointerDeviceType::Mouse)
	{
		const auto pos = pointer.Position();
		const float x = pos.X * m_window_info.surface_scale;
		const float y = pos.Y * m_window_info.surface_scale;

		NoGUIHost::ProcessPlatformMouseMoveEvent(static_cast<s32>(x), static_cast<s32>(y));
		UpdateMouseButtonState(pointer);
	}

	args.Handled(true);
}

void UWPNoGUIPlatform::OnPointerWheelChanged(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args)
{
	const auto pointer = args.CurrentPoint();
	const auto properties = pointer.Properties();
	const s32 delta = properties.MouseWheelDelta();
	const bool horizontal = properties.IsHorizontalMouseWheel();
	const float d = std::clamp(static_cast<float>(delta) / static_cast<float>(WHEEL_DELTA), -1.0f, 1.0f);
	NoGUIHost::ProcessPlatformMouseWheelEvent(horizontal ? d : 0.0f, horizontal ? d : 0.0f);
	args.Handled(true);
}

void UWPNoGUIPlatform::UpdateMouseButtonState(const winrt::Windows::UI::Input::PointerPoint& point)
{
	const auto properties = point.Properties();
	const bool states[3] = {properties.IsLeftButtonPressed(), properties.IsRightButtonPressed(),
		properties.IsMiddleButtonPressed()};

	for (u32 i = 0; i < 3; i++)
	{
		if (m_last_mouse_state[i] == states[i])
			continue;

		NoGUIHost::ProcessPlatformMouseButtonEvent(i, states[i]);
		m_last_mouse_state[i] = states[i];
	}
}

void UWPNoGUIPlatform::ReportError(const std::string_view& title, const std::string_view& message)
{
	const winrt::hstring title_copy(StringUtil::UTF8StringToWideString(title));
	const winrt::hstring message_copy(StringUtil::UTF8StringToWideString(message));
	MessageDialog popup(message_copy, title_copy);
	popup.ShowAsync();
}

void UWPNoGUIPlatform::SetDefaultConfig(SettingsInterface& si)
{
	const std::string games_dir(Path::Combine(EmuFolders::DataRoot, "games"));
	if (!FileSystem::DirectoryExists(games_dir.c_str()))
		FileSystem::CreateDirectoryPath(games_dir.c_str(), false);

	// Disable things that aren't present in UWP builds.
	si.SetBoolValue("InputSources", "SDL", false);
	si.SetBoolValue("InputSources", "XInput", true);
	si.AddToStringList("GameList", "RecursivePaths", games_dir.c_str());

	if (IsRunningOnXbox())
	{
		// For Xbox, default to DX12. Too bad if it's not in game mode.
		si.SetIntValue("EmuCore/GS", "Renderer", static_cast<int>(GSRendererType::DX12));
		si.SetBoolValue("EmuCore/Speedhacks", "vuThread", true);

		// Map first controller.
		si.SetStringValue("Pad1", "Type", "DualShock2");
		si.SetStringValue("Pad1", "Up", "XInput-0/DPadUp");
		si.SetStringValue("Pad1", "Right", "XInput-0/DPadRight");
		si.SetStringValue("Pad1", "Down", "XInput-0/DPadDown");
		si.SetStringValue("Pad1", "Left", "XInput-0/DPadLeft");
		si.SetStringValue("Pad1", "Triangle", "XInput-0/Y");
		si.SetStringValue("Pad1", "Circle", "XInput-0/B");
		si.SetStringValue("Pad1", "Cross", "XInput-0/A");
		si.SetStringValue("Pad1", "Square", "XInput-0/X");
		si.SetStringValue("Pad1", "Select", "XInput-0/Back");
		si.SetStringValue("Pad1", "Start", "XInput-0/Start");
		si.SetStringValue("Pad1", "L1", "XInput-0/LeftShoulder");
		si.SetStringValue("Pad1", "L2", "XInput-0/+LeftTrigger");
		si.SetStringValue("Pad1", "R1", "XInput-0/RightShoulder");
		si.SetStringValue("Pad1", "R2", "XInput-0/+RightTrigger");
		si.SetStringValue("Pad1", "L3", "XInput-0/LeftStick");
		si.SetStringValue("Pad1", "R3", "XInput-0/RightStick");
		si.SetStringValue("Pad1", "LUp", "XInput-0/-LeftY");
		si.SetStringValue("Pad1", "LRight", "XInput-0/+LeftX");
		si.SetStringValue("Pad1", "LDown", "XInput-0/+LeftY");
		si.SetStringValue("Pad1", "LLeft", "XInput-0/-LeftX");
		si.SetStringValue("Pad1", "RUp", "XInput-0/-RightY");
		si.SetStringValue("Pad1", "RRight", "XInput-0/+RightX");
		si.SetStringValue("Pad1", "RDown", "XInput-0/+RightY");
		si.SetStringValue("Pad1", "RLeft", "XInput-0/-RightX");
		si.SetStringValue("Pad1", "SmallMotor", "XInput-0/SmallMotor");
		si.SetStringValue("Pad1", "LargeMotor", "XInput-0/LargeMotor");

		// LB+RB => Open Pause Menu.
		si.SetStringValue("Hotkeys", "OpenPauseMenu", "XInput-0/Back & XInput-0/Start");
	}
}

bool UWPNoGUIPlatform::CreatePlatformWindow(std::string title)
{
	if (!m_appview)
	{
		const bool start_fullscreen = Host::GetBaseBoolSettingValue("UI", "StartFullscreen", false);
		m_appview = ApplicationView::GetForCurrentView();
		m_appview.PreferredLaunchWindowingMode(start_fullscreen ? ApplicationViewWindowingMode::FullScreen : ApplicationViewWindowingMode::Auto);
		m_window.Activate();
	}

	const auto di = DisplayInformation::GetForCurrentView();
	const auto hdi = HdmiDisplayInformation::GetForCurrentView();
	const s32 resolution_scale = static_cast<s32>(di.ResolutionScale());

	m_window_info.type = WindowInfo::Type::WinRT;
	m_window_info.window_handle = winrt::get_unknown(m_window);
	m_window_info.surface_scale = static_cast<float>(resolution_scale) / 100.0f;
	m_window_info.surface_width = static_cast<u32>(m_window.Bounds().Width * m_window_info.surface_scale);
	m_window_info.surface_height = static_cast<s32>(m_window.Bounds().Height * m_window_info.surface_scale);
	if (hdi)
	{
		try
		{
			const auto dm = hdi.GetCurrentDisplayMode();
			const u32 hdmi_width = dm.ResolutionWidthInRawPixels();
			const u32 hdmi_height = dm.ResolutionHeightInRawPixels();
			m_window_info.surface_refresh_rate = static_cast<float>(dm.RefreshRate());
			Console.WriteLn("HDMI mode: %ux%u @ %.2f hz", hdmi_width, hdmi_height, m_window_info.surface_refresh_rate);

			// If we're running on Xbox, use the HDMI mode instead of the CoreWindow size.
			// In UWP, the CoreWindow is always 1920x1080, even when running at 4K.
			if (IsRunningOnXbox())
			{
				GAMING_DEVICE_MODEL_INFORMATION gdinfo = {};
				if (SUCCEEDED(GetGamingDeviceModelInformation(&gdinfo)) && gdinfo.vendorId == GAMING_DEVICE_VENDOR_ID_MICROSOFT)
				{
					Console.WriteLn("Overriding core window size %ux%u with HDMI size %ux%u", m_window_info.surface_width, m_window_info.surface_height,
						hdmi_width, hdmi_height);
					m_window_info.surface_scale *= static_cast<float>(hdmi_width) / static_cast<float>(m_window_info.surface_width);
					m_window_info.surface_width = hdmi_width;
					m_window_info.surface_height = hdmi_height;
				}
			}
		}
		catch (const winrt::hresult_error&)
		{
		}
	}

	return true;
}

void UWPNoGUIPlatform::DestroyPlatformWindow()
{
	// just persist it until exit
}

std::optional<WindowInfo> UWPNoGUIPlatform::GetPlatformWindowInfo()
{
	return m_window ? std::optional<WindowInfo>(m_window_info) : std::nullopt;
}

void UWPNoGUIPlatform::SetPlatformWindowTitle(std::string title)
{
	if (!m_appview)
		return;

	m_appview.Title(StringUtil::UTF8StringToWideString(title));
}

std::optional<u32> UWPNoGUIPlatform::ConvertHostKeyboardStringToCode(const std::string_view& str)
{
	std::optional<VirtualKey> converted(UWPKeyNames::GetKeyCodeForName(str));
	return converted.has_value() ? std::optional<u32>(static_cast<u32>(converted.value())) : std::nullopt;
}

std::optional<std::string> UWPNoGUIPlatform::ConvertHostKeyboardCodeToString(u32 code)
{
	const char* converted = UWPKeyNames::GetKeyName(static_cast<VirtualKey>(code));
	return converted ? std::optional<std::string>(converted) : std::nullopt;
}

void UWPNoGUIPlatform::RunMessageLoop()
{
	m_dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void UWPNoGUIPlatform::ExecuteInMessageLoop(std::function<void()> func)
{
	m_dispatcher.RunAsync(CoreDispatcherPriority::Normal, std::move(func));
}

void UWPNoGUIPlatform::QuitMessageLoop()
{
	m_dispatcher.RunAsync(CoreDispatcherPriority::Normal, []() { CoreApplication::Exit(); });
}

void UWPNoGUIPlatform::SetFullscreen(bool enabled)
{
	m_dispatcher.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [this, enabled]() {
		if (enabled)
			m_appview.TryEnterFullScreenMode();
		else
			m_appview.ExitFullScreenMode();
	});
}

bool UWPNoGUIPlatform::RequestRenderWindowSize(s32 new_window_width, s32 new_window_height)
{
	if (!m_appview)
		return false;

	return m_appview.TryResizeView(Size(static_cast<float>(new_window_width), static_cast<float>(new_window_height)));
}

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(winrt::make<UWPNoGUIPlatform>());
}
