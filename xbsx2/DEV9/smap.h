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
#include "DEV9.h"

u8 smap_read8(u32 addr);
u16 smap_read16(u32 addr);
u32 smap_read32(u32 addr);

void smap_write8(u32 addr, u8 value);
void smap_write16(u32 addr, u16 value);
void smap_write32(u32 addr, u32 value);

void smap_readDMA8Mem(u32* pMem, int size);
void smap_writeDMA8Mem(u32* pMem, int size);
void smap_async(u32 cycles);
