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
#include "core/linux/linux_window.h"
#include "core/linux/linux_window_device.h"
#include "core/frontend/window_device.h"
#include "core/frontend/window.h"
#include "core/linux/xcb_includes.h"
#include "core/linux/xcb_helper.h"
#include <xcb/xproto.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxWindow::LinuxWindow(Window *window)
: WindowImpl(window)
, mXcbConnection(reinterpret_cast<LinuxWindowDevice*>(window->get_window_system()->get_impl())->get_xcb_connection())
, mRequestClosing(false) {

}

LinuxWindow::~LinuxWindow() {
  xcb_destroy_window(mXcbConnection, mXcbNativeWindow);
  xcb_disconnect(mXcbConnection);
  mXcbNativeWindow = 0;
}


handle LinuxWindow::get_native_window_handle() const {
  return mXcbNativeWindow;
}


bool LinuxWindow::close_requested() const {
  return mRequestClosing;
}


void LinuxWindow::create(const core::String &title, core::Recti shape, EWindowStyle windowStyle) {
  LinuxWindowDevice* lws = reinterpret_cast<LinuxWindowDevice*>(mWindow->get_window_system()->get_impl());

  mWindowStyle = windowStyle;
  mWindowBorders = {
    1.0f, 0.0f, 0.0f, 0.0f
  };

  uint32 screen_num;
  mXcbConnection = xcb_connect(nullptr, (int *)&screen_num);

  xcb_screen_iterator_t screen_iter;
  screen_iter = xcb_setup_roots_iterator(xcb_get_setup(mXcbConnection));
  for (; screen_iter.rem; --screen_num, xcb_screen_next(&screen_iter)) {
    if (screen_num == 0) {
      mXcbScreen = screen_iter.data;
      break;
    }
  }

  mXcbNativeWindow = xcb_generate_id(mXcbConnection);

  xcb_intern_atom_cookie_t *ewmh_cookies = xcb_ewmh_init_atoms(mXcbConnection, &mXcbEwmh);

  xcb_ewmh_init_atoms_replies(&mXcbEwmh, ewmh_cookies, nullptr);

  uint32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK;
  uint32 value_list[3] = { mXcbScreen->black_pixel, 1, XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW };

  mShape.x = shape.x;
  mShape.y = shape.y;
  mShape.w = shape.w - mWindowBorders.ResizeMargin;
  mShape.h = shape.h - mWindowBorders.ResizeMargin;

  xcb_create_window(mXcbConnection, XCB_COPY_FROM_PARENT, mXcbNativeWindow, mXcbScreen->root, mShape.x,
                    mShape.y, mShape.w, mShape.h, windowStyle == EWindowStyle::WindowStyle_NoBorder ? 0 : mWindowBorders.ResizeMargin,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, mXcbScreen->root_visual, value_mask, value_list);

  xcb_change_property(mXcbConnection, XCB_PROP_MODE_REPLACE, mXcbNativeWindow, XCB_ATOM_WM_NAME, XCB_ATOM_STRING,
                      8, title.size(), title.c_str());
  xcb_change_property(mXcbConnection, XCB_PROP_MODE_REPLACE, mXcbNativeWindow, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING,
                      8, title.size(), title.c_str());

  xcb_atom_t xdnd_version = 5;
  xcb_change_property(mXcbConnection, XCB_PROP_MODE_REPLACE, mXcbNativeWindow, lws->get_xcb_backend().get_atom(XDND_AWARE), XCB_ATOM_ATOM, 32, 1, &xdnd_version);

  xcb_atom_t wm_protocol_list[] = { lws->get_xcb_backend().get_atom(WM_DELETE_WINDOW) };

  xcb_icccm_set_wm_protocols(mXcbConnection, mXcbNativeWindow, mXcbEwmh.WM_PROTOCOLS, 1, wm_protocol_list);

  xcb_atom_t window_type[] = { mXcbEwmh._NET_WM_WINDOW_TYPE_NORMAL };
  xcb_ewmh_set_wm_window_type(&mXcbEwmh, mXcbNativeWindow, 1, window_type);

  uint32 margin = (uint32)mWindowBorders.ResizeMargin;
  xcb_configure_window(mXcbConnection, mXcbNativeWindow, XCB_CONFIG_WINDOW_BORDER_WIDTH, &margin);

  xcb_map_window(mXcbConnection, mXcbNativeWindow);

  xcb_flush(mXcbConnection);
}

void LinuxWindow::destroy() {
  xcb_destroy_window(mXcbConnection, mXcbNativeWindow);
}

void LinuxWindow::initialize() {

}

void LinuxWindow::set_shape(const Recti &shape) {
  mShape = shape;
  printf("Set shape: %d, %d, %d, %d\n", (uint32)shape.x, (uint32)shape.y, (uint32)shape.w, (uint32)shape.h);
  uint32 win_rect[] = { (uint32)shape.x, (uint32)shape.y, (uint32)shape.w, (uint32)shape.h };
  xcb_configure_window(mXcbConnection, mXcbNativeWindow,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, win_rect);
  xcb_flush(mXcbConnection);
}

void LinuxWindow::set_size(const Vec2i &size) {
  const uint32 values[] = { static_cast<uint32>(size.x), static_cast<uint32>(size.y) };
  xcb_configure_window(mXcbConnection, mXcbNativeWindow, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
  xcb_flush(mXcbConnection);
}

void LinuxWindow::set_position(const Vec2i &position) {
  const uint32 values[] = { static_cast<uint32>(position.x), static_cast<uint32>(position.y) };
  xcb_configure_window(mXcbConnection, mXcbNativeWindow, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
  xcb_flush(mXcbConnection);
}

FrontendPlatformData LinuxWindow::get_platform_data() const {
  FrontendPlatformData data{};

  data.os_linux.connection = mXcbConnection;
  data.os_linux.window = mXcbNativeWindow;

  return data;
}

void LinuxWindow::update() {
  mXcbEvent = !mWaitEvent ? xcb_poll_for_event(mXcbConnection) : mXcbEvent;
  LinuxWindowDevice* lws = reinterpret_cast<LinuxWindowDevice*>(mWindow->get_window_system()->get_impl());

  while (mXcbEvent) {
    xcb_generic_event_t *next_event = nullptr;
    uint8_t ev_type = mXcbEvent->response_type & ~0x80;
    switch (ev_type) {
      case XCB_EXPOSE: {
        /// ??? required ???
      }
      case XCB_CLIENT_MESSAGE: {
        const xcb_client_message_event_t *msg_ev = (const xcb_client_message_event_t *) mXcbEvent;
        if (msg_ev->data.data32[0] == lws->get_xcb_backend().get_atom(WM_DELETE_WINDOW)) {
          mRequestClosing = true;
        }
        else if (msg_ev->type == lws->get_xcb_backend().get_atom(XDND_ENTER)) {
          // We entered window, we have focus now
          mWindowStatus.HasFocus = true;
          mWindowStatus.IsUnderMouse = true;
          for (uint32 i = 2; i < 5; ++i) {
            if (msg_ev->data.data32[i] == lws->get_xcb_backend().get_atom(URI_LIST))
              lws->set_support_uri_list(true);
          }

          xcb_get_property_cookie_t prop_cookie = xcb_get_property(mXcbConnection, 0, msg_ev->data.data32[0], lws->get_xcb_backend().get_atom(XDND_TYPE_LIST), XCB_ATOM_ATOM, 0, 100);
          xcb_get_property_reply_t *prop_reply = xcb_get_property_reply(mXcbConnection, prop_cookie, nullptr);
          if (prop_reply && prop_reply->format == 8 * sizeof(xcb_atom_t)) {
            uint32 length = xcb_get_property_value_length(prop_reply) / 4;
            if (length > 0 && length < 100) {
              xcb_atom_t *list = static_cast<xcb_atom_t *>(xcb_get_property_value(prop_reply));
              for (uint32 i = 0; i < length; ++i) {
                if (list[i] == lws->get_xcb_backend().get_atom(URI_LIST))
                  lws->set_support_uri_list(true);
              }
            }
            free(prop_reply);
          }
        }
        else if (msg_ev->type == lws->get_xcb_backend().get_atom(XDND_POSITION)) {
          if (lws->support_uri_list()) {
            core::int32 xdnd_x = (int32)(msg_ev->data.data32[2] >> 16);
            core::int32 xdnd_y = (int32)(msg_ev->data.data32[2] & 0xFFFF);
            lws->set_dnd_position(xdnd_x, xdnd_y);
            xcb_client_message_event_t status = { 0 };
            status.response_type = XCB_CLIENT_MESSAGE;
            status.window = msg_ev->data.data32[0];
            status.format = 32;
            status.type = lws->get_xcb_backend().get_atom(XDND_STATUS);
            status.data.data32[0] = mXcbNativeWindow;
            status.data.data32[1] = 3;
            status.data.data32[2] = 0;
            status.data.data32[3] = 0;
            status.data.data32[4] = lws->get_xcb_backend().get_atom(XDND_ACTION_COPY);
            xcb_send_event(mXcbConnection, 1, msg_ev->data.data32[0], XCB_EVENT_MASK_NO_EVENT, (char *)&status);
            xcb_flush(lws->get_xcb_connection());
          }
        }
        else if (msg_ev->type == lws->get_xcb_backend().get_atom(XDND_DROP)) {
          if (lws->support_uri_list()) {
            xcb_convert_selection(mXcbConnection, mXcbNativeWindow, lws->get_xcb_backend().get_atom(XDND_SELECTION), lws->get_xcb_backend().get_atom(URI_LIST), lws->get_xcb_backend().get_atom(XDND_PROP), msg_ev->data.data32[3]);
            xcb_translate_coordinates_cookie_t trans_cookie = xcb_translate_coordinates(
              mXcbConnection,
              mXcbScreen->root,
              mXcbNativeWindow,
              lws->get_dnd_x(), lws->get_dnd_y());
            xcb_translate_coordinates_reply_t *trans_reply = xcb_translate_coordinates_reply(mXcbConnection, trans_cookie,
                                                                                             nullptr);
            xcb_client_message_event_t finished = { 0 };
            finished.response_type = XCB_CLIENT_MESSAGE;
            finished.window = msg_ev ? msg_ev->data.data32[0] : 0;
            finished.format = 32;
            finished.type = lws->get_xcb_backend().get_atom(XDND_FINISHED);
            finished.data.data32[0] = mXcbNativeWindow;
            finished.data.data32[1] = 1;
            finished.data.data32[2] = lws->get_xcb_backend().get_atom(XDND_ACTION_COPY);
            xcb_send_event(mXcbConnection, 1, msg_ev ? msg_ev->data.data32[0] : 0, XCB_EVENT_MASK_NO_EVENT, (char *)&finished);
            if (trans_reply) {
              lws->set_dnd_position(trans_reply->dst_x, trans_reply->dst_y);
              free(trans_reply);
            }
          }
        }
      }
        break;
      case XCB_SELECTION_NOTIFY:
      {
        xcb_get_property_cookie_t prop_cookie = xcb_icccm_get_text_property(mXcbConnection, mXcbNativeWindow, lws->get_xcb_backend().get_atom(XDND_PROP));
        xcb_icccm_get_text_property_reply_t reply;

        if (xcb_icccm_get_text_property_reply(mXcbConnection, prop_cookie, &reply, nullptr)) {
          char *s = reply.name;
          char *next = "file:";
          char *n = next;
          bool correct = true;
          while (s && correct && (uint32)(s - reply.name + 1) < reply.name_len) {
            correct = *s == *n;
            n++;
            s++;
            if (*n == '\0') {
              if (*s == '/' && *(s + 1) == '/') {
                s = strchr(s + 2, '/');
              }

              if (!s)
                break;
              char *cr = strchr(s, '\r');
              char *lf = strchr(s, '\n');
              if (!cr || !lf)
                break;
              *cr = '\0';
              char *filename = new char[strlen(s) + 1];
              // percent decoding
              char *percent = strchr(s, '%');
              uint32 offOrg = 0;
              uint32 offNew = 0;
              while (percent) {
                sizeT length = percent - (s + offOrg);
                memcpy(filename + offNew, s + offOrg, length);
                char code[3] = { percent[1], percent[2], '\0' };
                char ch = (char)strtol(code, nullptr, 16);
                *(filename + offNew + length) = ch;
                offNew += length + 1;
                offOrg += length + 3;
                percent = strchr(s + offOrg, '%');
              }
              strcpy(filename + offNew, s + offOrg);

              /// TODO(naetherm): Implement this
              printf(">> Working with dropped file %s\n", filename);

              s = lf + 1;
              n = next;
            }
          }

          xcb_delete_property(mXcbConnection, mXcbNativeWindow, lws->get_xcb_backend().get_atom(XDND_PROP));
        }
      }
        break;
      case XCB_FOCUS_IN:
        mWindowStatus.HasFocus = true;
        mWindow->on_enter();
        break;
      case XCB_FOCUS_OUT:
        mWindowStatus.HasFocus = false;
        mWindow->on_leave();
        break;
      case XCB_CONFIGURE_NOTIFY:
      {
        mWindowStatus.HasFocus = true;
        xcb_set_input_focus(mXcbConnection, XCB_INPUT_FOCUS_POINTER_ROOT, mXcbNativeWindow, XCB_TIME_CURRENT_TIME);
        const xcb_configure_notify_event_t *cfg_ev = (const xcb_configure_notify_event_t *) mXcbEvent;
        mShape = Recti(cfg_ev->x, cfg_ev->y, cfg_ev->width, cfg_ev->height);

        xcb_window_t root = XCB_WINDOW_NONE;

        xcb_query_tree_cookie_t query_tree_cookie = xcb_query_tree(mXcbConnection, mXcbNativeWindow);
        xcb_query_tree_reply_t *query_reply = xcb_query_tree_reply(mXcbConnection, query_tree_cookie, nullptr);
        if (query_reply) {
          root = query_reply->root;
          free(query_reply);
        }

        if (root != XCB_WINDOW_NONE) {
          xcb_translate_coordinates_cookie_t trans_cookie = xcb_translate_coordinates(mXcbConnection, mXcbNativeWindow,
                                                                                      root, 0, 0);
          xcb_translate_coordinates_reply_t *trans_reply = xcb_translate_coordinates_reply(mXcbConnection, trans_cookie,
                                                                                           nullptr);
          if (trans_reply) {
            mShape.x = trans_reply->dst_x;
            mShape.y = trans_reply->dst_y;
            free(trans_reply);
          }
        }

        mWindow->on_move(mShape.x, mShape.y);
        mWindow->on_size(mShape.w, mShape.h);
      }
        break;
      case XCB_LEAVE_NOTIFY:
        mWindowStatus.IsUnderMouse = false;
        mWindow->on_leave();
        break;
      case XCB_ENTER_NOTIFY:
        mWindowStatus.IsUnderMouse = true;
        mWindow->on_enter();
        break;
      case XCB_MOTION_NOTIFY:
      {
        const xcb_motion_notify_event_t *motion_ev = (const xcb_motion_notify_event_t *)mXcbEvent;

        // For the move to work we also need the previous mouse position information
        mWindow->on_mouse_position(motion_ev->root_x - mShape.x, motion_ev->root_y - mShape.y);
      }
        break;
      case XCB_BUTTON_PRESS:
      {
        const xcb_button_press_event_t *press = (const xcb_button_press_event_t *)mXcbEvent;

        if (press->detail == XCB_BUTTON_INDEX_4 || press->detail == XCB_BUTTON_INDEX_5) {
          float delta = press->detail == XCB_BUTTON_INDEX_4 ? 1.f : -1.f;

          mWindow->on_mouse_wheel(delta);

          break;
        }

        mWindow->on_mouse_button_down(XcbHelper::convert_xcb_to_mouse(press->detail));
      }
        break;
      case XCB_BUTTON_RELEASE:
      {
        const xcb_button_release_event_t *release = (const xcb_button_release_event_t *)mXcbEvent;

        if (release->detail == XCB_BUTTON_INDEX_1 && lws->is_dragging()) {
          /// TODO(naetherm): Restore old cursor: frontend()->get_cursor_system()->restore_last_cursor()

          xcb_ewmh_request_wm_moveresize(&mXcbEwmh, 0, mXcbNativeWindow, release->root_x, release->root_y, XCB_EWMH_WM_MOVERESIZE_CANCEL, XCB_BUTTON_INDEX_1, XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL);
          xcb_flush(mXcbConnection);

          lws->set_dragging(false);
          lws->set_dragging_begins(false);
        }

        if (release->detail == XCB_BUTTON_INDEX_4 || release->detail == XCB_BUTTON_INDEX_5) {
          float delta = release->detail == XCB_BUTTON_INDEX_4 ? 1.f : -1.f;

          mWindow->on_mouse_wheel(delta);

          break;
        }

        /// TODO(naetherm):
        mWindow->on_mouse_button_up(XcbHelper::convert_xcb_to_mouse(release->detail));
      }
        break;
      case XCB_KEY_PRESS:
      case XCB_KEY_RELEASE:
      {
        if (ev_type == XCB_KEY_RELEASE) {
          next_event = xcb_poll_for_queued_event(mXcbConnection);
          if (next_event && (next_event->response_type & ~0x80) == XCB_KEY_PRESS) {
            xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)mXcbEvent;
            xcb_key_press_event_t *next_key_event = (xcb_key_press_event_t *)next_event;
            if (next_key_event->time == key_event->time && next_key_event->detail == key_event->detail) {
              mXcbEvent->response_type = XCB_KEY_PRESS;
              free(next_event);
              next_event = nullptr;
            }

            xcb_keysym_t keysym = xkb_state_key_get_one_sym(lws->get_xcb_backend().get_xkb_state(), key_event->detail);
            mWindow->on_key_release(XcbHelper::convert_xcb_to_keysym(keysym));
          }
        } else {
          xcb_key_release_event_t *key_event = (xcb_key_release_event_t *)mXcbEvent;

          xcb_keysym_t keysym = xkb_state_key_get_one_sym(lws->get_xcb_backend().get_xkb_state(), key_event->detail);
          mWindow->on_key_press(XcbHelper::convert_xcb_to_keysym(keysym));
        }
      }
        break;
      default:
        break;
    }

    free(mXcbEvent);
    if (next_event) {
      mXcbEvent = next_event;
      next_event = nullptr;
    } else {
      mXcbEvent = xcb_poll_for_event(mXcbConnection);
    }
  }

  if (lws->is_dragging()) {
    xcb_query_pointer_cookie_t cookie = xcb_query_pointer(mXcbConnection, mXcbNativeWindow);
    xcb_query_pointer_reply_t *reply = xcb_query_pointer_reply(mXcbConnection, cookie, nullptr);
    if (reply) {
      if (!(reply->mask & XCB_BUTTON_MASK_1)) {
        /// TODO(naetherm): get_cursor_system()->restore_last_cursor();
        lws->set_dragging(false);
        lws->set_dragging_begins(false);
      }
    }
  }

  xcb_get_property_cookie_t cookie = xcb_ewmh_get_wm_state(&mXcbEwmh, mXcbNativeWindow);
  xcb_ewmh_get_atoms_reply_t reply;
  bool maximized_horz = false;
  bool maximized_vert = false;
  mWindowStatus.IsHidden = false;
  mWindowStatus.IsMinimized = false;
  if (xcb_ewmh_get_wm_state_reply(&mXcbEwmh, cookie, &reply, nullptr)) {
    for (uint32 i = 0; i < reply.atoms_len; ++i) {
      if (reply.atoms[i] == mXcbEwmh._NET_WM_STATE_MAXIMIZED_HORZ) {
        maximized_horz = true;
        mWindowStatus.WasMaximizedBefore = false;
      }

      if (reply.atoms[i] == mXcbEwmh._NET_WM_STATE_MAXIMIZED_VERT) {
        maximized_vert = true;
        mWindowStatus.WasMaximizedBefore = false;
      }

      if (reply.atoms[i] == mXcbEwmh._NET_WM_STATE_HIDDEN) {
        mWindowStatus.IsHidden = true;
        mWindowStatus.IsMinimized = true;
      }
    }
  }

  mWindowStatus.IsMaximized = maximized_horz && maximized_vert;

  mWindowStatus.WasMaximizedBefore = mWindowStatus.IsMinimized && mWindowStatus.IsMaximized;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core
