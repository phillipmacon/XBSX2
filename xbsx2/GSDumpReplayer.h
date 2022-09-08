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

#include "XBSX2Base.h"
#include <string>
#include <vector>

namespace GSDumpReplayer
{
bool IsReplayingDump();

/// If set, playback will repeat once it reaches the last frame.
void SetLoopCount(s32 loop_count = 0);

bool Initialize(const char* filename);
void Reset();
void Shutdown();

std::string GetDumpSerial();
u32 GetDumpCRC();

void RenderUI();
}