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
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"
#include "SPU2/Global.h"

#include <windows.h>
#include <mmsystem.h>


extern "C" {
#include "dsp.h"

typedef winampDSPHeader* (*pWinampDSPGetHeader2)();
}

HMODULE hLib = nullptr;
pWinampDSPGetHeader2 pGetHeader = nullptr;
winampDSPHeader* pHeader = nullptr;

winampDSPModule* pModule = nullptr;

HWND hTemp;

#define USE_A_THREAD
#ifdef USE_A_THREAD

HANDLE hUpdateThread;
DWORD UpdateThreadId;

bool running;

DWORD WINAPI DspUpdateThread(PVOID param);
#endif
s32 DspLoadLibrary(wchar_t* fileName, int modNum)
#ifdef USE_A_THREAD
{
	if (!dspPluginEnabled)
		return -1;

	running = true;
	hUpdateThread = CreateThread(nullptr, 0, DspUpdateThread, nullptr, 0, &UpdateThreadId);
	return (hUpdateThread == INVALID_HANDLE_VALUE);
}

s32 DspLoadLibrary2(wchar_t* fileName, int modNum)
#endif
{
	if (!dspPluginEnabled)
		return -1;

	hLib = LoadLibraryW(fileName);
	if (!hLib)
	{
		return 1;
	}

	pGetHeader = (pWinampDSPGetHeader2)GetProcAddress(hLib, "winampDSPGetHeader2");

	if (!pGetHeader)
	{
		FreeLibrary(hLib);
		hLib = nullptr;
		return 1;
	}

	pHeader = pGetHeader();

	pModule = pHeader->getModule(modNum);

	if (!pModule)
	{
		pGetHeader = nullptr;
		pHeader = nullptr;
		FreeLibrary(hLib);
		hLib = nullptr;
		return -1;
	}

	pModule->hDllInstance = hLib;
	pModule->hwndParent = 0;
	pModule->Init(pModule);

	return 0;
}

void DspCloseLibrary()
#ifdef USE_A_THREAD
{
	if (!dspPluginEnabled)
		return;

	PostThreadMessage(UpdateThreadId, WM_QUIT, 0, 0);
	running = false;
	if (WaitForSingleObject(hUpdateThread, 1000) == WAIT_TIMEOUT)
	{
		ConLog("SPU2: WARNING: DSP Thread did not close itself in time. Assuming hung. Terminating.\n");
		TerminateThread(hUpdateThread, 1);
	}
}

void DspCloseLibrary2()
#endif
{
	if (!dspPluginEnabled)
		return;

	if (hLib)
	{
		pModule->Quit(pModule);
		FreeLibrary(hLib);
	}
	pModule = nullptr;
	pHeader = nullptr;
	pGetHeader = nullptr;
	hLib = nullptr;
}

int DspProcess(s16* buffer, int samples)
{
	if (!dspPluginEnabled)
		return samples;

	if (hLib)
	{
		return pModule->ModifySamples(pModule, buffer, samples, 16, 2, SampleRate);
	}
	return samples;
}

void DspUpdate()
#ifdef USE_A_THREAD
{
}

DWORD WINAPI DspUpdateThread(PVOID param)
{
	if (!dspPluginEnabled)
		return -1;

	if (DspLoadLibrary2(dspPlugin, dspPluginModule))
		return -1;

	MSG msg;
	while (running)
	{
		GetMessage(&msg, 0, 0, 0);
		if ((msg.hwnd == nullptr) && (msg.message == WM_QUIT))
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DspCloseLibrary2();
	return 0;
}

#else
{
	if (!dspPluginEnabled)
		return;

	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#endif
