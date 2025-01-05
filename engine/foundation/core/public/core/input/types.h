////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
enum InputMouseItem {
  InputMouseItem_None,
  InputMouseItem_ButtonLeft = 0,
  InputMouseItem_ButtonRight,
  InputMouseItem_ButtonMiddle,
  InputMouseItem_Button4,
  InputMouseItem_Button5,
  InputMouseItem_Wheel,
  InputMouseItem_Move,
  InputMouseItem_Position,
  InputMouseItem_Count
};

enum InputControllerType {
  InputControllerType_None,
  InputControllerType_Keyboard,
  InputControllerType_Mouse
};

enum InputKeyboardItem {
  InputKeyboardItem_None = 0x00,
  InputKeyboardItem_LButton = 0x01,                         // "Left Button"
  InputKeyboardItem_RButton = 0x02,                         // "Right Button"
  InputKeyboardItem_Cancel = 0x03,                          // "Cancel"
  InputKeyboardItem_MButton = 0x04,                         // "Middle Button"
  InputKeyboardItem_XButton1 = 0x05,                        // "X1 Button"
  InputKeyboardItem_XButton2 = 0x06,                        // "X2 Button"
  InputKeyboardItem_Backspace = 0x08,                       // "Backspace"                   // VK_BACK
  InputKeyboardItem_Tab = 0x09,                             // "Tab"
  InputKeyboardItem_Clear = 0x0C,                           // "Clear"
  InputKeyboardItem_Enter = 0x0D,                           // "Enter"                       // VK_RETURN
  InputKeyboardItem_Shift = 0x10,                           // "Shift"
  InputKeyboardItem_Control = 0x11,                         // "Control"
  InputKeyboardItem_Menu = 0x12,                            // "Menu"
  InputKeyboardItem_Pause = 0x13,                           // "Pause"
  InputKeyboardItem_Capslock = 0x14,                        // "Caps Lock"                   // VK_CAPITAL
  InputKeyboardItem_Kana = 0x15,                            // "Kana"
  InputKeyboardItem_Junja = 0x17,                           // "Junja"
  InputKeyboardItem_Final = 0x18,                           // "Final"
  InputKeyboardItem_Hanja = 0x19,                           // "Hanja"
  InputKeyboardItem_Escape = 0x1B,                          // "Escape"
  InputKeyboardItem_Convert = 0x1C,                         // "Convert"                     // IME
  InputKeyboardItem_NonConvert = 0x1D,                      // "Non Convert"                 // IME
  InputKeyboardItem_Accept = 0x1E,                          // "Accept"                      // IME
  InputKeyboardItem_ModeChange = 0x1F,                      // "Mode Change"                 // IME
  InputKeyboardItem_Space = 0x20,                           // "Space"
  InputKeyboardItem_PageUp = 0x21,                          // "Page Up"                     // VK_PRIOR
  InputKeyboardItem_PageDown = 0x22,                        // "Page Down"                   // VK_NEXT
  InputKeyboardItem_End = 0x23,                             // "End"
  InputKeyboardItem_Home = 0x24,                            // "Home"
  InputKeyboardItem_Left = 0x25,                            // "Left"
  InputKeyboardItem_Up = 0x26,                              // "Up"
  InputKeyboardItem_Right = 0x27,                           // "Right"
  InputKeyboardItem_Down = 0x28,                            // "Down"
  InputKeyboardItem_Select = 0x29,                          // "Select"
  InputKeyboardItem_Print = 0x2A,                           // "Print"
  InputKeyboardItem_Execute = 0x2B,                         // "Execute"
  InputKeyboardItem_PrintScreen = 0x2C,                     // "Print Screen"                // VK_SNAPSHOT, SysRq
  InputKeyboardItem_Insert = 0x2D,                          // "Insert"
  InputKeyboardItem_Delete = 0x2E,                          // "Delete"
  InputKeyboardItem_Help = 0x2F,                            // "Help"
  InputKeyboardItem_0 = 0x30,                               // "0"
  InputKeyboardItem_1 = 0x31,                               // "1"
  InputKeyboardItem_2 = 0x32,                               // "2"
  InputKeyboardItem_3 = 0x33,                               // "3"
  InputKeyboardItem_4 = 0x34,                               // "4"
  InputKeyboardItem_5 = 0x35,                               // "5"
  InputKeyboardItem_6 = 0x36,                               // "6"
  InputKeyboardItem_7 = 0x37,                               // "7"
  InputKeyboardItem_8 = 0x38,                               // "8"
  InputKeyboardItem_9 = 0x39,                               // "9"
  InputKeyboardItem_A = 0x41,                               // "A"
  InputKeyboardItem_B = 0x42,                               // "B"
  InputKeyboardItem_C = 0x43,                               // "C"
  InputKeyboardItem_D = 0x44,                               // "D"
  InputKeyboardItem_E = 0x45,                               // "E"
  InputKeyboardItem_F = 0x46,                               // "F"
  InputKeyboardItem_G = 0x47,                               // "G"
  InputKeyboardItem_H = 0x48,                               // "H"
  InputKeyboardItem_I = 0x49,                               // "I"
  InputKeyboardItem_J = 0x4a,                               // "J"
  InputKeyboardItem_K = 0x4b,                               // "K"
  InputKeyboardItem_L = 0x4c,                               // "L"
  InputKeyboardItem_M = 0x4d,                               // "M"
  InputKeyboardItem_N = 0x4e,                               // "N"
  InputKeyboardItem_O = 0x4f,                               // "O"
  InputKeyboardItem_P = 0x50,                               // "P"
  InputKeyboardItem_Q = 0x51,                               // "Q"
  InputKeyboardItem_R = 0x52,                               // "R"
  InputKeyboardItem_S = 0x53,                               // "S"
  InputKeyboardItem_T = 0x54,                               // "T"
  InputKeyboardItem_U = 0x55,                               // "U"
  InputKeyboardItem_V = 0x56,                               // "V"
  InputKeyboardItem_W = 0x57,                               // "W"
  InputKeyboardItem_X = 0x58,                               // "X"
  InputKeyboardItem_Y = 0x59,                               // "Y"
  InputKeyboardItem_Z = 0x5a,                               // "Z"
  InputKeyboardItem_LWin = 0x5B,                            // "LWindows"
  InputKeyboardItem_RWin = 0x5C,                            // "RWindows"
  InputKeyboardItem_Apps = 0x5D,                            // "Apps"
  InputKeyboardItem_Sleep = 0x5F,                           // "Sleep"
  InputKeyboardItem_Numpad0 = 0x60,                         // "Num 0"
  InputKeyboardItem_Numpad1 = 0x61,                         // "Num 1"
  InputKeyboardItem_Numpad2 = 0x62,                         // "Num 2"
  InputKeyboardItem_Numpad3 = 0x63,                         // "Num 3"
  InputKeyboardItem_Numpad4 = 0x64,                         // "Num 4"
  InputKeyboardItem_Numpad5 = 0x65,                         // "Num 5"
  InputKeyboardItem_Numpad6 = 0x66,                         // "Num 6"
  InputKeyboardItem_Numpad7 = 0x67,                         // "Num 7"
  InputKeyboardItem_Numpad8 = 0x68,                         // "Num 8"
  InputKeyboardItem_Numpad9 = 0x69,                         // "Num 9"
  InputKeyboardItem_NumpadAsterik = 0x6A,                  // "Num *"                       // VK_MULTIPLY
  InputKeyboardItem_NumpadPlus = 0x6B,                      // "Num +"                       // VK_ADD
  InputKeyboardItem_NumpadEnter = 0x6C,                     // "Num Enter"                   // VK_SEPARATOR
  InputKeyboardItem_NumpadMinus = 0x6D,                     // "Num -"                       // VK_SUBTRACT
  InputKeyboardItem_NumpadDot = 0x6E,                       // "Num ."                       // VK_DECIMAL
  InputKeyboardItem_NumpadSlash = 0x6F,                     // "Num /"                       // VK_DIVIDE
  InputKeyboardItem_F1 = 0x70,                              // "F1"
  InputKeyboardItem_F2 = 0x71,                              // "F2"
  InputKeyboardItem_F3 = 0x72,                              // "F3"
  InputKeyboardItem_F4 = 0x73,                              // "F4"
  InputKeyboardItem_F5 = 0x74,                              // "F5"
  InputKeyboardItem_F6 = 0x75,                              // "F6"
  InputKeyboardItem_F7 = 0x76,                              // "F7"
  InputKeyboardItem_F8 = 0x77,                              // "F8"
  InputKeyboardItem_F9 = 0x78,                              // "F9"
  InputKeyboardItem_F10 = 0x79,                             // "F10"
  InputKeyboardItem_F11 = 0x7A,                             // "F11"
  InputKeyboardItem_F12 = 0x7B,                             // "F12"
  InputKeyboardItem_F13 = 0x7C,                             // "F13"
  InputKeyboardItem_F14 = 0x7D,                             // "F14"
  InputKeyboardItem_F15 = 0x7E,                             // "F15"
  InputKeyboardItem_F16 = 0x7F,                             // "F16"
  InputKeyboardItem_F17 = 0x80,                             // "F17"
  InputKeyboardItem_F18 = 0x81,                             // "F18"
  InputKeyboardItem_F19 = 0x82,                             // "F19"
  InputKeyboardItem_F20 = 0x83,                             // "F20"
  InputKeyboardItem_F21 = 0x84,                             // "F21"
  InputKeyboardItem_F22 = 0x85,                             // "F22"
  InputKeyboardItem_F23 = 0x86,                             // "F23"
  InputKeyboardItem_F24 = 0x87,                             // "F24"
  InputKeyboardItem_NavigationView = 0x88,                 // "Navigation View"
  InputKeyboardItem_NavigationMenu = 0x89,                 // "Navigation Menu"
  InputKeyboardItem_NavigationUp = 0x8A,                   // "Navigation Up"
  InputKeyboardItem_NavigationDown = 0x8B,                 // "Navigation Down"
  InputKeyboardItem_NavigationLeft = 0x8C,                 // "Navigation Left"
  InputKeyboardItem_NavigationRight = 0x8D,                // "Navigation Right"
  InputKeyboardItem_NavigationAccept = 0x8E,               // "Navigation Accept"
  InputKeyboardItem_NavigationCancel = 0x8F,               // "Navigation Cancel"
  InputKeyboardItem_NumLock = 0x90,                         // "Num Lock"
  InputKeyboardItem_ScrollLock = 0x91,                      // "Scroll Lock"                 // VK_SCROLL
  InputKeyboardItem_NumpadEequal = 0x92,                     // "Num ="                       // VK_OEM_NEC_EQUAL, 'Dictionary' key
  InputKeyboardItem_OEM_FJ_MassHou = 0x93,                  // "Unregister Word"             // 'Unregister word' key
  InputKeyboardItem_OEM_FJ_Touroku = 0x94,                  // "Register Word"               // 'Register word' key
  InputKeyboardItem_OEM_FJ_Loya = 0x95,                     // "Left OYAYUBI"                // 'Left OYAYUBI' key
  InputKeyboardItem_OEM_FJ_Roya = 0x96,                     // "Right OYAYUBI"               // 'Right OYAYUBI' key
  InputKeyboardItem_LeftShift = 0xA0,                       // "LShift"                      // VK_LSHIFT
  InputKeyboardItem_RightShift = 0xA1,                      // "RShift"                      // VK_RSHIFT
  InputKeyboardItem_LeftControl = 0xA2,                     // "LCtrl"                       // VK_LCONTROL
  InputKeyboardItem_RightControl = 0xA3,                    // "RCtrl"                       // VK_RCONTROL
  InputKeyboardItem_LeftAlt = 0xA4,                         // "LAlt"                        // VK_LMENU
  InputKeyboardItem_RightAlt = 0xA5,                        // "RAlt"                        // VK_RMENU
  InputKeyboardItem_BrowserBack = 0xA6,                    // "Browser Back"
  InputKeyboardItem_BrowserForward = 0xA7,                 // "Browser Forward"
  InputKeyboardItem_BrowserRefresh = 0xA8,                 // "Browser Refresh"
  InputKeyboardItem_BrowserStop = 0xA9,                    // "Browser Stop"
  InputKeyboardItem_BrowserSearch = 0xAA,                  // "Browser Search"
  InputKeyboardItem_BrowserFavorites = 0xAB,               // "Browser Favorites"
  InputKeyboardItem_BrowserHome = 0xAC,                    // "Browser Home"
  InputKeyboardItem_VolumeMute = 0xAD,                     // "Volume Mute"
  InputKeyboardItem_VolumeDown = 0xAE,                     // "Volume Down"
  InputKeyboardItem_VolumeUp = 0xAF,                       // "Volume Up"
  InputKeyboardItem_MediaNextTrack = 0xB0,                // "Media Next Track"
  InputKeyboardItem_MediaPrevTrack = 0xB1,                // "Media Prev Track"
  InputKeyboardItem_MediaStop = 0xB2,                      // "Media Stop"
  InputKeyboardItem_MediaPlayPause = 0xB3,                // "Media Play Pause"
  InputKeyboardItem_LaunchMail = 0xB4,                     // "Launch Mail"
  InputKeyboardItem_LaunchMediaSelect = 0xB5,             // "Launch Media Select"
  InputKeyboardItem_LaunchApp1 = 0xB6,                     // "Launch App 1"
  InputKeyboardItem_LaunchApp2 = 0xB7,                     // "Launch App 2"
  InputKeyboardItem_Semicolon = 0xBA,                       // "Semicolon"                   // VK_OEM_1
  InputKeyboardItem_Equal = 0xBB,                           // "Equal"                       // VK_OEM_PLUS
  InputKeyboardItem_Comma = 0xBC,                           // "Comma"                       // VK_OEM_COMMA
  InputKeyboardItem_Minus = 0xBD,                           // "Minus"                       // VK_OEM_MINUS
  InputKeyboardItem_Dot = 0xBE,                             // "Dot"                         // VK_OEM_PERIOD
  InputKeyboardItem_Slash = 0xBF,                           // "Slash"                       // VK_OEM_2
  InputKeyboardItem_Grave = 0xC0,                           // "Grave"                       // VK_OEM_3
  InputKeyboardItem_GAMEPAD_A = 0xC3,                       // "Gamepad A"
  InputKeyboardItem_GAMEPAD_B = 0xC4,                       // "Gamepad B"
  InputKeyboardItem_GAMEPAD_X = 0xC5,                       // "Gamepad X"
  InputKeyboardItem_GAMEPAD_Y = 0xC6,                       // "Gamepad Y"
  InputKeyboardItem_GAMEPAD_RIGHT_SHOULDER = 0xC7,          // "Gamepad RShoulder"
  InputKeyboardItem_GAMEPAD_LEFT_SHOULDER = 0xC8,           // "Gamepad LShoulder"
  InputKeyboardItem_GAMEPAD_LEFT_TRIGGER = 0xC9,            // "Gamepad LTrigger"
  InputKeyboardItem_GAMEPAD_RIGHT_TRIGGER = 0xCA,           // "Gamepad RTrigger"
  InputKeyboardItem_GAMEPAD_DPAD_UP = 0xCB,                 // "Gamepad DPad Up"
  InputKeyboardItem_GAMEPAD_DPAD_DOWN = 0xCC,               // "Gamepad DPad Down"
  InputKeyboardItem_GAMEPAD_DPAD_LEFT = 0xCD,               // "Gamepad DPad Left"
  InputKeyboardItem_GAMEPAD_DPAD_RIGHT = 0xCE,              // "Gamepad DPad Right"
  InputKeyboardItem_GAMEPAD_MENU = 0xCF,                    // "Gamepad Menu"
  InputKeyboardItem_GAMEPAD_VIEW = 0xD0,                    // "Gamepad View"
  InputKeyboardItem_GAMEPAD_LEFT_THUMBSTICK_Button = 0xD1,  // "Gamepad LThumbstick Button"
  InputKeyboardItem_GAMEPAD_RIGHT_THUMBSTICK_Button = 0xD2, // "Gamepad RThumbstick Button"
  InputKeyboardItem_GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3,      // "Gamepad LThumbstick Up"
  InputKeyboardItem_GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4,    // "Gamepad LThumbstick Down"
  InputKeyboardItem_GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5,   // "Gamepad LThumbstick Right"
  InputKeyboardItem_GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6,    // "Gamepad LThumbstick Left"
  InputKeyboardItem_GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7,     // "Gamepad RThumbstick Up"
  InputKeyboardItem_GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8,   // "Gamepad RThumbstick Down"
  InputKeyboardItem_GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9,  // "Gamepad RThumbstick Right"
  InputKeyboardItem_GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA,   // "Gamepad RThumbstick Left"
  InputKeyboardItem_LeftBrace = 0xDB,                       // "LBrace"                      // VK_OEM_4
  InputKeyboardItem_BackSlash = 0xDC,                       // "Backslash"                   // VK_OEM_5
  InputKeyboardItem_RightBRace = 0xDD,                      // "RBrace"                      // VK_OEM_6
  InputKeyboardItem_Apostrophe = 0xDE,                      // "Apostrophe"                  // VK_OEM_7
  InputKeyboardItem_OEM_8 = 0xDF,                           // "OEM 8"
  InputKeyboardItem_OEM_AX = 0xE1,                          // "OEM AX"                      // 'AX' key on Japanese AX kbd
  InputKeyboardItem_OEM_102 = 0xE2,                         // "OEM 102",                    // "<>" or "\|" //  "<>" or "\|" on RT 102-key kbd.
  InputKeyboardItem_ICO_HELP = 0xE3,                        // "ICO Help"                    // Help key on ICO
  InputKeyboardItem_ICO_00 = 0xE4,                          // "ICO 00"                      // 00 key on ICO
  InputKeyboardItem_ProcessKey = 0xE5,                      // "Process Key"
  InputKeyboardItem_ICO_CLEAR = 0xE6,                       // "ICO Clear"
  InputKeyboardItem_PACKET = 0xE7,                          // "Packet"
  InputKeyboardItem_OEM_RESET = 0xE9,                       // "OEM Reset"
  InputKeyboardItem_OEM_JUMP = 0xEA,                        // "OEM Jump"
  InputKeyboardItem_OEM_PA1 = 0xEB,                         // "OEM PA1"
  InputKeyboardItem_OEM_PA2 = 0xEC,                         // "OEM PA2"
  InputKeyboardItem_OEM_PA3 = 0xED,                         // "OEM PA3"
  InputKeyboardItem_OEM_WSCTRL = 0xEE,                      // "OEM WsCtrl"
  InputKeyboardItem_OEM_CUSEL = 0xEF,                       // "OEM CuSel"
  InputKeyboardItem_OEM_ATTN = 0xF0,                        // "OEM Attn"
  InputKeyboardItem_OEM_FINISH = 0xF1,                      // "OEM Finish"
  InputKeyboardItem_COPY = 0xF2,                            // "Copy"
  InputKeyboardItem_OEM_AUTO = 0xF3,                        // "OEM Auto"
  InputKeyboardItem_OEM_ENLW = 0xF4,                        // "OEM ENLW"
  InputKeyboardItem_OEM_BACKTAB = 0xF5,                     // "OEM Backtab"
  InputKeyboardItem_Attn = 0xF6,                            // "Attn"
  InputKeyboardItem_CrSel = 0xF7,                           // "CrSel"
  InputKeyboardItem_ExSel = 0xF8,                           // "ExSel"
  InputKeyboardItem_EREOF = 0xF9,                           // "EREOF"
  InputKeyboardItem_Play = 0xFA,                            // "Play"
  InputKeyboardItem_Zoom = 0xFB,                            // "Zoom"
  InputKeyboardItem_NoName = 0xFC,                          // "No Name"
  InputKeyboardItem_PA1 = 0xFD,                             // "PA1"
  InputKeyboardItem_OEM_CLEAR = 0xFE,                       // "OEM Clear"

  // Duplicate/alternate key names
  InputKeyboardItem_KANJI        = 0x19,
  InputKeyboardItem_OEM_FJ_JISHO = 0x92,

  // Keys not available as Windows virtual keys.
  //
  // I've tried as best I can to match up the key codes from the OS X HID input system with
  // Windows Virtual Key Codes, but I might have missed something. I.e., some of the keys listed
  // below might be duplicates of the Windows keys above. We should test with a bunch of keyboards
  // on OS X and Windows and see that we have the best key matchup possible.

  InputKeyboardItem_HASHTILDE = 0xFF,
  InputKeyboardItem_102ND,
  InputKeyboardItem_COMPOSE,
  InputKeyboardItem_POWER,

  InputKeyboardItem_OPEN,
  InputKeyboardItem_PROPS,
  InputKeyboardItem_FRONT,
  InputKeyboardItem_STOP,
  InputKeyboardItem_AGAIN,
  InputKeyboardItem_UNDO,
  InputKeyboardItem_CUT,
  InputKeyboardItem_PASTE,
  InputKeyboardItem_FIND,

  InputKeyboardItem_NumpadComma,

  InputKeyboardItem_RO, // Keyboard International1
  InputKeyboardItem_KATAKANAHIRAGANA, // Keyboard International2
  InputKeyboardItem_YEN, // Keyboard International3
  InputKeyboardItem_HENKAN, // Keyboard International4
  InputKeyboardItem_MUHENKAN, // Keyboard International5
  InputKeyboardItem_NumpadJPCOMMA, // Keyboard International6
  InputKeyboardItem_INTERNATIONAL_7,
  InputKeyboardItem_INTERNATIONAL_8,
  InputKeyboardItem_INTERNATIONAL_9,

  InputKeyboardItem_HANGEUL, // Keyboard LANG1
  InputKeyboardItem_KATAKANA, // Keyboard LANG3
  InputKeyboardItem_HIRAGANA, // Keyboard LANG4
  InputKeyboardItem_ZENKAKUHANKAKU, // Keyboard LANG5
  InputKeyboardItem_LANG_6,
  InputKeyboardItem_LANG_7,
  InputKeyboardItem_LANG_8,
  InputKeyboardItem_LANG_9,

  InputKeyboardItem_NumpadLEFTPAREN,
  InputKeyboardItem_NumpadRIGHTPAREN,

  InputKeyboardItem_LEFTMETA,
  InputKeyboardItem_RIGHTMETA,

  InputKeyboardItem_MEDIA_EJECT,
  InputKeyboardItem_MEDIA_VolumeUP,
  InputKeyboardItem_MEDIA_VolumeDOWN,
  InputKeyboardItem_MEDIA_MUTE,

  InputKeyboardItem_BrowserWWW,
  InputKeyboardItem_BrowserSCROLLUP,
  InputKeyboardItem_BrowserSCROLLDOWN,
  InputKeyboardItem_BrowserEDIT,
  InputKeyboardItem_BrowserSLEEP,
  InputKeyboardItem_BrowserCOFFEE,
  InputKeyboardItem_BrowserCALC,

  InputKeyboardItem_Count,
};


enum class ControllerType {
  ControllerType_Device = 0,
  ControllerType_Virtual
};

enum class ControlType {
  ControlType_Unknown,
  ControlType_Button,
  ControlType_Axis,
  ControlType_Led,
  ControlType_Effect
};

enum class DeviceBackendType {
  DeviceBackendType_Unknown,
  DeviceBackendType_UpdateDevice,
  DeviceBackendType_ConnectionDevice,
  DeviceBackendType_Hid
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core