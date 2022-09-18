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
 *  You should have received a copy of the GNU General Public License along with XBSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

struct ImFont;

union InputBindingKey;
enum class GenericInputBinding : u8;

namespace ImGuiManager
{
	/// Initializes ImGui, creates fonts, etc.
	bool Initialize();

	/// Frees all ImGui resources.
	void Shutdown();

	/// Updates internal state when the window is size.
	void WindowResized();

	/// Updates scaling of the on-screen elements.
	void UpdateScale();

	/// Call at the beginning of the frame to set up ImGui state.
	void NewFrame();

	/// Renders any on-screen display elements.
	void RenderOSD();

	/// Returns the scale of all on-screen elements.
	float GetGlobalScale();

	/// Returns true if fullscreen fonts are present.
	bool HasFullscreenFonts();

	/// Allocates/adds fullscreen fonts if they're not loaded.
	bool AddFullscreenFontsIfMissing();

	/// Returns the standard font for external drawing.
	ImFont* GetStandardFont();

	/// Returns the fixed-width font for external drawing.
	ImFont* GetFixedFont();

	/// Returns the medium font for external drawing, scaled by ImGuiFullscreen.
	/// This font is allocated on demand.
	ImFont* GetMediumFont();

	/// Returns the large font for external drawing, scaled by ImGuiFullscreen.
	/// This font is allocated on demand.
	ImFont* GetLargeFont();

#ifdef XBSX2_CORE
	/// Returns true if imgui wants to intercept text input.
	bool WantsTextInput();

	/// Called on the UI or CPU thread in response to a key press. String is UTF-8.
	void AddTextInput(std::string str);

	/// Called on the UI or CPU thread in response to mouse movement.
	void UpdateMousePosition(float x, float y);

	/// Called on the CPU thread in response to a mouse button press.
	/// Returns true if ImGui intercepted the event, and regular handlers should not execute.
	bool ProcessPointerButtonEvent(InputBindingKey key, float value);

	/// Called on the CPU thread in response to a mouse wheel movement.
	/// Returns true if ImGui intercepted the event, and regular handlers should not execute.
	bool ProcessPointerAxisEvent(InputBindingKey key, float value);

	/// Called on the CPU thread in response to a key press.
	/// Returns true if ImGui intercepted the event, and regular handlers should not execute.
	bool ProcessHostKeyEvent(InputBindingKey key, float value);

	/// Called on the CPU thread when any input event fires. Allows imgui to take over controller navigation.
	bool ProcessGenericInputEvent(GenericInputBinding key, float value);
#endif
} // namespace ImGuiManager

namespace Host
{
	/// Called by ImGuiManager when the cursor enters a text field. The host may choose to open an on-screen
	/// keyboard for devices without a physical keyboard.
	void BeginTextInput();

	/// Called by ImGuiManager when the cursor leaves a text field.
	void EndTextInput();
} // namespace Host