/*  XBSX2 - PS2 Emulator for Xbox Consoles
 *  Copyright (C) 2002-2021 PCSX2 Dev Team
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
#include "GSTexture.h"
#include "GSDevice.h"
#include "GS/GSPng.h"
#include <bitset>

GSTexture::GSTexture()
	: m_scale(1, 1)
	, m_size(0, 0)
	, m_mipmap_levels(0)
	, m_type(Type::Invalid)
	, m_format(Format::Invalid)
	, m_state(State::Dirty)
	, m_needs_mipmaps_generated(true)
	, last_frame_used(0)
	, OffsetHack_modxy(0.0f)
{
}

bool GSTexture::Save(const std::string& fn)
{
#ifdef XBSX2_DEVBUILD
	GSPng::Format format = GSPng::RGB_A_PNG;
#else
	GSPng::Format format = GSPng::RGB_PNG;
#endif
	switch (m_format)
	{
		case Format::UNorm8:
			format = GSPng::R8I_PNG;
			break;
		case Format::Color:
			break;
		default:
			Console.Error("Format %d not saved to image", static_cast<int>(m_format));
			return false;
	}

	GSMap map;
	if (!g_gs_device->DownloadTexture(this, GSVector4i(0, 0, m_size.x, m_size.y), map))
	{
		Console.Error("(GSTexture) DownloadTexture() failed.");
		return false;
	}

	const int compression = theApp.GetConfigI("png_compression_level");
	bool success = GSPng::Save(format, fn, map.bits, m_size.x, m_size.y, map.pitch, compression);

	g_gs_device->DownloadTextureComplete();

	return success;
}

void GSTexture::Swap(GSTexture* tex)
{
	std::swap(m_scale, tex->m_scale);
	std::swap(m_size, tex->m_size);
	std::swap(m_mipmap_levels, tex->m_mipmap_levels);
	std::swap(m_type, tex->m_type);
	std::swap(m_format, tex->m_format);
	std::swap(m_state, tex->m_state);
	std::swap(m_needs_mipmaps_generated, tex->m_needs_mipmaps_generated);
	std::swap(last_frame_used, tex->last_frame_used);
	std::swap(OffsetHack_modxy, tex->OffsetHack_modxy);
}

u32 GSTexture::GetCompressedBytesPerBlock() const
{
	static constexpr u32 bytes_per_block[] = {
		1, // Invalid
		4, // Color/RGBA8
		16, // FloatColor/RGBA32F
		32, // DepthStencil
		1, // UNorm8/R8
		2, // UInt16/R16UI
		4, // UInt32/R32UI
		4, // Int32/R32I
		8, // BC1 - 16 pixels in 64 bits
		16, // BC2 - 16 pixels in 128 bits
		16, // BC3 - 16 pixels in 128 bits
		16, // BC4 - 16 pixels in 128 bits
	};

	return bytes_per_block[static_cast<u32>(m_format)];
}

u32 GSTexture::GetCompressedBlockSize() const
{
	if (m_format >= Format::BC1 && m_format <= Format::BC7)
		return 4;
	else
		return 1;
}

u32 GSTexture::CalcUploadRowLengthFromPitch(u32 pitch) const
{
	const u32 block_size = GetCompressedBlockSize();
	const u32 bytes_per_block = GetCompressedBytesPerBlock();
	return ((pitch + (bytes_per_block - 1)) / bytes_per_block) * block_size;
}

u32 GSTexture::CalcUploadSize(u32 height, u32 pitch) const
{
	const u32 block_size = GetCompressedBlockSize();
	return pitch * ((static_cast<u32>(height) + (block_size - 1)) / block_size);
}

void GSTexture::GenerateMipmapsIfNeeded()
{
	if (!m_needs_mipmaps_generated || m_mipmap_levels <= 1 || IsCompressedFormat())
		return;

	m_needs_mipmaps_generated = false;
	GenerateMipmap();
}
