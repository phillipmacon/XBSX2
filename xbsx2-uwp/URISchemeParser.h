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
#include <winrt/base.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.h>

/* If the app is activated using protocol, it is expected to be in this format:
* "xbsx2:?cmd=<PCSX2 CLI arguments>&launchOnExit=<app to launch on exit>"
* For example:
* xbsx2:?cmd=pcsx2.exe "c:\mypath\path with spaces\game.iso"&launchOnExit=LaunchApp:
* "cmd" and "launchOnExit" are optional. If none specified, it will normally launch into menu
*/
class URISchemeParser
{
public:
	bool IsInitialized();
	void ParseProtocolArgs(const winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs& args);
	int	GetArgc();
	char** GetArgv();
	winrt::hstring GetLaunchOnExitURI();

private:
	bool m_initialized;
	int m_argc = NULL;
	std::vector<char*> m_pArgv;
	std::vector<std::string> m_argv; //using std::string as temp buf instead of char* array to avoid manual char allocations
	winrt::hstring m_launchOnExit;
};
