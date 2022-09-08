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

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Qt build requires Windows 10+, WX Windows 8.1+.
#if defined(_WIN32_WINNT) && !defined(_UWP)
#ifdef XBSX2_CORE
#define _WIN32_WINNT 0x0A00 // Windows 10
#else
#define _WIN32_WINNT 0x0603 // Windows 8.1
#endif
#endif

#include <windows.h>
#include <VersionHelpers.h>
#include <ShTypes.h>
#include <timeapi.h>
#include <tchar.h>

#endif
