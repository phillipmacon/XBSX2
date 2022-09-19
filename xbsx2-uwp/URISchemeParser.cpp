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
#include "URISchemeParser.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <sstream>
#include <iomanip>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::System;

bool URISchemeParser::IsInitialized()
{
	return m_initialized;
}

void URISchemeParser::ParseProtocolArgs(const IActivatedEventArgs& args)
{
	m_argc = NULL;
	m_pArgv.clear();
	m_argv.clear();

	if (args.Kind() == ActivationKind::Protocol)
	{
		ProtocolActivatedEventArgs protocolArgs{args.as<ProtocolActivatedEventArgs>()};
		WwwFormUrlDecoder query = protocolArgs.Uri().QueryParsed();

		for (uint i = 0; i < query.Size(); i++)
		{
			IWwwFormUrlDecoderEntry arg = query.GetAt(i);

			//parse command line string
			if (arg.Name() == winrt::hstring(L"cmd"))
			{
				std::istringstream iss(winrt::to_string(arg.Value()));
				std::string s;

				//set escape character to null char to preserve backslashes in paths which are inside quotes, they get stripped by default
				while (iss >> std::quoted(s, '"', (char)0))
				{
					m_argv.push_back(s);
				}
			}
			else if (arg.Name() == winrt::hstring(L"launchOnExit"))
			{
				//if this UWP app is started using protocol with argument "launchOnExit", this gives an option to launch another app on exit,
				//making it easy to integrate this app with other UWP frontends
				m_launchOnExit = arg.Value();
			}
		}
	}

	m_argc = m_argv.size();
	//convert to char* array compatible with argv
	for (int i = 0; i < m_argv.size(); i++)
	{
		m_pArgv.push_back((char*)(m_argv.at(i)).c_str());
	}
	m_pArgv.push_back(nullptr);
	m_initialized = true;
}

int URISchemeParser::GetArgc()
{
	return m_argc;
}

char** URISchemeParser::GetArgv()
{	
	return m_pArgv.data();
}

winrt::hstring URISchemeParser::GetLaunchOnExitURI()
{
	return m_launchOnExit;
}

