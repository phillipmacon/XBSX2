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
#include <string>

// Factory - creates an AsyncFileReader derived instance which can read a compressed file
class CompressedFileReader
{
public:
	// fileName and its contents may be used to choose the compressed reader.
	// If no matching handler is found, NULL is returned.
	// The returned instance still needs ->Open(filename) before usage.
	// Open(filename) may still fail.
	static AsyncFileReader* GetNewReader(const std::string& fileName);

private:
	virtual ~CompressedFileReader() = 0;
};
