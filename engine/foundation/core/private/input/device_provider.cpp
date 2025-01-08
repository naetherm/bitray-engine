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
#include "core/input/device_provider.h"
#include "core/input/input_server.h"
#include "core/input/device/device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


DeviceProvider::~DeviceProvider() {
  // Clean up
  clear();
}

void DeviceProvider::detect_devices(bool reset) {
  // Delete all devices?
  if (reset)
    clear();

  // Flag all current devices as 'not confirmed'
  for (uint32 i=0; i<mDevices.size(); ++i)
    mDevices[i]->mConfirmed = false;

  // Detect new devices (the ones that are already there will be ignored by add_device)
  query_devices();

  // Delete all devices that are no longer there (confirmed == false)
  for (uint32 i=0; i<mDevices.size(); ++i) {
    Device *pDevice = mDevices[i];
    if (!pDevice->mConfirmed) {
      // Remove device
      mInputManager.remove_device(pDevice);
      mDevices.erase(core::find(mDevices.cbegin(), mDevices.cend(), pDevice));
      delete pDevice;
      --i;
    }
  }
}

const Vector<Device *> &DeviceProvider::get_devices() const {
  return mDevices;
}

DeviceProvider::DeviceProvider(InputServer &inputManager)
: mInputManager(inputManager) {

}

void DeviceProvider::clear() {
  // Delete all input devices
  for (uint32 i=0; i<mDevices.size(); ++i)
  {
    Device *pDevice = mDevices[i];
    mInputManager.remove_device(pDevice);
    delete pDevice;
  }
  mDevices.clear();
}

bool DeviceProvider::check_device(const String &name) {
  // Check if the device is already present
  Device* device = mInputManager.get_device(name);

  if (device) {
    // Update device
    device->mConfirmed = true;
    return true;
  }

  // No such device found
  return false;
}

bool DeviceProvider::add_device(const String &name, Device *device) {
  // Check if the device is already present
  Device* foundDevice = mInputManager.get_device(name);
  if (!foundDevice) {
    // Add device to manager
    if (mInputManager.add_device(device)) {
      // Add device to own list - if we're in here, we now that the pDevice pointer is valid
      mDevices.push_back(device);
      device->mConfirmed = true;
      return true;
    }
  }

  // Error!
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core