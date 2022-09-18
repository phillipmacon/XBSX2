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

#pragma once
#include "Frontend/InputSource.h"
#include "common/RedtapeWindows.h"
#include <array>
#include <functional>
#include <mutex>
#include <vector>

class SettingsInterface;

class Win32RawInputSource final : public InputSource
{
public:
  Win32RawInputSource();
  ~Win32RawInputSource();

  bool Initialize(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock) override;
  void UpdateSettings(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock) override;
  void Shutdown() override;

  void PollEvents() override;
  std::vector<std::pair<std::string, std::string>> EnumerateDevices() override;
  std::vector<InputBindingKey> EnumerateMotors() override;
  bool GetGenericBindingMapping(const std::string_view& device, GenericInputBindingMapping* mapping) override;
  void UpdateMotorState(InputBindingKey key, float intensity) override;
  void UpdateMotorState(InputBindingKey large_key, InputBindingKey small_key, float large_intensity, float small_intensity) override;

  std::optional<InputBindingKey> ParseKeyString(const std::string_view& device, const std::string_view& binding) override;
  std::string ConvertKeyToString(InputBindingKey key) override;

private:
  struct MouseState
  {
    HANDLE device;
    u32 button_state;
    s32 last_x;
    s32 last_y;
  };

  static bool RegisterDummyClass();
  static LRESULT CALLBACK DummyWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  bool CreateDummyWindow();
  void DestroyDummyWindow();
  bool OpenDevices();
  void CloseDevices();

  bool ProcessRawInputEvent(const RAWINPUT* event);

  HWND m_dummy_window = {};
  u32 m_num_keyboards = 0;
  u32 m_num_mice = 0;

  std::vector<MouseState> m_mice;
};
