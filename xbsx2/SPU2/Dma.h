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

#define MADR (Index == 0 ? HW_DMA4_MADR : HW_DMA7_MADR)
#define TADR (Index == 0 ? HW_DMA4_TADR : HW_DMA7_TADR)

extern void DMALogOpen();
extern void DMA4LogWrite(void* lpData, u32 ulSize);
extern void DMA7LogWrite(void* lpData, u32 ulSize);
extern void DMALogClose();
