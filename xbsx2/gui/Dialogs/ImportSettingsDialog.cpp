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

#include "PrecompiledHeader.h"
#include "System.h"
#include "gui/MSWstuff.h"

#include "ModalPopups.h"

using namespace pxSizerFlags;

Dialogs::ImportSettingsDialog::ImportSettingsDialog( wxWindow* parent )
	: wxDialogWithHelpers( parent, _("Import Existing Settings?") )
{
	SetMinWidth( MSW_GetDPIScale() * 440 );

	pxStaticText& heading( Text( pxsFmt(

		/// (%s is the app name, normally PCSX2 -- omitting one or both %s is allowed)
		pxE( L"Existing %s settings have been found in the configured settings folder.  Would you like to import these settings or overwrite them with %s default values?\n\n(or press Cancel to select a different settings folder)"
		), WX_STR(pxGetAppName()), WX_STR(pxGetAppName())
	)));

	wxBoxSizer& s_buttons = *new wxBoxSizer( wxHORIZONTAL );
	wxButton* b_import	= new wxButton( this, wxID_ANY, _("Import") );
	wxButton* b_over	= new wxButton( this, wxID_ANY, _("Overwrite") );

	// --------------------------------------------------------------------
	// Layout Some Shizat...

	s_buttons += b_import	| StdButton();
	s_buttons += 16;
	s_buttons += b_over		| StdButton();
	s_buttons += 16;
	s_buttons += new wxButton( this, wxID_CANCEL ) | StdButton();

	*this += 4;
	*this += heading		| StdExpand();
	*this += 12;
	*this += &s_buttons		| StdCenter();

	Bind(wxEVT_BUTTON, &ImportSettingsDialog::OnImport_Click, this, b_import->GetId());
	Bind(wxEVT_BUTTON, &ImportSettingsDialog::OnOverwrite_Click, this, b_over->GetId());
}

void Dialogs::ImportSettingsDialog::OnImport_Click( wxCommandEvent& /* evt */ )
{
	AppConfig_OnChangedSettingsFolder( false );		// ... and import existing settings
	g_Conf->Folders.Bios.Mkdir();
	EndModal( wxID_OK );
}

void Dialogs::ImportSettingsDialog::OnOverwrite_Click( wxCommandEvent& /* evt */ )
{
	AppConfig_OnChangedSettingsFolder( true );		// ... and overwrite any existing settings
	g_Conf->Folders.Bios.Mkdir();
	EndModal( wxID_OK );
}
