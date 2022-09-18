/*  XBSX2 - PS2 Emulator for Xbox Consoles
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
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

#include "MemoryTypes.h"

extern uptr *psxMemWLUT;
extern const uptr *psxMemRLUT;


// Obtains a writable pointer into the IOP's memory, with TLB address translation.
// If the address maps to read-only memory, NULL is returned.
// Hacky!  This should really never be used, ever, since it bypasses the iop's Hardware
// Register handler and SPU/DEV/USB maps.
template<typename T>
static __fi T* iopVirtMemW( u32 mem )
{
	return (psxMemWLUT[(mem) >> 16] == 0) ? NULL : (T*)(psxMemWLUT[(mem) >> 16] + ((mem) & 0xffff));
}

// Obtains a read-safe pointer into the IOP's physical memory, with TLB address translation.
// Returns NULL if the address maps to an invalid/unmapped physical address.
//
// Hacky!  This should really never be used, since anything reading through the
// TLB should be using iopMemRead/Write instead for each individual access.  That ensures
// correct handling of page boundary crossings.
template<typename T>
static __fi const T* iopVirtMemR( u32 mem )
{
	mem &= 0x1fffffff;
	return (psxMemRLUT[mem >> 16] == 0) ? NULL : (const T*)(psxMemRLUT[mem >> 16] + (mem & 0xffff));
}

// Obtains a pointer to the IOP's physical mapping (bypasses the TLB)
static __fi u8* iopPhysMem( u32 addr )
{
	return &iopMem->Main[addr & 0x1fffff];
}

#define psxSs8(mem)		iopMem->Sif[(mem) & 0x00ff]
#define psxSs16(mem)	(*(s16*)&iopMem->Sif[(mem) & 0x00ff])
#define psxSs32(mem)	(*(s32*)&iopMem->Sif[(mem) & 0x00ff])
#define psxSu8(mem)		(*(u8*) &iopMem->Sif[(mem) & 0x00ff])
#define psxSu16(mem)	(*(u16*)&iopMem->Sif[(mem) & 0x00ff])
#define psxSu32(mem)	(*(u32*)&iopMem->Sif[(mem) & 0x00ff])

#define psxPs8(mem)		iopMem->P[(mem) & 0xffff]
#define psxPs16(mem)	(*(s16*)&iopMem->P[(mem) & 0xffff])
#define psxPs32(mem)	(*(s32*)&iopMem->P[(mem) & 0xffff])
#define psxPu8(mem)		(*(u8*) &iopMem->P[(mem) & 0xffff])
#define psxPu16(mem)	(*(u16*)&iopMem->P[(mem) & 0xffff])
#define psxPu32(mem)	(*(u32*)&iopMem->P[(mem) & 0xffff])

#define psxHs8(mem)		iopHw[(mem) & 0xffff]
#define psxHs16(mem)	(*(s16*)&iopHw[(mem) & 0xffff])
#define psxHs32(mem)	(*(s32*)&iopHw[(mem) & 0xffff])
#define psxHu8(mem)		(*(u8*) &iopHw[(mem) & 0xffff])
#define psxHu16(mem)	(*(u16*)&iopHw[(mem) & 0xffff])
#define psxHu32(mem)	(*(u32*)&iopHw[(mem) & 0xffff])

extern void psxMemReserve();
extern void psxMemAlloc();
extern void psxMemReset();
extern void psxMemShutdown();

extern u8   iopMemRead8 (u32 mem);
extern u16  iopMemRead16(u32 mem);
extern u32  iopMemRead32(u32 mem);
extern void iopMemWrite8 (u32 mem, u8 value);
extern void iopMemWrite16(u32 mem, u16 value);
extern void iopMemWrite32(u32 mem, u32 value);

std::string iopMemReadString(u32 mem, int maxlen = 65536);

namespace IopMemory
{
	// Sif functions not made yet (will for future Iop improvements):
	extern mem8_t SifRead8( u32 iopaddr );
	extern mem16_t SifRead16( u32 iopaddr );
	extern mem32_t SifRead32( u32 iopaddr );

	extern void SifWrite8( u32 iopaddr, mem8_t data );
	extern void SifWrite16( u32 iopaddr, mem16_t data );
	extern void SifWrite32( u32 iopaddr, mem32_t data );

	extern mem8_t iopHwRead8_generic( u32 addr );
	extern mem16_t iopHwRead16_generic( u32 addr );
	extern mem32_t iopHwRead32_generic( u32 addr );
	extern void iopHwWrite8_generic( u32 addr, mem8_t val );
	extern void iopHwWrite16_generic( u32 addr, mem16_t val );
	extern void iopHwWrite32_generic( u32 addr, mem32_t val );


	extern mem8_t iopHwRead8_Page1( u32 iopaddr );
	extern mem8_t iopHwRead8_Page3( u32 iopaddr );
	extern mem8_t iopHwRead8_Page8( u32 iopaddr );
	extern mem16_t iopHwRead16_Page1( u32 iopaddr );
	extern mem16_t iopHwRead16_Page3( u32 iopaddr );
	extern mem16_t iopHwRead16_Page8( u32 iopaddr );
	extern mem32_t iopHwRead32_Page1( u32 iopaddr );
	extern mem32_t iopHwRead32_Page3( u32 iopaddr );
	extern mem32_t iopHwRead32_Page8( u32 iopaddr );

	extern void iopHwWrite8_Page1( u32 iopaddr, mem8_t data );
	extern void iopHwWrite8_Page3( u32 iopaddr, mem8_t data );
	extern void iopHwWrite8_Page8( u32 iopaddr, mem8_t data );
	extern void iopHwWrite16_Page1( u32 iopaddr, mem16_t data );
	extern void iopHwWrite16_Page3( u32 iopaddr, mem16_t data );
	extern void iopHwWrite16_Page8( u32 iopaddr, mem16_t data );
	extern void iopHwWrite32_Page1( u32 iopaddr, mem32_t data );
	extern void iopHwWrite32_Page3( u32 iopaddr, mem32_t data );
	extern void iopHwWrite32_Page8( u32 iopaddr, mem32_t data );
}
