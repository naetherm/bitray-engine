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
#include "core/linux/xcb_helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
 InputKeyboardItem XcbHelper::convert_xcb_to_keysym(xcb_keysym_t key) {
  switch(key) {
    case XK_A: case XK_a: return InputKeyboardItem_A;
    case XK_B: case XK_b: return InputKeyboardItem_B;
    case XK_C: case XK_c: return InputKeyboardItem_C;
    case XK_D: case XK_d: return InputKeyboardItem_D;
    case XK_E: case XK_e: return InputKeyboardItem_E;
    case XK_F: case XK_f: return InputKeyboardItem_F;
    case XK_G: case XK_g: return InputKeyboardItem_G;
    case XK_H: case XK_h: return InputKeyboardItem_H;
    case XK_I: case XK_i: return InputKeyboardItem_I;
    case XK_J: case XK_j: return InputKeyboardItem_J;
    case XK_K: case XK_k: return InputKeyboardItem_K;
    case XK_L: case XK_l: return InputKeyboardItem_L;
    case XK_M: case XK_m: return InputKeyboardItem_M;
    case XK_N: case XK_n: return InputKeyboardItem_N;
    case XK_O: case XK_o: return InputKeyboardItem_O;
    case XK_P: case XK_p: return InputKeyboardItem_P;
    case XK_Q: case XK_q: return InputKeyboardItem_Q;
    case XK_R: case XK_r: return InputKeyboardItem_R;
    case XK_S: case XK_s: return InputKeyboardItem_S;
    case XK_T: case XK_t: return InputKeyboardItem_T;
    case XK_U: case XK_u: return InputKeyboardItem_U;
    case XK_V: case XK_v: return InputKeyboardItem_V;
    case XK_W: case XK_w: return InputKeyboardItem_W;
    case XK_X: case XK_x: return InputKeyboardItem_X;
    case XK_Y: case XK_y: return InputKeyboardItem_Y;
    case XK_Z: case XK_z: return InputKeyboardItem_Z;
    case XK_0: return InputKeyboardItem_0;
    case XK_1: return InputKeyboardItem_1;
    case XK_2: return InputKeyboardItem_2;
    case XK_3: return InputKeyboardItem_3;
    case XK_4: return InputKeyboardItem_4;
    case XK_5: return InputKeyboardItem_5;
    case XK_6: return InputKeyboardItem_6;
    case XK_7: return InputKeyboardItem_7;
    case XK_8: return InputKeyboardItem_8;
    case XK_9: return InputKeyboardItem_9;
    default: return InputKeyboardItem_None;
  }
}

InputMouseItem XcbHelper::convert_xcb_to_mouse(xcb_button_t button) {
  switch(button) {
    case XCB_BUTTON_INDEX_1: return InputMouseItem_ButtonLeft;
    case XCB_BUTTON_INDEX_2: return InputMouseItem_ButtonMiddle;
    case XCB_BUTTON_INDEX_3: return InputMouseItem_ButtonRight;
    default: return InputMouseItem_None;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core