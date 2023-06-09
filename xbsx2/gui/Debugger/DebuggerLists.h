/*  XBSX2 - PS2 Emulator for Xbox Consoles
 *  Copyright (C) 2002-2014  PCSX2 Dev Team
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
#include <wx/listctrl.h>
#include "DebugTools/DebugInterface.h"
#include "DebugTools/Breakpoints.h"
#include "DebugTools/BiosDebugData.h"
#include "DebugTools/MipsStackWalk.h"
#include "CtrlDisassemblyView.h"

struct GenericListViewColumn
{
	const wchar_t* name;
	float size;
};

class GenericListView : public wxListView
{
public:
	GenericListView(wxWindow* parent, GenericListViewColumn* columns, int columnCount);
	void update();

	wxDECLARE_EVENT_TABLE();

protected:
	void sizeEvent(wxSizeEvent& evt);
	void keydownEvent(wxKeyEvent& evt);
	void postEvent(wxEventType type, int value);
	void mouseEvent(wxMouseEvent& evt);
	void listEvent(wxListEvent& evt);

	[[nodiscard]] virtual wxString getColumnText(int row, int col) const = 0;
	virtual int getRowCount() = 0;
	virtual void onDoubleClick(int itemIndex, const wxPoint& point){};
	virtual void onRightClick(int itemIndex, const wxPoint& point){};
	virtual void onKeyDown(int key){};

	// This flag prevents resizing loop in the resizeColumn method of this class
	// when the Windows Classic theme with some large resolutions around larger
	// than 1024 x 768 have been chosen.
	//
	// The resizing loop will occur by the ListView_SetColumnWidth macro in the
	// Windows SDK called by the wxListCtrl::SetColumnWidth method when the
	// conditions above have been chosen.
	bool m_isInResizeColumn;

private:
	void insertColumns(GenericListViewColumn* columns, int count);
	void resizeColumn(int col, int width);
	void resizeColumns(int totalWidth);
	[[nodiscard]] wxString OnGetItemText(long item, long col) const override;

	GenericListViewColumn* columns;
	wxPoint clickPos;
	bool dontResizeColumnsInSizeEventHandler;
};

class BreakpointList : public GenericListView
{
public:
	BreakpointList(wxWindow* parent, DebugInterface* _cpu, CtrlDisassemblyView* _disassembly);
	void reloadBreakpoints();

protected:
	void onPopupClick(wxCommandEvent& evt);

	[[nodiscard]] wxString getColumnText(int row, int col) const override;
	int getRowCount() override;
	void onDoubleClick(int itemIndex, const wxPoint& point) override;
	void onRightClick(int itemIndex, const wxPoint& point) override;
	void onKeyDown(int key) override;

private:
	int getBreakpointIndex(int itemIndex, bool& isMemory) const;
	int getTotalBreakpointCount();
	void editBreakpoint(int itemIndex);
	void toggleEnabled(int itemIndex);
	void gotoBreakpointAddress(int itemIndex);
	void removeBreakpoint(int itemIndex);
	void showMenu(const wxPoint& pos);

	std::vector<BreakPoint> displayedBreakPoints_;
	std::vector<MemCheck> displayedMemChecks_;
	DebugInterface* cpu;
	CtrlDisassemblyView* disasm;
};

class ThreadList : public GenericListView
{
public:
	ThreadList(wxWindow* parent, DebugInterface* _cpu);
	void reloadThreads();
	EEThread getRunningThread();

protected:
	void onPopupClick(wxCommandEvent& evt);

	[[nodiscard]] wxString getColumnText(int row, int col) const override;
	int getRowCount() override;
	void onDoubleClick(int itemIndex, const wxPoint& point) override;

private:
	DebugInterface* cpu;
	std::vector<EEThread> threads;
};

class StackFramesList : public GenericListView
{
public:
	StackFramesList(wxWindow* parent, DebugInterface* _cpu, CtrlDisassemblyView* _disassembly);
	void loadStackFrames(EEThread& currentThread);

protected:
	void onPopupClick(wxCommandEvent& evt);

	[[nodiscard]] wxString getColumnText(int row, int col) const override;
	int getRowCount() override;
	void onDoubleClick(int itemIndex, const wxPoint& point) override;

private:
	DebugInterface* cpu;
	CtrlDisassemblyView* disassembly;
	std::vector<MipsStackWalk::StackFrame> frames;
};
