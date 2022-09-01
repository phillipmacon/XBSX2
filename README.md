# XBSX2
XBSX2 - The Playstation 2 Emulator for Xbox Consoles

# Original Source Code
Below is an archive of Stenzeks original PUBLIC source which has since been expanded and improved upon.
https://github.com/PCSX2/pcsx2/archive/a3eeed316a0bdcb60a445fe6237b0777f40d553b.zip

# Improvements Made To The Original Source Code

- Backports from PCSX2 to both the GameDB as well as various changes to the backend, this has resulted in some improvements such as the Spider-Man games now working with the Hardware Renderer (don't get too excited, they crash after a couple minutes).

- Lots of changes to UI elements, the settings page has been heavily altered so where most options are located makes more sense, also has a Directory settings tab where you can set the directories to wherever you wish.

- Made some changes to the control scheme, I would recommend viewing the little question mark bubble in the top right of the main menu but basically LB/RB can be used to jump through Game List entries, pressing Start on an entry now opens up the Game Properties menu and the Back button exits to Main Menu.

- Fixed the looping settings issue caused by 2 options being named the same.

- Changed the default font used by the app to one users should be more familiar with.

- Altered how most of the OSD elements are displayed, the FPS counter no longer has [G] and [V], the FPS now just shows the games internal FPS instead.

- Altered the language used for things like switching to the Software/Hardware renderer to be not so long-winded.

- Removed most options located in the Game Properties menu due to them being non-functional.

- Changed around the Quick Menu option placements to be more inline with something we're all familiar with.

- Added basic shaders/filters in the form of FXAA, Brightness, Contrast and Saturation adjustments and some CRT filters.

- Able to remap controls/hotkeys from within the Settings.

- Networking features are enabled by default making online play much simpler for new users.

- Disables the Enable XInput Source option from Controller Settings to avoid any users potentially disabling it.

- Pressure Sensitive Buttons will now actually work with the Apply Pressure setting in controls, before the pressure modifier was set too high resulting in the button press being registered as a hold regardless.

- When saving to a Memory Card Folder there will now be an appropriate OSD message displaying as such.

- New Exploded PS2 App, Splash and Menu Icon.

- Adjusted how the PAD values were stored resulting in only being able to choose between 0/1/2% instead of the intended 100%/200% for most settings.

- The Back Button is no longer hard coded to be the "Go Back" button, instead you now can use the B button to close menus AND use it to go back, doing this previously wasn't possible.

- The icons used on the Settings/Game Settings bar have been resized (slightly larger) and they are no longer marginally cut off on the left hand side.

- The CRC code of the selected game is now displayed along side Filename, Region etc which should make confirming the CRC less inconvenient and so much more!

# Known Issues
- Some widescreen patches/cheats causing a crash when booting a game.
