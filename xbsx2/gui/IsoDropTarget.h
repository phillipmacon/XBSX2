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
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "AppCommon.h"
#include <wx/dnd.h>

// --------------------------------------------------------------------------------------
//  IsoDropTarget
// --------------------------------------------------------------------------------------
class IsoDropTarget : public wxFileDropTarget
{
protected:
	wxWindow* m_WindowBound;

public:
	virtual ~IsoDropTarget() = default;
	IsoDropTarget( wxWindow* parent ) : wxFileDropTarget()
	{
		m_WindowBound = parent;
	}

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};


extern wxString GetMsg_ConfirmSysReset();
extern wxWindowID SwapOrReset_Iso( wxWindow* owner, IScopedCoreThread& core_control, const wxString& isoFilename, const wxString& descpart1 );
