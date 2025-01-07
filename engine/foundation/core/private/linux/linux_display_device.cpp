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
#include "core/linux/linux_display_device.h"
#include "core/linux/xcb_includes.h"
#include "core/frontend/display.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


#define RE_DEFAULT_SCREEN_DPI 96.0f


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxDisplayDevice::LinuxDisplayDevice() {

}

LinuxDisplayDevice::~LinuxDisplayDevice() {

}

void LinuxDisplayDevice::enumerate_displays(Vector<Display*>& displays) {

  displays.clear();

  xcb_connection_t *dummy_connection = xcb_connect(nullptr, nullptr);

  xcb_screen_t *display = xcb_setup_roots_iterator(xcb_get_setup(dummy_connection)).data;

  xcb_window_t dummy_window = xcb_generate_id(dummy_connection);
  xcb_create_window(dummy_connection, 0, dummy_window, display->root, 0, 0, 1, 1, 0, 0, 0, 0, 0);

  xcb_flush(dummy_connection);

  xcb_randr_get_monitors_cookie_t mon_cookie = xcb_randr_get_monitors(dummy_connection, dummy_window, 1);

  uint16_t current_width_pixels = display->width_in_pixels;
  uint16_t current_width_mm = display->width_in_millimeters;
  float current_dpi = 25.4f * (float)current_width_pixels / (float)current_width_mm;

  xcb_randr_get_monitors_reply_t *mon_reply = xcb_randr_get_monitors_reply(dummy_connection, mon_cookie, nullptr);
  xcb_randr_monitor_info_iterator_t mon_it = xcb_randr_get_monitors_monitors_iterator(mon_reply);

  xcb_xrm_database_t *database = xcb_xrm_database_from_default(dummy_connection);

  if (database) {
    char *value;
    if (xcb_xrm_resource_get_string(database, "Xft.dpi", nullptr, &value) >= 0) {
      current_dpi = atof(value);
      free(value);
    }
  }

  xcb_xrm_database_free(database);

  for (uint32 i = 0; i < mon_reply->nMonitors; ++i) {
    auto *_display = new Display();
    _display->set_shape(Recti(mon_it.data->x, mon_it.data->y, mon_it.data->width, mon_it.data->height));

    _display->set_dpi_factor(current_dpi / RE_DEFAULT_SCREEN_DPI);
    xcb_randr_monitor_info_next(&mon_it);
    displays.push_back(_display);
  }

  free(mon_reply);

  xcb_disconnect(dummy_connection);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core