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

#include "DEV9/net.h"
#include "Payload.h"

namespace PacketReader
{
	enum struct EtherType : u16
	{
		null = 0x0000,
		IPv4 = 0x0800,
		ARP = 0x0806,
		//Tags extend the ether header length
		VlanQTag = 0x8100,
		VlanServiceQTag = 0x88A8,
		VlanDoubleQTag = 0x9100
	};

	class EthernetFrame
	{
	public:
		u8 destinationMAC[6] = {0};
		u8 sourceMAC[6] = {0};

		u16 protocol = 0;
		int headerLength = 14;
		//Length
	private:
		std::unique_ptr<Payload> payload;

	public:
		//Takes ownership of payload
		EthernetFrame(Payload* data);
		EthernetFrame(NetPacket* pkt);

		Payload* GetPayload();

		void WritePacket(NetPacket* pkt);
	};
} // namespace PacketReader
