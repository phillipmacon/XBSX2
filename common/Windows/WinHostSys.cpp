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
 *  You should have received a copy of the GNU General Public License along with XBSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(_WIN32)

#include "common/RedtapeWindows.h"
#include "common/PageFaultSource.h"
#include "common/Console.h"
#include "common/Exceptions.h"
#include "common/StringUtil.h"
#include "common/AlignedMalloc.h"

static long DoSysPageFaultExceptionFilter(const EXCEPTION_RECORD* er)
{
	if (er->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	// Note: This exception can be accessed by the EE or MTVU thread
	// Source_PageFault is a global variable with its own state information
	// so for now we lock this exception code unless someone can fix this better...
	std::unique_lock lock(PageFault_Mutex);
	Source_PageFault->Dispatch(PageFaultInfo((uptr)er->ExceptionInformation[1]));
	return Source_PageFault->WasHandled() ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}

long __stdcall SysPageFaultExceptionFilter(EXCEPTION_POINTERS* eps)
{
	// Prevent recursive exception filtering by catching the exception from the filter here.
	// In the event that the filter causes an access violation (happened during shutdown
	// because Source_PageFault was deallocated), this will allow the debugger to catch the
	// exception.
	// TODO: find a reliable way to debug the filter itself, I've come up with a few ways that
	// work but I don't fully understand why some do and some don't.
	__try
	{
		return DoSysPageFaultExceptionFilter(eps->ExceptionRecord);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

void _platform_InstallSignalHandler()
{
#ifndef _UWP
	AddVectoredExceptionHandler(true, SysPageFaultExceptionFilter);
#endif
}

static DWORD ConvertToWinApi(const PageProtectionMode& mode)
{
	DWORD winmode = PAGE_NOACCESS;

	// Windows has some really bizarre memory protection enumeration that uses bitwise
	// numbering (like flags) but is in fact not a flag value.  *Someone* from the early
	// microsoft days wasn't a very good coder, me thinks.  --air

	if (mode.CanExecute())
	{
		winmode = mode.CanWrite() ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
	}
	else if (mode.CanRead())
	{
		winmode = mode.CanWrite() ? PAGE_READWRITE : PAGE_READONLY;
	}

	return winmode;
}

void* HostSys::MmapReservePtr(void* base, size_t size)
{
#ifndef _UWP
	return VirtualAlloc(base, size, MEM_RESERVE, PAGE_NOACCESS);
#else
	return VirtualAllocFromApp(base, size, MEM_RESERVE, PAGE_NOACCESS);
#endif
}

bool HostSys::MmapCommitPtr(void* base, size_t size, const PageProtectionMode& mode)
{
#ifndef _UWP
	void* result = VirtualAlloc(base, size, MEM_COMMIT, ConvertToWinApi(mode));
#else
	// UWP requires allocating as RW and then reprotecting to RWX (can't allocate RWX directly).
	void* result = VirtualAllocFromApp(base, size, MEM_COMMIT, ConvertToWinApi(PageProtectionMode(mode).Execute(false)));
	if (result && mode.CanExecute())
	{
		ULONG old_protection;
		if (!VirtualProtectFromApp(base, size, PAGE_EXECUTE_READWRITE, &old_protection))
		{
			VirtualFree(base, size, MEM_RELEASE);
			result = nullptr;
		}
	}
#endif
	if (result)
		return true;

	const DWORD errcode = GetLastError();
	if (errcode == ERROR_COMMITMENT_MINIMUM)
	{
		Console.Warning("(MmapCommit) Received windows error %u {Virtual Memory Minimum Too Low}.", ERROR_COMMITMENT_MINIMUM);
		Sleep(1000); // Cut windows some time to rework its memory...
	}
	else if (errcode != ERROR_NOT_ENOUGH_MEMORY && errcode != ERROR_OUTOFMEMORY)
	{
		pxFailDev(("VirtualAlloc COMMIT failed: " + Exception::WinApiError().GetMsgFromWindows()).c_str());
		return false;
	}

	return false;
}

void HostSys::MmapResetPtr(void* base, size_t size)
{
	VirtualFree(base, size, MEM_DECOMMIT);
}


void* HostSys::MmapReserve(uptr base, size_t size)
{
	return MmapReservePtr((void*)base, size);
}

bool HostSys::MmapCommit(uptr base, size_t size, const PageProtectionMode& mode)
{
	return MmapCommitPtr((void*)base, size, mode);
}

void HostSys::MmapReset(uptr base, size_t size)
{
	MmapResetPtr((void*)base, size);
}


void* HostSys::Mmap(uptr base, size_t size)
{
#ifndef _UWP
	return VirtualAlloc((void*)base, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	// UWP requires allocating as RW and then reprotecting to RWX (can't allocate RWX directly).
	void* result = VirtualAllocFromApp((void*)base, size, MEM_COMMIT, PAGE_READWRITE);
	if (result)
	{
		ULONG old_protection;
		if (!VirtualProtectFromApp(result, size, PAGE_EXECUTE_READWRITE, &old_protection))
		{
			VirtualFree(result, size, MEM_RELEASE);
			result = nullptr;
		}
	}
	return result;
#endif
}

void HostSys::Munmap(uptr base, size_t size)
{
	if (!base)
		return;
	//VirtualFree((void*)base, size, MEM_DECOMMIT);
	VirtualFree((void*)base, 0, MEM_RELEASE);
}

void HostSys::MemProtect(void* baseaddr, size_t size, const PageProtectionMode& mode)
{
	pxAssert((size & (__pagesize - 1)) == 0);

	DWORD OldProtect; // enjoy my uselessness, yo!
	if (!VirtualProtect(baseaddr, size, ConvertToWinApi(mode), &OldProtect))
	{
		Exception::WinApiError apiError;

		apiError.SetDiagMsg(
			StringUtil::StdStringFromFormat("VirtualProtect failed @ 0x%08X -> 0x%08X  (mode=%s)",
				baseaddr, (uptr)baseaddr + size, mode.ToString().c_str()));

		pxFailDev(apiError.FormatDiagnosticMessage().c_str());
	}
}

#ifdef _UWP

// https://docs.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-160
struct UNWIND_INFO
{
	BYTE version : 3;
	BYTE flags : 5;
	BYTE size_of_prologue;
	BYTE count_of_unwind_codes;
	BYTE frame_register : 4;
	BYTE frame_offset_scaled : 4;
	ULONG exception_handler_address;
};

struct UnwindHandler
{
	RUNTIME_FUNCTION runtime_function;
	UNWIND_INFO unwind_info;
	uint8_t exception_handler_code[32];
	DWORD64 code_base;
	DWORD64 code_end;
	UnwindHandler* next_unwind_handler;
};

static_assert(sizeof(UnwindHandler) <= UWP_JIT_EXCEPTION_HANDLER_SIZE);

static UnwindHandler* s_handler_head = nullptr;
static UnwindHandler* s_handler_tail = nullptr;

static EXCEPTION_DISPOSITION UnwindExceptionHandler(PEXCEPTION_RECORD ExceptionRecord, ULONG64 EstablisherFrame,
	PCONTEXT ContextRecord, PDISPATCHER_CONTEXT DispatcherContext)
{
	__try
	{
		const long result = DoSysPageFaultExceptionFilter(ExceptionRecord);
		return (result == EXCEPTION_CONTINUE_EXECUTION) ? ExceptionContinueExecution : ExceptionContinueSearch;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return ExceptionContinueSearch;
	}
}

static PRUNTIME_FUNCTION GetRuntimeFunctionCallback(DWORD64 ControlPc, PVOID Context)
{
	for (UnwindHandler* handler = s_handler_head; handler; handler = handler->next_unwind_handler)
	{
		if (ControlPc >= handler->code_base && ControlPc < handler->code_end)
			return &handler->runtime_function;
	}

	return nullptr;
}

bool UWPInstallExceptionHandlerForJIT(void* start_pc, size_t code_size, void* unwind_handler)
{
	if (!RtlInstallFunctionTableCallback(reinterpret_cast<DWORD64>(UWPInstallExceptionHandlerForJIT) | 0x3,
		reinterpret_cast<DWORD64>(start_pc), static_cast<DWORD>(code_size),
		&GetRuntimeFunctionCallback, nullptr, nullptr))
	{
		Console.WriteLn("RtlInstallFunctionTableCallback() failed: %08X", GetLastError());
		return false;
	}

	UnwindHandler* uh = reinterpret_cast<UnwindHandler*>(unwind_handler);

	// unwind handler must be after code
	const size_t unwind_handler_offset = static_cast<size_t>(static_cast<u8*>(unwind_handler) - static_cast<u8*>(start_pc));
	pxAssertRel(static_cast<u8*>(unwind_handler) >= (static_cast<u8*>(start_pc) + code_size), "Unwind handler is not after code");
	pxAssertRel(code_size <= std::numeric_limits<DWORD>::max(), "Code size is <4GB");
	pxAssertRel(unwind_handler_offset <= std::numeric_limits<DWORD>::max(), "Unwind handler is <4GB away from code");

	// protect it to RW
	ULONG old_protection;
	if (!VirtualProtectFromApp(uh, UWP_JIT_EXCEPTION_HANDLER_SIZE, PAGE_READWRITE, &old_protection))
	{
		Console.Error("VirtualProtectFromApp(RW) for exception handler failed: %08X", GetLastError());
		return false;
	}

	// add it to the handler chain
	if (!s_handler_tail)
	{
		s_handler_head = uh;
		s_handler_tail = uh;
	}
	else
	{
		ULONG old_protection;
		if (!VirtualProtectFromApp(s_handler_tail, UWP_JIT_EXCEPTION_HANDLER_SIZE, PAGE_READWRITE, &old_protection))
		{
			Console.Error("VirtualProtectFromApp(RW) for previous handler failed: %08X", GetLastError());
			return false;
		}

		s_handler_tail->next_unwind_handler = uh;
		if (!VirtualProtectFromApp(s_handler_tail, UWP_JIT_EXCEPTION_HANDLER_SIZE, PAGE_EXECUTE_READ, &old_protection))
		{
			Console.Error("VirtualProtectFromApp(RX) for previous handler failed: %08X", GetLastError());
			return false;
		}

		s_handler_tail = uh;
	}

	// This is only valid on x86 for now.
	uh->code_base = reinterpret_cast<DWORD64>(start_pc);
	uh->code_end = reinterpret_cast<DWORD64>(start_pc) + code_size;
	uh->next_unwind_handler = nullptr;

	uh->runtime_function.BeginAddress = 0;
	uh->runtime_function.EndAddress = static_cast<DWORD>(code_size);
	uh->runtime_function.UnwindInfoAddress = static_cast<DWORD>(unwind_handler_offset + offsetof(UnwindHandler, unwind_info));

	uh->unwind_info.version = 1;
	uh->unwind_info.flags = UNW_FLAG_EHANDLER;
	uh->unwind_info.size_of_prologue = 0;
	uh->unwind_info.count_of_unwind_codes = 0;
	uh->unwind_info.frame_register = 0;
	uh->unwind_info.frame_offset_scaled = 0;
	uh->unwind_info.exception_handler_address = static_cast<DWORD>(unwind_handler_offset + offsetof(UnwindHandler, exception_handler_code));

#ifndef _M_AMD64
	Console.Error("Exception unwind codegen not implemented");
	return false;
#else
	// mov rax, handler
	const void* handler = UnwindExceptionHandler;
	uh->exception_handler_code[0] = 0x48;
	uh->exception_handler_code[1] = 0xb8;
	std::memcpy(&uh->exception_handler_code[2], &handler, sizeof(handler));

	// jmp rax
	uh->exception_handler_code[10] = 0xff;
	uh->exception_handler_code[11] = 0xe0;
#endif

	if (!VirtualProtectFromApp(uh, UWP_JIT_EXCEPTION_HANDLER_SIZE, PAGE_EXECUTE_READ, &old_protection))
	{
		Console.Error("VirtualProtectFromApp(RX) for exception handler failed: %08X", GetLastError());
		return false;
	}

	return true;
}

#endif

#endif
