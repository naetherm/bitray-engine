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
#include "core/input/device/mouse.h"
#include "core/input/device/device_impl.h"
#include "core/input/device/update_device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


Mouse::Mouse(InputServer &inputManager, const String &name, DeviceImpl *impl)
: Device(inputManager, name, "Mouse input controller", impl)
, X		(*this, "X",		"X axis (movement data, no absolute data)")
, Y		(*this, "Y",		"Y axis (movement data, no absolute data)")
, Wheel	(*this, "Wheel",	"Mouse wheel (movement data, no absolute data)")
, Left	(*this, "Left",		"Left mouse button (mouse button #0)")
, Right	(*this, "Right",	"Right mouse button (mouse button #1)")
, Middle	(*this, "Middle",	"Middle mouse button (mouse button #2)") {

}

Mouse::~Mouse() {

}

void Mouse::update() {
  // Update device
  if (mDeviceImpl && mDeviceImpl->get_device_backend_type() == DeviceBackendType::DeviceBackendType_UpdateDevice) {
    reinterpret_cast<UpdateDevice*>(mDeviceImpl)->update();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core