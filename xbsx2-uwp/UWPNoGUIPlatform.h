/*  XBSX2 - PS2 Emulator for Xbox Consoles
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

#pragma once

#include <atomic>
#include <functional>

#include "common/RedtapeWindows.h"

#include "NoGUIPlatform.h"
#include "URISchemeParser.h"
#include "VMManager.h"

#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Devices.Input.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/base.h>


class UWPNoGUIPlatform : public NoGUIPlatform,
						 public winrt::implements<UWPNoGUIPlatform, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource,
							 winrt::Windows::ApplicationModel::Core::IFrameworkView>
{
public:
	UWPNoGUIPlatform();
	~UWPNoGUIPlatform();

	void ReportError(const std::string_view& title, const std::string_view& message) override;

	void SetDefaultControllerConfig(SettingsInterface& si) override;

	bool CreatePlatformWindow(std::string title) override;
	void DestroyPlatformWindow() override;
	std::optional<WindowInfo> GetPlatformWindowInfo() override;
	void SetPlatformWindowTitle(std::string title) override;

	std::optional<u32> ConvertHostKeyboardStringToCode(const std::string_view& str) override;
	std::optional<std::string> ConvertHostKeyboardCodeToString(u32 code) override;

	void RunMessageLoop() override;
	void ExecuteInMessageLoop(std::function<void()> func) override;
	void QuitMessageLoop() override;

	void SetFullscreen(bool enabled) override;

	void BeginTextInput() override;
	void EndTextInput() override;

	bool RequestRenderWindowSize(s32 new_window_width, s32 new_window_height) override;

	winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();
	void Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView&);
	void Load(const winrt::hstring&);
	void Uninitialize();
	void Run();
	void SetWindow(const winrt::Windows::UI::Core::CoreWindow& window);

	void OnUnhandledErrorDetected(const IInspectable&, const winrt::Windows::ApplicationModel::Core::UnhandledErrorDetectedEventArgs& args);
	void OnActivated(const winrt::Windows::ApplicationModel::Core::CoreApplicationView&, const winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs& args); 
	void OnSuspending(const IInspectable&, const winrt::Windows::ApplicationModel::SuspendingEventArgs& args);
	void OnResuming(const IInspectable&, const IInspectable&);
	void OnEnteredBackground(const IInspectable& ,const winrt::Windows::ApplicationModel::EnteredBackgroundEventArgs& args);
	void OnClosed(const IInspectable&, const winrt::Windows::UI::Core::CoreWindowEventArgs& args);
	void OnSizeChanged(const IInspectable&, const winrt::Windows::UI::Core::WindowSizeChangedEventArgs& args);
	void OnKeyDown(const IInspectable&, const winrt::Windows::UI::Core::KeyEventArgs& args);
	void OnKeyUp(const IInspectable&, const winrt::Windows::UI::Core::KeyEventArgs& args);
	void OnCharacterReceived(const IInspectable&, const winrt::Windows::UI::Core::CharacterReceivedEventArgs& args);
	void OnPointerPressed(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args);
	void OnPointerReleased(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args);
	void OnPointerMoved(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args);
	void OnPointerWheelChanged(const IInspectable&, const winrt::Windows::UI::Core::PointerEventArgs& args);
	void UpdateMouseButtonState(const winrt::Windows::UI::Input::PointerPoint& point);

private:
	winrt::Windows::UI::Core::CoreWindow m_window{nullptr};
	winrt::Windows::UI::Core::CoreDispatcher m_dispatcher{nullptr};
	winrt::Windows::UI::ViewManagement::ApplicationView m_appview{nullptr};
	WindowInfo m_window_info = {};

	bool m_last_mouse_state[3] = {};

	//copied from pcsx2-nogui Main.cpp
	std::shared_ptr<VMBootParameters> autoboot;
	std::shared_ptr<VMBootParameters>& AutoBoot(std::shared_ptr<VMBootParameters>& autoboot);
	bool ParseCommandLineOptions(int argc, char* argv[], std::shared_ptr<VMBootParameters>& autoboot);

	//process arguments when app is activated using Uri Scheme
	URISchemeParser m_uriSchemeParser;
	bool m_launchOnExitShutdown;
};
