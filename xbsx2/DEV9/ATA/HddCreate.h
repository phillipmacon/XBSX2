/*  XBSX2 - PS2 Emulator for Xbox Consoles  
 *  Copyright (C) 2002-2020  PCSX2 Dev Team
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

#include <atomic>
#include <chrono>
#include <string>

#include "common/Path.h"

class HddCreate
{
public:
	std::string filePath;
	u64 neededSize;

	std::atomic_bool errored{false};

private:
	std::atomic_bool canceled{false};

	std::chrono::steady_clock::time_point lastUpdate;

public:
	HddCreate(){};

	void Start();

	virtual ~HddCreate(){};

protected:
	virtual void Init(){};
	virtual void Cleanup(){};
	virtual void SetFileProgress(u64 currentSize);
	virtual void SetError();
	void SetCanceled();

private:
	void WriteImage(std::string hddPath, u64 reqSizeBytes);
};
