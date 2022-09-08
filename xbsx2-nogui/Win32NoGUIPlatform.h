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
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <atomic>

#include "common/RedtapeWindows.h"

#include "NoGUIPlatform.h"

class Win32NoGUIPlatform : public NoGUIPlatform
{
public:
	Win32NoGUIPlatform();
	~Win32NoGUIPlatform();

	bool Initialize();

	void ReportError(const std::string_view& title, const std::string_view& message) override;
	bool ConfirmMessage(const std::string_view& title, const std::string_view& message) override;

	void SetDefaultConfig(SettingsInterface& si) override;

	bool CreatePlatformWindow(std::string title) override;
	void DestroyPlatformWindow() override;
	std::optional<WindowInfo> GetPlatformWindowInfo() override;
	void SetPlatformWindowTitle(std::string title) override;
	void* GetPlatformWindowHandle() override;

	std::optional<u32> ConvertHostKeyboardStringToCode(const std::string_view& str) override;
	std::optional<std::string> ConvertHostKeyboardCodeToString(u32 code) override;

	void RunMessageLoop() override;
	void ExecuteInMessageLoop(std::function<void()> func) override;
	void QuitMessageLoop() override;

	void SetFullscreen(bool enabled) override;

	bool RequestRenderWindowSize(s32 new_window_width, s32 new_window_height) override;

	bool OpenURL(const std::string_view& url) override;
	bool CopyTextToClipboard(const std::string_view& text) override;

private:
	enum : u32
	{
		WM_FIRST = WM_USER + 1337,
		WM_FUNC = WM_FIRST,
		WM_WAKEUP,
		WM_LAST = WM_WAKEUP
	};

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND m_hwnd{};
	DWORD m_window_thread_id = 0;
	RECT m_windowed_rect = {};
	float m_window_scale = 1.0f;

	std::atomic_bool m_message_loop_running{false};
	std::atomic_bool m_fullscreen{false};

	DWORD m_last_mouse_buttons = 0;
};