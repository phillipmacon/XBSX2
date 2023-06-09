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

// Implementations found here: TEST + BTS/BT/BTC/BTR + BSF/BSR! (for lack of better location)

namespace x86Emitter
{

	// --------------------------------------------------------------------------------------
	//  xImpl_Test
	// --------------------------------------------------------------------------------------
	//
	struct xImpl_Test
	{
		void operator()(const xRegisterInt& to, const xRegisterInt& from) const;
		void operator()(const xIndirect64orLess& dest, int imm) const;
		void operator()(const xRegisterInt& to, int imm) const;
	};

	enum G8Type
	{
		G8Type_BT = 4,
		G8Type_BTS,
		G8Type_BTR,
		G8Type_BTC,
	};

	// --------------------------------------------------------------------------------------
	//  BSF / BSR
	// --------------------------------------------------------------------------------------
	// 16/32 operands are available.  No 8 bit ones, not that any of you cared, I bet.
	//
	struct xImpl_BitScan
	{
		// 0xbc [fwd] / 0xbd [rev]
		u16 Opcode;

		void operator()(const xRegister16or32or64& to, const xRegister16or32or64& from) const;
		void operator()(const xRegister16or32or64& to, const xIndirectVoid& sibsrc) const;
	};

	// --------------------------------------------------------------------------------------
	//  xImpl_Group8
	// --------------------------------------------------------------------------------------
	// Bit Test Instructions - Valid on 16/32 bit instructions only.
	//
	struct xImpl_Group8
	{
		G8Type InstType;

		void operator()(const xRegister16or32or64& bitbase, const xRegister16or32or64& bitoffset) const;
		void operator()(const xRegister16or32or64& bitbase, u8 bitoffset) const;

		void operator()(const xIndirectVoid& bitbase, const xRegister16or32or64& bitoffset) const;

		void operator()(const xIndirect64& bitbase, u8 bitoffset) const;
		void operator()(const xIndirect32& bitbase, u8 bitoffset) const;
		void operator()(const xIndirect16& bitbase, u8 bitoffset) const;
	};

} // End namespace x86Emitter
