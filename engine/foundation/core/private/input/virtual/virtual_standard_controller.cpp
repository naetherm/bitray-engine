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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/input/virtual/virtual_standard_controller.h"
#include "core/input/input_server.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


VirtualStandardController::VirtualStandardController(InputServer& inputManager)
: VirtualController(inputManager, "VirtualStandardController", "Standard virtual input controller")
// Mouse
, MouseX						(*this, "MouseX",						"X axis (movement data, no absolute data)")
, MouseY						(*this, "MouseY",						"Y axis (movement data, no absolute data)")
, MouseWheel					(*this, "MouseWheel",					"Mouse wheel (movement data, no absolute data)")
, MouseLeft					(*this, "MouseLeft",					"Left mouse button (mouse button #0)")
, MouseRight					(*this, "MouseRight",					"Right mouse button (mouse button #1)")
, MouseMiddle					(*this, "MouseMiddle",					"Middle mouse button (mouse button #2)")
, MouseButton4				(*this, "MouseButton4",					"Mouse button #4")
, MouseButton5				(*this, "MouseButton5",					"Mouse button #5")
, MouseButton6				(*this, "MouseButton6",					"Mouse button #6")
, MouseButton7				(*this, "MouseButton7",					"Mouse button #7")
, MouseButton8				(*this, "MouseButton8",					"Mouse button #8")
, MouseButton9				(*this, "MouseButton9",					"Mouse button #9")
, MouseButton10				(*this, "MouseButton10",				"Mouse button #10")
, MouseButton11				(*this, "MouseButton11",				"Mouse button #11")
, MouseButton12				(*this, "MouseButton12",				"Mouse button #12")
// Keyboard
, KeyboardBackspace			(*this, "KeyboardBackspace",			"Backspace",																	0x08)
, KeyboardTab					(*this, "KeyboardTab",					"Tabulator",																	0x09)
, KeyboardClear				(*this, "KeyboardClear",				"Clear (not available everywhere)",												0x00)
, KeyboardReturn				(*this, "KeyboardReturn",				"Return (often the same as \"Enter\")",											0x0D)
, KeyboardShift				(*this, "KeyboardShift",				"Shift",																		0x00)
, KeyboardControl				(*this, "KeyboardControl",				"Control (\"Ctrl\")",															0x00)
, KeyboardAlt					(*this, "KeyboardAlt",					"Alt",																			0x00)
, KeyboardPause				(*this, "KeyboardPause",				"Pause",																		0x00)
, KeyboardCapsLock			(*this, "KeyboardCapsLock",				"Caps lock",																	0x00)
, KeyboardEscape				(*this, "KeyboardEscape",				"Escape",																		0x1B)
, KeyboardSpace				(*this, "KeyboardSpace",				"Space",																		0x20)
, KeyboardPageUp				(*this, "KeyboardPageUp",				"Page up",																		0x00)
, KeyboardPageDown			(*this, "KeyboardPageDown",				"Page down",																	0x00)
, KeyboardEnd					(*this, "KeyboardEnd",					"End",																			0x00)
, KeyboardHome				(*this, "KeyboardHome",					"Home",																			0x00)
, KeyboardLeft				(*this, "KeyboardLeft",					"Left arrow",																	0x00)
, KeyboardUp					(*this, "KeyboardUp",					"Up arrow",																		0x00)
, KeyboardRight				(*this, "KeyboardRight",				"Right arrow",																	0x00)
, KeyboardDown				(*this, "KeyboardDown",					"Down arrow",																	0x00)
, KeyboardSelect				(*this, "KeyboardSelect",				"Select (not available everywhere)",											0x00)
, KeyboardExecute				(*this, "KeyboardExecute",				"Execute (not available everywhere)",											0x00)
, KeyboardPrint				(*this, "KeyboardPrint",				"Print screen",																	0x00)
, KeyboardInsert				(*this, "KeyboardInsert",				"Insert",																		0x00)
, KeyboardDelete				(*this, "KeyboardDelete",				"Delete",																		0x7F)
, KeyboardHelp				(*this, "KeyboardHelp",					"Help (not available everywhere)",												0x00)
, Keyboard0					(*this, "Keyboard0",					"0",																			'0')
, Keyboard1					(*this, "Keyboard1",					"1",																			'1')
, Keyboard2					(*this, "Keyboard2",					"2",																			'2')
, Keyboard3					(*this, "Keyboard3",					"3",																			'3')
, Keyboard4					(*this, "Keyboard4",					"4",																			'4')
, Keyboard5					(*this, "Keyboard5",					"5",																			'5')
, Keyboard6					(*this, "Keyboard6",					"6",																			'6')
, Keyboard7					(*this, "Keyboard7",					"7",																			'7')
, Keyboard8					(*this, "Keyboard8",					"8",																			'8')
, Keyboard9					(*this, "Keyboard9",					"9",																			'9')
, KeyboardA					(*this, "KeyboardA",					"A",																			'a')
, KeyboardB					(*this, "KeyboardB",					"B",																			'b')
, KeyboardC					(*this, "KeyboardC",					"C",																			'c')
, KeyboardD					(*this, "KeyboardD",					"D",																			'd')
, KeyboardE					(*this, "KeyboardE",					"E",																			'e')
, KeyboardF					(*this, "KeyboardF",					"F",																			'f')
, KeyboardG					(*this, "KeyboardG",					"G",																			'g')
, KeyboardH					(*this, "KeyboardH",					"H",																			'h')
, KeyboardI					(*this, "KeyboardI",					"I",																			'i')
, KeyboardJ					(*this, "KeyboardJ",					"J",																			'j')
, KeyboardK					(*this, "KeyboardK",					"K",																			'k')
, KeyboardL					(*this, "KeyboardL",					"L",																			'l')
, KeyboardM					(*this, "KeyboardM",					"M",																			'm')
, KeyboardN					(*this, "KeyboardN",					"N",																			'n')
, KeyboardO					(*this, "KeyboardO",					"O",																			'o')
, KeyboardP					(*this, "KeyboardP",					"P",																			'p')
, KeyboardQ					(*this, "KeyboardQ",					"Q",																			'q')
, KeyboardR					(*this, "KeyboardR",					"R",																			'r')
, KeyboardS					(*this, "KeyboardS",					"S",																			's')
, KeyboardT					(*this, "KeyboardT",					"T",																			't')
, KeyboardU					(*this, "KeyboardU",					"U",																			'u')
, KeyboardV					(*this, "KeyboardV",					"V",																			'v')
, KeyboardW					(*this, "KeyboardW",					"W",																			'w')
, KeyboardX					(*this, "KeyboardX",					"X",																			'x')
, KeyboardY					(*this, "KeyboardY",					"Y",																			'y')
, KeyboardZ					(*this, "KeyboardZ",					"Z",																			'z')
, KeyboardNumpad0				(*this, "KeyboardNumpad0",				"Numpad 0",																		'0')
, KeyboardNumpad1				(*this, "KeyboardNumpad1",				"Numpad 1",																		'1')
, KeyboardNumpad2				(*this, "KeyboardNumpad2",				"Numpad 2",																		'2')
, KeyboardNumpad3				(*this, "KeyboardNumpad3",				"Numpad 3",																		'3')
, KeyboardNumpad4				(*this, "KeyboardNumpad4",				"Numpad 4",																		'4')
, KeyboardNumpad5				(*this, "KeyboardNumpad5",				"Numpad 5",																		'5')
, KeyboardNumpad6				(*this, "KeyboardNumpad6",				"Numpad 6",																		'6')
, KeyboardNumpad7				(*this, "KeyboardNumpad7",				"Numpad 7",																		'7')
, KeyboardNumpad8				(*this, "KeyboardNumpad8",				"Numpad 8",																		'8')
, KeyboardNumpad9				(*this, "KeyboardNumpad9",				"Numpad 9",																		'9')
, KeyboardNumpadMultiply		(*this, "KeyboardNumpadMultiply",		"Numpad multiply",																'*')
, KeyboardNumpadAdd			(*this, "KeyboardNumpadAdd",			"Numpad add",																	'+')
, KeyboardNumpadSeparator		(*this, "KeyboardNumpadSeparator",		"Numpad separator",																',')
, KeyboardNumpadSubtract		(*this, "KeyboardNumpadSubtract",		"Numpad subtract",																'-')
, KeyboardNumpadDecimal		(*this, "KeyboardNumpadDecimal",		"Numpad decimal",																'.')
, KeyboardNumpadDivide		(*this, "KeyboardNumpadDivide",			"Numpad divide",																'/')
, KeyboardF1					(*this, "KeyboardF1",					"F1",																			0x00)
, KeyboardF2					(*this, "KeyboardF2",					"F2",																			0x00)
, KeyboardF3					(*this, "KeyboardF3",					"F3",																			0x00)
, KeyboardF4					(*this, "KeyboardF4",					"F4",																			0x00)
, KeyboardF5					(*this, "KeyboardF5",					"F5",																			0x00)
, KeyboardF6					(*this, "KeyboardF6",					"F6",																			0x00)
, KeyboardF7					(*this, "KeyboardF7",					"F7",																			0x00)
, KeyboardF8					(*this, "KeyboardF8",					"F8",																			0x00)
, KeyboardF9					(*this, "KeyboardF9",					"F9",																			0x00)
, KeyboardF10					(*this, "KeyboardF10",					"F10",																			0x00)
, KeyboardF11					(*this, "KeyboardF11",					"F11",																			0x00)
, KeyboardF12					(*this, "KeyboardF12",					"F12",																			0x00)
, KeyboardNumLock				(*this, "KeyboardNumLock",				"Num lock",																		0x00)
, KeyboardScrollLock			(*this, "KeyboardScrollLock",			"Scroll lock",																	0x00)
, KeyboardCircumflex			(*this, "KeyboardCircumflex",			"Circumflex (^)",																0x00)
, KeyboardLeftWindows			(*this, "KeyboardLeftWindows",			"Left Windows key",																0x00)
, KeyboardRightWindows		(*this, "KeyboardRightWindows",			"Right Windows key",															0x00)
, KeyboardApplications		(*this, "KeyboardApplications",			"Applications key (natural keyboard)",											0x00)
, KeyboardF13					(*this, "KeyboardF13",					"F13",																			0x00)
, KeyboardF14					(*this, "KeyboardF14",					"F14",																			0x00)
, KeyboardF15					(*this, "KeyboardF15",					"F15",																			0x00)
, KeyboardF16					(*this, "KeyboardF16",					"F16",																			0x00)
, KeyboardF17					(*this, "KeyboardF17",					"F17",																			0x00)
, KeyboardF18					(*this, "KeyboardF18",					"F18",																			0x00)
, KeyboardF19					(*this, "KeyboardF19",					"F19",																			0x00)
, KeyboardF20					(*this, "KeyboardF20",					"F20",																			0x00)
, KeyboardF21					(*this, "KeyboardF21",					"F21",																			0x00)
, KeyboardF22					(*this, "KeyboardF22",					"F22",																			0x00)
, KeyboardF23					(*this, "KeyboardF23",					"F23",																			0x00)
, KeyboardF24					(*this, "KeyboardF24",					"F24",																			0x00)
, KeyboardLeftShift			(*this, "KeyboardLeftShift",			"Left shift",																	0x00)
, KeyboardRightShift			(*this, "KeyboardRightShift",			"Right shift",																	0x00)
, KeyboardLeftControl			(*this, "KeyboardLeftControl",			"Left control",																	0x00)
, KeyboardRightControl		(*this, "KeyboardRightControl",			"Right control",																0x00)
, KeyboardVolumeMute			(*this, "KeyboardVolumeMute",			"Volume mute",																	0x00)
, KeyboardVolumeDown			(*this, "KeyboardVolumeDown",			"Volume down",																	0x00)
, KeyboardVolumeUp			(*this, "KeyboardVolumeUp",				"Volume up",																	0x00)
, KeyboardMediaNextTrack		(*this, "KeyboardMediaNextTrack",		"Media next track",																0x00)
, KeyboardMediaPreviousTrack	(*this, "KeyboardMediaPreviousTrack",	"Media previous track",															0x00)
, KeyboardMediaStop			(*this, "KeyboardMediaStop",			"Media stop",																	0x00)
, KeyboardMediaPlayPause		(*this, "KeyboardMediaPlayPause",		"Media play pause",																0x00)
, KeyboardAdd					(*this, "KeyboardAdd",					"For any country/region, the '+' key",											 '+')
, KeyboardSeparator			(*this, "KeyboardSeparator",			"For any country/region, the ',' key",											 ',')
, KeyboardSubtract			(*this, "KeyboardSubtract",				"For any country/region, the '-' key",											 '-')
, KeyboardDecimal				(*this, "KeyboardDecimal",				"For any country/region, the '.' key",											 '.')
, KeyboardOEM1				(*this, "KeyboardOEM1",					"For the US standard keyboard, the ';:' key",									0x00)
, KeyboardOEM2				(*this, "KeyboardOEM2",					"For the US standard keyboard, the '/?' key",									0x00)
, KeyboardOEM3				(*this, "KeyboardOEM3",					"For the US standard keyboard, the '`~' key",									0x00)
, KeyboardOEM4				(*this, "KeyboardOEM4",					"For the US standard keyboard, the '[{' key",									0x00)
, KeyboardOEM5				(*this, "KeyboardOEM5",					"For the US standard keyboard, the 'backslash|' key",							0x00)
, KeyboardOEM6				(*this, "KeyboardOEM6",					"For the US standard keyboard, the ']}' key",									0x00)
, KeyboardOEM7				(*this, "KeyboardOEM7",					"For the US standard keyboard, the 'single-quote/double-quote' key",			0x00)
, KeyboardOEM8				(*this, "KeyboardOEM8",					"Used for miscellaneous characters; it can vary by keyboard",					0x00)
, KeyboardOEM102				(*this, "KeyboardOEM102",				"Either the angle bracket key or the backslash key on the RT 102-key keyboard",	0x00)
// Main character controls
, TransX						(*this, "TransX",						"X translation axis: Strafe left/right (+/-)")
, TransY						(*this, "TransY",						"Y translation axis: Move up/down (+/-)")
, TransZ						(*this, "TransZ",						"Z translation axis: Move forwards/backwards (+/-)")
, Pan							(*this, "Pan",							"Keep pressed to pan",															0x00)
, PanX						(*this, "PanX",							"X pan translation axis: Strafe left/right (+/-)")
, PanY						(*this, "PanY",							"Y pan translation axis: Move up/down (+/-)")
, PanZ						(*this, "PanZ",							"Z pan translation axis: Move forwards/backwards (+/-)")
, RotX						(*this, "RotX",							"X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)")
, RotY						(*this, "RotY",							"Y rotation axis: Yaw (also called 'heading') change is turning to the left or right")
, RotZ						(*this, "RotZ",							"Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down")
, Rotate						(*this, "Rotate",						"Keep pressed to rotate",														0x00)
, RotateSlow					(*this, "RotateSlow",					"Keep pressed to rotate slowly",												0x00)
, Forward						(*this, "Forward",						"Move forwards",																0x00)
, Backward					(*this, "Backward",						"Move backwards",																0x00)
, Left						(*this, "Left",							"Move (rotate) left",															0x00)
, Right						(*this, "Right",						"Move (rotate) right",															0x00)
, StrafeLeft					(*this, "StrafeLeft",					"Strafe left",																	0x00)
, StrafeRight					(*this, "StrafeRight",					"Strafe right",																	0x00)
, Up							(*this, "Up",							"Move up",																		0x00)
, Down						(*this, "Down",							"Move down",																	0x00)
, Run							(*this, "Run",							"Keep pressed to run",															0x00)
, Sneak						(*this, "Sneak",						"Keep pressed to sneak",														0x00)
, Crouch						(*this, "Crouch",						"Keep pressed to crouch",														0x00)
, Jump						(*this, "Jump",							"Jump",																			0x00)
, Zoom						(*this, "Zoom",							"Keep pressed to zoom",															0x00)
, ZoomAxis					(*this, "ZoomAxis",						"Zoom axis to zoom in or out (+/-)")
//, Button1						(*this, "Button1",						"Button for action #1",															0x00)
//, Button2						(*this, "Button2",						"Button for action #2",															0x00)
//, Button3						(*this, "Button3",						"Button for action #3",															0x00)
//, Button4						(*this, "Button4",						"Button for action #4",															0x00)
//, Button5						(*this, "Button5",						"Button for action #5",															0x00)
// Interaction
, Pickup						(*this, "Pickup",						"Keep pressed to pickup",														0x00)
, Throw						(*this, "Throw",						"Throw the picked object",														0x00)
, IncreaseForce				(*this, "IncreaseForce",				"Keep pressed to increase the force applied to the picked object",				0x00)
, DecreaseForce				(*this, "DecreaseForce",				"Keep pressed to decrease the force applied to the picked object",				0x00)
, PushPull					(*this, "PushPull",						"Used to push/pull the picked object") {
  connect_to_devices();
}

VirtualStandardController::~VirtualStandardController() {

}

void VirtualStandardController::connect_to_devices() {
  // Connect mouse
  connect_all(mInputManager.get_mouse(), "Mouse", "");

  // Connect keyboard
  connect_all(mInputManager.get_keyboard(), "Keyboard", "");

  // Connect character controls
  Vector<Device*> devices = mInputManager.get_devices();
  for (auto device : devices) {
    if (device->get_name() == "Mouse") {
      // Get mouse
      auto *mouse = dynamic_cast<Mouse*>(device);

      // Movement
      // Keep pressed to pan
      connect("Pan",			mouse->Middle);
      // RotX: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
      connect("RotX",			mouse->X);
      // RotY: Yaw (also called 'heading') change is turning to the left or right
      connect("RotY",			mouse->Y);
      // Keep pressed to rotate
      connect("Rotate",		mouse->Right);
      // Pan x
      connect("PanX",			mouse->X, -0.05f);
      // Pan y
      connect("PanY",			mouse->Y, -0.05f);

      // Zoom
      connect("Zoom",			mouse->Middle);
      connect("ZoomAxis",		mouse->Wheel, 0.01f);
      connect("ZoomAxis",		mouse->Y, -0.1f);

      // Buttons
      connect("MouseWheel",	mouse->Wheel);
      connect("Button1",		mouse->Left);
      connect("Button2",		mouse->Right);
      connect("Button3",		mouse->Middle);
      //connect("Button4",		mouse->Button4);
      //connect("Button5",		mouse->Button5);

      // Interaction
      connect("Pickup",		mouse->Middle);
      connect("PushPull",		mouse->Wheel, 0.001f);
    } else if (device->get_name() == "Keyboard") {
      // Get keyboard
      auto *keyboard = dynamic_cast<Keyboard*>(device);

      // Keep pressed to rotate slowly
      connect("RotateSlow",		keyboard->Q);

      // WASD
      connect("Forward",			keyboard->W);
      connect("Backward",			keyboard->S);
      connect("StrafeLeft",		keyboard->A);
      connect("StrafeRight",		keyboard->D);
      connect("Left",				keyboard->Q);
      connect("Right",			keyboard->E);

      // Cursor keys
      connect("Forward",			keyboard->Up);
      connect("Backward",			keyboard->Down);
      connect("StrafeLeft",		keyboard->Left);
      connect("StrafeRight",		keyboard->Right);
      connect("Left",				keyboard->Left);
      connect("Right",			keyboard->Right);

      // Look up/down
      connect("Up",				keyboard->PageUp);
      connect("Down",				keyboard->PageDown);

      // Run/sneak/crouch/jump
      connect("Run",				keyboard->Shift);
      connect("Sneak",			keyboard->Control);
      connect("Crouch",			keyboard->C);
      connect("Jump",				keyboard->Space);

      // Buttons
      connect("Button1",			keyboard->Space);
      connect("Button2",			keyboard->Return);
      connect("Button3",			keyboard->Backspace);
      connect("Button4",			keyboard->Alt);
      connect("Button5",			keyboard->Circumflex);

      // Interaction
      connect("Throw",			keyboard->T);
      connect("IncreaseForce",	keyboard->Shift);
      connect("DecreaseForce",	keyboard->Control);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core