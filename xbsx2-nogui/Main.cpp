/*  XBSX2 - PS2 Emulator for Xbox Consoles
 *  Copyright (C) 2002-2022  PCSX2 Dev Team
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

#include <cstdlib>
#include <csignal>

#include "NoGUIHost.h"
#include "NoGUIPlatform.h"

#include "CDVD/CDVD.h"
#include "Frontend/GameList.h"
#include "Frontend/LogSink.h"

#include "common/CrashHandler.h"
#include "common/FileSystem.h"
#include "common/StringUtil.h"

static void PrintCommandLineVersion()
{
	Host::InitializeEarlyConsole();
	std::fprintf(stderr, "%s\n", (NoGUIHost::GetAppNameAndVersion() + NoGUIHost::GetAppConfigSuffix()).c_str());
	std::fprintf(stderr, "https://pcsx2.net/\n");
	std::fprintf(stderr, "\n");
}

static void PrintCommandLineHelp(const char* progname)
{
	PrintCommandLineVersion();
	std::fprintf(stderr, "Usage: %s [parameters] [--] [boot filename]\n", progname);
	std::fprintf(stderr, "\n");
	std::fprintf(stderr, "  -help: Displays this information and exits.\n");
	std::fprintf(stderr, "  -version: Displays version information and exits.\n");
	std::fprintf(stderr, "  -batch: Enables batch mode (exits after shutting down).\n");
	std::fprintf(stderr, "  -elf <file>: Overrides the boot ELF with the specified filename.\n");
	std::fprintf(stderr, "  -disc <path>: Uses the specified host DVD drive as a source.\n");
	std::fprintf(stderr, "  -bios: Starts the BIOS (System Menu/OSDSYS).\n");
	std::fprintf(stderr, "  -fastboot: Force fast boot for provided filename.\n");
	std::fprintf(stderr, "  -slowboot: Force slow boot for provided filename.\n");
	std::fprintf(stderr, "  -state <index>: Loads specified save state by index.\n");
	std::fprintf(stderr, "  -statefile <filename>: Loads state from the specified filename.\n");
	std::fprintf(stderr, "  -fullscreen: Enters fullscreen mode immediately after starting.\n");
	std::fprintf(stderr, "  -nofullscreen: Prevents fullscreen mode from triggering if enabled.\n");
	std::fprintf(stderr, "  --: Signals that no more arguments will follow and the remaining\n"
						 "    parameters make up the filename. Use when the filename contains\n"
						 "    spaces or starts with a dash.\n");
	std::fprintf(stderr, "\n");
}

static std::shared_ptr<VMBootParameters>& AutoBoot(std::shared_ptr<VMBootParameters>& autoboot)
{
	if (!autoboot)
		autoboot = std::make_shared<VMBootParameters>();

	return autoboot;
}

static bool ParseCommandLineOptions(int argc, char* argv[], std::shared_ptr<VMBootParameters>& autoboot)
{
	bool no_more_args = false;

	for (int i = 1; i < argc; i++)
	{
		if (!no_more_args)
		{
#define CHECK_ARG(str) !std::strcmp(argv[i], str)
#define CHECK_ARG_PARAM(str) (!std::strcmp(argv[i], str) && ((i + 1) < argc))

			if (CHECK_ARG("-help"))
			{
				PrintCommandLineHelp(argv[0]);
				return false;
			}
			else if (CHECK_ARG("-version"))
			{
				PrintCommandLineVersion();
				return false;
			}
			else if (CHECK_ARG("-batch"))
			{
				NoGUIHost::SetBatchMode(true);
				continue;
			}
			else if (CHECK_ARG("-fastboot"))
			{
				AutoBoot(autoboot)->fast_boot = true;
				continue;
			}
			else if (CHECK_ARG("-slowboot"))
			{
				AutoBoot(autoboot)->fast_boot = false;
				continue;
			}
			else if (CHECK_ARG_PARAM("-state"))
			{
				AutoBoot(autoboot)->state_index = std::atoi(argv[++i]);
				continue;
			}
			else if (CHECK_ARG_PARAM("-statefile"))
			{
				AutoBoot(autoboot)->save_state = argv[++i];
				continue;
			}
			else if (CHECK_ARG_PARAM("-elf"))
			{
				AutoBoot(autoboot)->elf_override = argv[++i];
				continue;
			}
			else if (CHECK_ARG_PARAM("-disc"))
			{
				AutoBoot(autoboot)->source_type = CDVD_SourceType::Disc;
				AutoBoot(autoboot)->filename = argv[++i];
				continue;
			}
			else if (CHECK_ARG("-bios"))
			{
				AutoBoot(autoboot)->source_type = CDVD_SourceType::NoDisc;
				continue;
			}
			else if (CHECK_ARG("-fullscreen"))
			{
				AutoBoot(autoboot)->fullscreen = true;
				continue;
			}
			else if (CHECK_ARG("-nofullscreen"))
			{
				AutoBoot(autoboot)->fullscreen = false;
				continue;
			}
			else if (CHECK_ARG("--"))
			{
				no_more_args = true;
				continue;
			}
			else if (argv[i][0] == '-')
			{
				Host::InitializeEarlyConsole();
				std::fprintf(stderr, "Unknown parameter: '%s'", argv[i]);
				return false;
			}

#undef CHECK_ARG
#undef CHECK_ARG_PARAM
		}

		if (!AutoBoot(autoboot)->filename.empty())
			AutoBoot(autoboot)->filename += ' ';

		AutoBoot(autoboot)->filename += argv[i];
	}

	// check autoboot parameters, if we set something like fullscreen without a bios
	// or disc, we don't want to actually start.
	if (autoboot && !autoboot->source_type.has_value() && autoboot->filename.empty() && autoboot->elf_override.empty())
	{
		Host::InitializeEarlyConsole();
		Console.Warning("Skipping autoboot due to no boot parameters.");
		autoboot.reset();
	}

	// if we don't have autoboot, we definitely don't want batch mode (because that'll skip
	// scanning the game list).
	if (NoGUIHost::InBatchMode() && !autoboot)
	{
		Host::InitializeEarlyConsole();
		Console.Warning("Disabling batch mode, because we have no autoboot.");
		NoGUIHost::SetBatchMode(false);
	}

	return true;
}

int main(int argc, char* argv[])
{
	CrashHandler::Install();

	std::shared_ptr<VMBootParameters> autoboot;
	if (!ParseCommandLineOptions(argc, argv, autoboot))
		return EXIT_FAILURE;

#ifdef _WIN32
	g_nogui_window = NoGUIPlatform::CreateWin32Platform();
#elif defined(NOGUI_PLATFORM_WAYLAND)
	g_nogui_window = NoGUIPlatform::CreateWaylandPlatform();
#endif
	if (!g_nogui_window)
		return EXIT_FAILURE;

	if (!NoGUIHost::Initialize())
	{
		g_nogui_window.reset();
		return EXIT_FAILURE;
	}

	if (autoboot)
		NoGUIHost::StartVM(std::move(autoboot));

	g_nogui_window->RunMessageLoop();

	NoGUIHost::Shutdown();
	g_nogui_window.reset();
	return EXIT_SUCCESS;
}

#ifdef _WIN32

#include "common/RedtapeWindows.h"
#include <shellapi.h>

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	std::vector<std::string> argc_strings;
	argc_strings.reserve(1);

	// CommandLineToArgvW() only adds the program path if the command line is empty?!
	argc_strings.push_back(FileSystem::GetProgramPath());

	if (std::wcslen(lpCmdLine) > 0)
	{
		int argc;
		LPWSTR* argv_wide = CommandLineToArgvW(lpCmdLine, &argc);
		if (argv_wide)
		{
			for (int i = 0; i < argc; i++)
				argc_strings.push_back(StringUtil::WideStringToUTF8String(argv_wide[i]));

			LocalFree(argv_wide);
		}
	}

	std::vector<char*> argc_pointers;
	argc_pointers.reserve(argc_strings.size());
	for (std::string& arg : argc_strings)
		argc_pointers.push_back(arg.data());

	return main(static_cast<int>(argc_pointers.size()), argc_pointers.data());
}

#endif
