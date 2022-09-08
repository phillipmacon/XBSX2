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

#include "common/AlignedMalloc.h"

template <int i>
class GSAlignedClass
{
protected:
	GSAlignedClass() = default;
	~GSAlignedClass() = default;

public:
	void* operator new(size_t size)
	{
		return _aligned_malloc(size, i);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void* operator new(size_t size, void* ptr)
	{
		return ptr;
	}

	void operator delete(void* ptr, void* placement_ptr)
	{
		// Just here to satisfy compilers
		// Person who calls in-place placement new must handle error case
	}

	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, i);
	}

	void operator delete[](void* p)
	{
		_aligned_free(p);
	}
};
