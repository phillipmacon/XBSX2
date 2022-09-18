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

#include "common/Xbsx2Defs.h"
#include <string>
#include <mutex>

class SettingsInterface;

#ifndef _UWP
/// Sets host-specific default settings.
void SetDefaultUISettings(SettingsInterface& si);
#endif

namespace Host
{
} // namespace Host

namespace CommonHost
{
	/// Initializes critical folders (AppRoot, DataRoot, Settings). Call once on startup.
	bool InitializeCriticalFolders();

	/// Checks settings version. Call once on startup. If it returns false, you should prompt the user to reset.
	bool CheckSettingsVersion();

	/// Loads early settings. Call once on startup.
	void LoadStartupSettings();

	/// Sets default settings for the specified categories.
	void SetDefaultSettings(SettingsInterface& si, bool folders, bool core, bool controllers, bool hotkeys, bool ui);
} // namespace CommonHost