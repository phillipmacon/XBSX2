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

#pragma once
#include "pcsx2/Host.h"
#include "pcsx2/HostDisplay.h"
#include "pcsx2/HostSettings.h"
#include "pcsx2/Frontend/InputManager.h"
#include "pcsx2/VMManager.h"
#include <functional>
#include <memory>
#include <optional>

namespace NoGUIHost
{
	bool Initialize();
	void Shutdown();

	/// Sets batch mode (exit after game shutdown).
	bool InBatchMode();
	void SetBatchMode(bool enabled);

	/// Starts the virtual machine.
	void StartVM(std::shared_ptr<VMBootParameters> params);

	/// Returns the application name and version, optionally including debug/devel config indicator.
	std::string GetAppNameAndVersion();

	/// Returns the debug/devel config indicator.
	std::string GetAppConfigSuffix();

	/// Thread-safe settings access.
	SettingsInterface* GetBaseSettingsInterface();
	std::string GetBaseStringSettingValue(const char* section, const char* key, const char* default_value = "");
	bool GetBaseBoolSettingValue(const char* section, const char* key, bool default_value = false);
	int GetBaseIntSettingValue(const char* section, const char* key, int default_value = 0);
	float GetBaseFloatSettingValue(const char* section, const char* key, float default_value = 0.0f);
	std::vector<std::string> GetBaseStringListSetting(const char* section, const char* key);
	void SetBaseBoolSettingValue(const char* section, const char* key, bool value);
	void SetBaseIntSettingValue(const char* section, const char* key, int value);
	void SetBaseFloatSettingValue(const char* section, const char* key, float value);
	void SetBaseStringSettingValue(const char* section, const char* key, const char* value);
	void SetBaseStringListSettingValue(const char* section, const char* key, const std::vector<std::string>& values);
	bool AddBaseValueToStringList(const char* section, const char* key, const char* value);
	bool RemoveBaseValueFromStringList(const char* section, const char* key, const char* value);
	void RemoveBaseSettingValue(const char* section, const char* key);
	void SaveSettings();

	/// Called on the UI thread in response to various events.
	void ProcessPlatformWindowResize(s32 width, s32 height, float scale);
	void ProcessPlatformMouseMoveEvent(float x, float y);
	void ProcessPlatformMouseButtonEvent(s32 button, bool pressed);
	void ProcessPlatformMouseWheelEvent(float x, float y);
	void ProcessPlatformKeyEvent(s32 key, bool pressed);
	void PlatformWindowFocusGained();
	void PlatformWindowFocusLost();
	bool GetSavedPlatformWindowGeometry(s32* x, s32* y, s32* width, s32* height);
	void SavePlatformWindowGeometry(s32 x, s32 y, s32 width, s32 height);
} // namespace NoGUIHost
