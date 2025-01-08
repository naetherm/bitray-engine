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
#include "core/linux/linux_keyboard_device.h"
#include "core/input/control/button.h"
#include "core/input/device/keyboard.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxKeyboardDevice::LinuxKeyboardDevice()
: mXcbReply(nullptr) {

}

LinuxKeyboardDevice::~LinuxKeyboardDevice() {

}

void LinuxKeyboardDevice::update() {
  if (mDevice) {
    Keyboard* keyboard = reinterpret_cast<Keyboard*>(mDevice);

    // Get keyboard state
    mXcbQueryKeymapCookie = xcb_query_keymap(XcbBackend::instance().get_xcb_connection());
    mXcbReply = xcb_query_keymap_reply(XcbBackend::instance().get_xcb_connection(), mXcbQueryKeymapCookie, nullptr);

    // Copy state
    Memory::copy(mKeys, mXcbReply->keys, sizeof(char)*32);

    // Find changed keys
    for (int32 i = 0; i < 32*8; ++i) {
      const int32 state = (mKeys[i/8] >> (i%8)) & 1;

      // Get button
      xcb_keysym_t keysym = xkb_state_key_get_one_sym(XcbBackend::instance().get_xkb_state(), i);

      Button *pButton = get_keyboard_key(keyboard, keysym);
      if (pButton) {
        // Get button state
        const bool bPressed = (state != 0);

        // Propagate changes
        if (pButton->is_pressed() != bPressed)
          pButton->set_pressed(bPressed);
      }
    }
  }
}

Button* LinuxKeyboardDevice::get_keyboard_key(Keyboard* keyboard, KeySym keySym) {
  switch (keySym) {
    case XK_BackSpace:		return &keyboard->Backspace;
    case XK_Tab:			return &keyboard->Tab;
    case XK_Clear:			return &keyboard->Clear;
    case XK_Shift_L:		return &keyboard->Shift;
    case XK_Control_L:		return &keyboard->Control;
    case XK_Menu:			return &keyboard->Alt;
    case XK_Pause:			return &keyboard->Pause;
    case XK_Caps_Lock:		return &keyboard->CapsLock;
    case XK_Escape:			return &keyboard->Escape;
    case XK_space:			return &keyboard->Space;
    case XK_Prior:			return &keyboard->PageUp;
    case XK_Next:			return &keyboard->PageDown;
    case XK_End:			return &keyboard->End;
    case XK_Home:			return &keyboard->Home;
    case XK_Left:			return &keyboard->Left;
    case XK_Up:				return &keyboard->Up;
    case XK_Right:			return &keyboard->Right;
    case XK_Down:			return &keyboard->Down;
    case XK_Select:			return &keyboard->Select;
    case XK_Execute:		return &keyboard->Execute;
    case XK_Print:			return &keyboard->Print;
    case XK_Insert:			return &keyboard->Insert;
    case XK_Delete:			return &keyboard->Delete;
    case XK_Help:			return &keyboard->Help;
    case XK_0:				return &keyboard->Key0;
    case XK_1:				return &keyboard->Key1;
    case XK_2:				return &keyboard->Key2;
    case XK_3:				return &keyboard->Key3;
    case XK_4:				return &keyboard->Key4;
    case XK_5:				return &keyboard->Key5;
    case XK_6:				return &keyboard->Key6;
    case XK_7:				return &keyboard->Key7;
    case XK_8:				return &keyboard->Key8;
    case XK_9:				return &keyboard->Key9;
    case XK_a:				return &keyboard->A;
    case XK_b:				return &keyboard->B;
    case XK_c:				return &keyboard->C;
    case XK_d:				return &keyboard->D;
    case XK_e:				return &keyboard->E;
    case XK_f:				return &keyboard->F;
    case XK_g:				return &keyboard->G;
    case XK_h:				return &keyboard->H;
    case XK_i:				return &keyboard->I;
    case XK_j:				return &keyboard->J;
    case XK_k:				return &keyboard->K;
    case XK_l:				return &keyboard->L;
    case XK_m:				return &keyboard->M;
    case XK_n:				return &keyboard->N;
    case XK_o:				return &keyboard->O;
    case XK_p:				return &keyboard->P;
    case XK_q:				return &keyboard->Q;
    case XK_r:				return &keyboard->R;
    case XK_s:				return &keyboard->S;
    case XK_t:				return &keyboard->T;
    case XK_u:				return &keyboard->U;
    case XK_v:				return &keyboard->V;
    case XK_w:				return &keyboard->W;
    case XK_x:				return &keyboard->X;
    case XK_y:				return &keyboard->Y;
    case XK_z:				return &keyboard->Z;
    case XK_KP_0:			return &keyboard->Numpad0;
    case XK_KP_1:			return &keyboard->Numpad1;
    case XK_KP_2:			return &keyboard->Numpad2;
    case XK_KP_3:			return &keyboard->Numpad3;
    case XK_KP_4:			return &keyboard->Numpad4;
    case XK_KP_5:			return &keyboard->Numpad5;
    case XK_KP_6:			return &keyboard->Numpad6;
    case XK_KP_7:			return &keyboard->Numpad7;
    case XK_KP_8:			return &keyboard->Numpad8;
    case XK_KP_9:			return &keyboard->Numpad9;
    case XK_KP_Multiply:	return &keyboard->NumpadMultiply;
    case XK_KP_Add:			return &keyboard->NumpadAdd;
    case XK_KP_Separator:	return &keyboard->NumpadSeparator;
    case XK_KP_Subtract:	return &keyboard->NumpadSubtract;
    case XK_KP_Decimal:		return &keyboard->NumpadDecimal;
    case XK_KP_Divide:		return &keyboard->NumpadDivide;
    case XK_F1:				return &keyboard->F1;
    case XK_F2:				return &keyboard->F2;
    case XK_F3:				return &keyboard->F3;
    case XK_F4:				return &keyboard->F4;
    case XK_F5:				return &keyboard->F5;
    case XK_F6:				return &keyboard->F6;
    case XK_F7:				return &keyboard->F7;
    case XK_F8:				return &keyboard->F8;
    case XK_F9:				return &keyboard->F9;
    case XK_F10:			return &keyboard->F10;
    case XK_F11:			return &keyboard->F11;
    case XK_F12:			return &keyboard->F12;
    case XK_Num_Lock:		return &keyboard->NumLock;
    case XK_Scroll_Lock:	return &keyboard->ScrollLock;
    case XK_asciicircum:	return &keyboard->Circumflex;
    default:				return  nullptr;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core