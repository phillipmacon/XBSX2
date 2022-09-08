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
#include "Xbsx2Defs.h"
#include <cstdint>
#include <cstddef>

struct fastjmp_buf
{
#if defined(_WIN32)
	static constexpr std::size_t BUF_SIZE = 240;
#else
	static constexpr std::size_t BUF_SIZE = 64;
#endif

	alignas(16) std::uint8_t buf[BUF_SIZE];
};

extern "C" {
int fastjmp_set(fastjmp_buf* buf);
__noreturn void fastjmp_jmp(const fastjmp_buf* buf, int ret);
}
