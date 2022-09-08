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

#include "common/Xbsx2Defs.h"
#include <string_view>
#include <vector>

enum IsoFS_Type
{
	FStype_ISO9660 = 1,
	FStype_Joliet = 2,
};

class IsoDirectory
{
public:
	SectorSource& internalReader;
	std::vector<IsoFileDescriptor> files;
	IsoFS_Type m_fstype;

public:
	IsoDirectory(SectorSource& r);
	IsoDirectory(SectorSource& r, const IsoFileDescriptor directoryEntry);
	virtual ~IsoDirectory() = default;

	std::string FStype_ToString() const;
	SectorSource& GetReader() const { return internalReader; }

	bool Exists(const std::string_view& filePath) const;
	bool IsFile(const std::string_view& filePath) const;
	bool IsDir(const std::string_view& filePath) const;

	u32 GetFileSize(const std::string_view& filePath) const;

	IsoFileDescriptor FindFile(const std::string_view& filePath) const;

protected:
	const IsoFileDescriptor& GetEntry(const std::string_view& fileName) const;
	const IsoFileDescriptor& GetEntry(int index) const;

	void Init(const IsoFileDescriptor& directoryEntry);
	int GetIndexOf(const std::string_view& fileName) const;
};
