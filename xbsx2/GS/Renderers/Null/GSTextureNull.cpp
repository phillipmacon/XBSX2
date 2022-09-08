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

#include "PrecompiledHeader.h"
#include "GSTextureNull.h"

GSTextureNull::GSTextureNull()
{
	memset(&m_desc, 0, sizeof(m_desc));
}

GSTextureNull::GSTextureNull(Type type, int w, int h, GSTexture::Format format)
{
	m_desc.type = type;
	m_desc.w = w;
	m_desc.h = h;
	m_desc.format = format;
}

void GSTextureNull::Swap(GSTexture* tex)
{
	GSTexture::Swap(tex);
	std::swap(m_desc, static_cast<GSTextureNull*>(tex)->m_desc);
}

void* GSTextureNull::GetNativeHandle() const
{
	return nullptr;
}
