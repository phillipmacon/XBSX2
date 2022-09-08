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

extern void iDumpRegisters(u32 startpc, u32 temp);
extern void iDumpPsxRegisters(u32 startpc, u32 temp);
extern void iDumpVU0Registers();
extern void iDumpVU1Registers();
extern void iDumpBlock(u32 ee_pc, u32 ee_size, uptr x86_pc, u32 x86_size);
extern void iDumpBlock( int startpc, u8 * ptr );
extern void iIopDumpBlock( int startpc, u8 * ptr );
