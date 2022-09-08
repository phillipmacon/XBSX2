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
#include "common/Xbsx2Defs.h"
#include <array>
#include <cstring>
#include <map>
#include <optional>
#include <string_view>

#include <winrt/Windows.System.h>

namespace UWPKeyNames
{
	using namespace winrt::Windows::System;

	static const std::map<VirtualKey, const char*> s_key_names = {
		{VirtualKey::LeftButton, "LeftButton"},
		{VirtualKey::RightButton, "RightButton"},
		{VirtualKey::Cancel, "Cancel"},
		{VirtualKey::MiddleButton, "MiddleButton"},
		{VirtualKey::XButton1, "XButton1"},
		{VirtualKey::XButton2, "XButton2"},
		{VirtualKey::Back, "Back"},
		{VirtualKey::Tab, "Tab"},
		{VirtualKey::Clear, "Clear"},
		{VirtualKey::Enter, "Return"},
		{VirtualKey::Shift, "Shift"},
		{VirtualKey::Control, "Control"},
		{VirtualKey::Menu, "Menu"},
		{VirtualKey::Pause, "Pause"},
		{VirtualKey::CapitalLock, "CapitalLock"},
		{VirtualKey::Kana, "Kana"},
		{VirtualKey::Hangul, "Hangul"},
		{VirtualKey::Junja, "Junja"},
		{VirtualKey::Final, "Final"},
		{VirtualKey::Hanja, "Hanja"},
		{VirtualKey::Kanji, "Kanji"},
		{VirtualKey::Escape, "Escape"},
		{VirtualKey::Convert, "Convert"},
		{VirtualKey::NonConvert, "NonConvert"},
		{VirtualKey::Accept, "Accept"},
		{VirtualKey::ModeChange, "ModeChange"},
		{VirtualKey::Space, "Space"},
		{VirtualKey::PageUp, "PageUp"},
		{VirtualKey::PageDown, "PageDown"},
		{VirtualKey::End, "End"},
		{VirtualKey::Home, "Home"},
		{VirtualKey::Left, "Left"},
		{VirtualKey::Up, "Up"},
		{VirtualKey::Right, "Right"},
		{VirtualKey::Down, "Down"},
		{VirtualKey::Select, "Select"},
		{VirtualKey::Print, "Print"},
		{VirtualKey::Execute, "Execute"},
		{VirtualKey::Snapshot, "Snapshot"},
		{VirtualKey::Insert, "Insert"},
		{VirtualKey::Delete, "Delete"},
		{VirtualKey::Help, "Help"},
		{VirtualKey::Number0, "Number0"},
		{VirtualKey::Number1, "Number1"},
		{VirtualKey::Number2, "Number2"},
		{VirtualKey::Number3, "Number3"},
		{VirtualKey::Number4, "Number4"},
		{VirtualKey::Number5, "Number5"},
		{VirtualKey::Number6, "Number6"},
		{VirtualKey::Number7, "Number7"},
		{VirtualKey::Number8, "Number8"},
		{VirtualKey::Number9, "Number9"},
		{VirtualKey::A, "A"},
		{VirtualKey::B, "B"},
		{VirtualKey::C, "C"},
		{VirtualKey::D, "D"},
		{VirtualKey::E, "E"},
		{VirtualKey::F, "F"},
		{VirtualKey::G, "G"},
		{VirtualKey::H, "H"},
		{VirtualKey::I, "I"},
		{VirtualKey::J, "J"},
		{VirtualKey::K, "K"},
		{VirtualKey::L, "L"},
		{VirtualKey::M, "M"},
		{VirtualKey::N, "N"},
		{VirtualKey::O, "O"},
		{VirtualKey::P, "P"},
		{VirtualKey::Q, "Q"},
		{VirtualKey::R, "R"},
		{VirtualKey::S, "S"},
		{VirtualKey::T, "T"},
		{VirtualKey::U, "U"},
		{VirtualKey::V, "V"},
		{VirtualKey::W, "W"},
		{VirtualKey::X, "X"},
		{VirtualKey::Y, "Y"},
		{VirtualKey::Z, "Z"},
		{VirtualKey::LeftWindows, "LeftWindows"},
		{VirtualKey::RightWindows, "RightWindows"},
		{VirtualKey::Application, "Application"},
		{VirtualKey::Sleep, "Sleep"},
		{VirtualKey::NumberPad0, "Keypad+0"},
		{VirtualKey::NumberPad1, "Keypad+1"},
		{VirtualKey::NumberPad2, "Keypad+2"},
		{VirtualKey::NumberPad3, "Keypad+3"},
		{VirtualKey::NumberPad4, "Keypad+4"},
		{VirtualKey::NumberPad5, "Keypad+5"},
		{VirtualKey::NumberPad6, "Keypad+6"},
		{VirtualKey::NumberPad7, "Keypad+7"},
		{VirtualKey::NumberPad8, "Keypad+8"},
		{VirtualKey::NumberPad9, "Keypad+9"},
		{VirtualKey::Multiply, "Multiply"},
		{VirtualKey::Add, "Add"},
		{VirtualKey::Separator, "Separator"},
		{VirtualKey::Subtract, "Subtract"},
		{VirtualKey::Decimal, "Decimal"},
		{VirtualKey::Divide, "Divide"},
		{VirtualKey::F1, "F1"},
		{VirtualKey::F2, "F2"},
		{VirtualKey::F3, "F3"},
		{VirtualKey::F4, "F4"},
		{VirtualKey::F5, "F5"},
		{VirtualKey::F6, "F6"},
		{VirtualKey::F7, "F7"},
		{VirtualKey::F8, "F8"},
		{VirtualKey::F9, "F9"},
		{VirtualKey::F10, "F10"},
		{VirtualKey::F11, "F11"},
		{VirtualKey::F12, "F12"},
		{VirtualKey::F13, "F13"},
		{VirtualKey::F14, "F14"},
		{VirtualKey::F15, "F15"},
		{VirtualKey::F16, "F16"},
		{VirtualKey::F17, "F17"},
		{VirtualKey::F18, "F18"},
		{VirtualKey::F19, "F19"},
		{VirtualKey::F20, "F20"},
		{VirtualKey::F21, "F21"},
		{VirtualKey::F22, "F22"},
		{VirtualKey::F23, "F23"},
		{VirtualKey::F24, "F24"},
		{VirtualKey::NavigationView, "NavigationView"},
		{VirtualKey::NavigationMenu, "NavigationMenu"},
		{VirtualKey::NavigationUp, "NavigationUp"},
		{VirtualKey::NavigationDown, "NavigationDown"},
		{VirtualKey::NavigationLeft, "NavigationLeft"},
		{VirtualKey::NavigationRight, "NavigationRight"},
		{VirtualKey::NavigationAccept, "NavigationAccept"},
		{VirtualKey::NavigationCancel, "NavigationCancel"},
		{VirtualKey::NumberKeyLock, "NumberKeyLock"},
		{VirtualKey::Scroll, "Scroll"},
		{VirtualKey::LeftShift, "LeftShift"},
		{VirtualKey::RightShift, "RightShift"},
		{VirtualKey::LeftControl, "LeftControl"},
		{VirtualKey::RightControl, "RightControl"},
		{VirtualKey::LeftMenu, "LeftMenu"},
		{VirtualKey::RightMenu, "RightMenu"},
		{VirtualKey::GoBack, "GoBack"},
		{VirtualKey::GoForward, "GoForward"},
		{VirtualKey::Refresh, "Refresh"},
		{VirtualKey::Stop, "Stop"},
		{VirtualKey::Search, "Search"},
		{VirtualKey::Favorites, "Favorites"},
		{VirtualKey::GoHome, "GoHome"}};

	static const char* GetKeyName(VirtualKey key)
	{
		const auto it = s_key_names.find(key);
		return it == s_key_names.end() ? nullptr : it->second;
	}

	static std::optional<VirtualKey> GetKeyCodeForName(const std::string_view& key_name)
	{
		for (const auto& it : s_key_names)
		{
			if (key_name == it.second)
				return it.first;
		}

		return std::nullopt;
	}
} // namespace UWPKeyNames
