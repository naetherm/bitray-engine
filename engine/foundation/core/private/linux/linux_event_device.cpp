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
#include "core/linux/linux_event_device.h"
#include "core/input/control/axis.h"
#include "core/input/control/button.h"
#include "core/input/device/device.h"
#include <unistd.h>
#include <linux/input.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxEventDevice::LinuxEventDevice(int fileHandle)
: mFile(fileHandle)
, mBusType(0)
, mVendor(0)
, mProduct(0)
, mVersion(0) {
  // Do not destroy automatically
  mDelete = false;

  input_id deviceInfo;
  if (!ioctl(mFile, EVIOCGID, &deviceInfo)) {
    // Save device info
    mBusType = deviceInfo.bustype;
    mVendor  = deviceInfo.vendor;
    mProduct = deviceInfo.product;
    mVersion = deviceInfo.version;
  }
}

LinuxEventDevice::~LinuxEventDevice() {
  close(mFile);
}

void LinuxEventDevice::update() {
  // Read input events
  struct input_event pEvents[64];
  ssize_t nSize = read(mFile, pEvents, sizeof(struct input_event)*64);
  int nEvents = (nSize > 0 ? static_cast<int>(nSize / sizeof(struct input_event)) : 0);
  for (int i=0; i<nEvents; i++) {
    // get corresponding control
    Control *pControl = nullptr;
    if (pEvents[i].code == ABS_X)
      pControl = mDevice->get_control("X");
    else if (pEvents[i].code == ABS_Y)
      pControl = mDevice->get_control("Y");
    else if (pEvents[i].code == ABS_Z)
      pControl = mDevice->get_control("Z");
    else if (pEvents[i].code == ABS_RX)
      pControl = mDevice->get_control("RX");
    else if (pEvents[i].code == ABS_RY)
      pControl = mDevice->get_control("RY");
    else if (pEvents[i].code == ABS_RZ)
      pControl = mDevice->get_control("RZ");
    else if (pEvents[i].code == ABS_HAT0X)
      pControl = mDevice->get_control("Hat");
    else if (pEvents[i].code >= BTN_JOYSTICK && pEvents[i].code < BTN_GAMEPAD)
      pControl = mDevice->get_control(core::String("Button") + core::to_string(pEvents[i].code - BTN_JOYSTICK));
    else if (pEvents[i].code >= BTN_GAMEPAD && pEvents[i].code < BTN_DIGI)
      pControl = mDevice->get_control(core::String("Button") + core::to_string(pEvents[i].code - BTN_GAMEPAD));

    // get control type
    Axis   *pAxis   = (pControl != nullptr && pControl->get_control_type() == ControlType::ControlType_Axis) ? dynamic_cast<Axis*>  (pControl) : nullptr;
    Button *pButton = (pControl != nullptr && pControl->get_control_type() == ControlType::ControlType_Button) ? dynamic_cast<Button*>(pControl) : nullptr;

    // Set control value
    if (pEvents[i].type == EV_KEY && pButton)
    {
      // Button
      pButton->set_pressed(pEvents[i].value != 0);
    }
    else if ((pEvents[i].type == EV_ABS || pEvents[i].type == EV_REL) && pAxis)
    {
      // Absolute or relative axis
      // get minimum and maximum values
      input_absinfo absInfo;
      if (!ioctl(mFile, EVIOCGABS(0), &absInfo))
      {
        // get value in a range of -1.0 - +1.0
        float fValue = (static_cast<float>(pEvents[i].value - absInfo.minimum) / static_cast<float>(absInfo.maximum - absInfo.minimum)) * 2.0f - 1.0f;
        if (fValue >  1.0f)
          fValue =  1.0f;
        if (fValue < -1.0f)
          fValue = -1.0f;
        pAxis->set_value(fValue, false);
      }
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core