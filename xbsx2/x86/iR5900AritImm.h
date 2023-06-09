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

/*********************************************************
* Arithmetic with immediate operand                      *
* Format:  OP rt, rs, immediate                          *
*********************************************************/
namespace R5900 {
namespace Dynarec {
namespace OpcodeImpl {

	void recADDI();
	void recADDIU();
	void recDADDI();
	void recDADDIU();
	void recANDI();
	void recORI();
	void recXORI();

	void recSLTI();
	void recSLTIU();

} // namespace OpcodeImpl
} // namespace Dynarec
} // namespace R5900
