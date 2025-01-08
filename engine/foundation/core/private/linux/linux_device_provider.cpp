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
#include "core/linux/linux_device_provider.h"
#include "core/linux/linux_event_device.h"
#include "core/linux/linux_keyboard_device.h"
#include "core/linux/linux_mouse_device.h"
#include "core/input/device/joystick.h"
#include "core/input/device/keyboard.h"
#include "core/input/device/mouse.h"
#include <dirent.h>
#include <fcntl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxDeviceProvider::LinuxDeviceProvider(InputServer &inputManager)
: DeviceProvider(inputManager) {

}


void LinuxDeviceProvider::query_devices() {
  // Create a mouse device, if not already present
  if (!check_device("Mouse")) {
    auto* impl = new LinuxMouseDevice();
    add_device("Mouse", new Mouse(mInputManager, "Mouse", impl));
  }

  // Create a keyboard, if not already present
  if (!check_device("Keyboard")) {
    auto* impl = new LinuxKeyboardDevice();
    add_device("Keyboard", new Keyboard(mInputManager, "Keyboard", impl));
  }

  // List devices in "/dev/input/event*"
  DIR *pDir = opendir("/dev/input");
  if (pDir) {
    int nDevice = 0;

    // Read first entry
    dirent *pEntry = readdir(pDir);
    while (pEntry) {
      // Check if filename is "eventX"
      String sFilename = pEntry->d_name;
      if (sFilename.substr(0, 5) == "event") {
        // Try to open the device
        int f = open(("/dev/input/" + sFilename).c_str(), O_RDWR | O_NONBLOCK);
        if (f > 0) {
          // Create device
          auto *pImpl = new LinuxEventDevice(f);
          String sName = String("Joystick") + core::to_string(nDevice);
          add_device(sName, new Joystick(this->mInputManager, sName, pImpl));
          nDevice++;
        }
      }

      // Read next entry
      pEntry = readdir(pDir);
    }

    // Be polite and close the directory after we're done...
    closedir(pDir);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core