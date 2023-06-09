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

#include "gui/AppCommon.h"
#include "gui/ApplyState.h"
#include <memory>

namespace Panels
{
	class LogOptionsPanel;

	class BaseCpuLogOptionsPanel : public CheckedStaticBox
	{
	protected:
		wxStaticBoxSizer*	m_miscGroup;

	public:
		BaseCpuLogOptionsPanel(wxWindow* parent, const wxString& title, wxOrientation orient = wxVERTICAL)
			: CheckedStaticBox(parent, orient, title), m_miscGroup(NULL){}

		virtual wxStaticBoxSizer* GetMiscGroup() const { return m_miscGroup; }
		virtual CheckedStaticBox* GetStaticBox( const wxString& subgroup ) const=0;
	};

	class eeLogOptionsPanel : public BaseCpuLogOptionsPanel
	{
	protected:
		CheckedStaticBox*	m_disasmPanel;
		CheckedStaticBox*	m_hwPanel;
		CheckedStaticBox*	m_evtPanel;

	public:
		eeLogOptionsPanel( LogOptionsPanel* parent );
		virtual ~eeLogOptionsPanel() = default;

		CheckedStaticBox* GetStaticBox( const wxString& subgroup ) const;

		void OnSettingsChanged();
		void Apply();
	};


	class iopLogOptionsPanel : public BaseCpuLogOptionsPanel
	{
	protected:
		CheckedStaticBox*	m_disasmPanel;
		CheckedStaticBox*	m_hwPanel;
		CheckedStaticBox*	m_evtPanel;

	public:
		iopLogOptionsPanel( LogOptionsPanel* parent );
		virtual ~iopLogOptionsPanel() = default;

		CheckedStaticBox* GetStaticBox( const wxString& subgroup ) const;

		void OnSettingsChanged();
		void Apply();
	};

	class LogOptionsPanel : public BaseApplicableConfigPanel
	{
	protected:
		eeLogOptionsPanel*	m_eeSection;
		iopLogOptionsPanel*	m_iopSection;
		wxStaticBoxSizer*	m_miscSection;

		pxCheckBox*			m_masterEnabler;

		std::unique_ptr<pxCheckBox*[]> m_checks;

	public:
		LogOptionsPanel( wxWindow* parent );
		virtual ~LogOptionsPanel() = default;

		void AppStatusEvent_OnSettingsApplied();
		void OnUpdateEnableAll();
		void OnCheckBoxClicked(wxCommandEvent &event);
		void Apply();
		
	protected:
		BaseCpuLogOptionsPanel* GetCpuPanel( const wxString& token ) const;
	};
}
