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
#include "core/input/input_server.h"
#include "core/input/controller.h"
#if defined(LINUX)
#include "core/linux/linux_device_provider.h"
#elif defined(WIN32)

#endif
#include "core/platform/mutex.h"
#include "core/threading/lock_guard.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


InputServer &InputServer::instance() {
  static InputServer SInstance;
  return SInstance;
}

InputServer::InputServer()
: mMutex(new Mutex()) {
  // We are starting, let's see if we can find any device
  detect_devices();
}

InputServer::~InputServer() {
  // Clear everything
  clear();

  // Delete the mutex again
  delete mMutex;
}

void InputServer::update() {
  // Update all controls
  Vector<Control*> updatedControls;
  {
    LockGuard<Mutex> mutexLock(*mMutex);

    // Copy list of controls that have changed
    updatedControls = mUpdatedControls;

    // Clear list
    mUpdatedControls.clear();
  }

  // Now inform controllers
  for (sizeT i = 0; i < updatedControls.size(); ++i) {
    // Inform controller
    Control* control = updatedControls[i];
    control->get_controller().inform_control(control);
  }

  // Update all devices
  for (sizeT i = 0; i < mDevices.size(); ++i) {
    mDevices[i]->update();
  }
}

void InputServer::detect_devices(bool reset) {
  LockGuard<Mutex> mutexLock(*mMutex);

  if (reset) {
    clear();
  }

  // Now query all device providers
#if defined(LINUX)
  detect_device_providers("core::LinuxDeviceProvider", reset);
#elif defined(WIN32)
  detect_device_providers("core::WindowsDeviceProvider", reset);
#endif
}

const Vector<DeviceProvider *> &InputServer::get_device_providers() const {
  return mDeviceProviders;
}

DeviceProvider *InputServer::get_device_provider(const String &provider) {
  if (!mMapDeviceProviders.contains(provider)) {
    return nullptr;
  }
  return mMapDeviceProviders.find(provider).value();
  //Map<String, DeviceProvider*>::const_iterator iter = mMapDeviceProviders.find(provider);
  //return (iter != mMapDeviceProviders.cend()) ? iter.value() : nullptr;
}

Vector<Device *> &InputServer::get_devices() {
  return mDevices;
}

Device *InputServer::get_device(const String &device) const {
  Map<String, Device*>::const_iterator iter = mMapDevices.find(device);
  return (iter != mMapDevices.cend()) ? iter.value() : nullptr;
}

Keyboard *InputServer::get_keyboard() const {
  return reinterpret_cast<Keyboard*>(get_device("Keyboard"));
}

Mouse *InputServer::get_mouse() const {
  return reinterpret_cast<Mouse*>(get_device("Mouse"));
}

void InputServer::clear() {
  // Destroy all device providers
  for (sizeT i = 0; i < mDeviceProviders.size(); ++i) {
    delete mDeviceProviders[i];
  }
  mDeviceProviders.clear();
  mMapDeviceProviders.clear();

  // Destroy all other devices (e.g. bluetooth and hid devices, etc.)
  for (sizeT i = 0; i < mDevices.size(); ++i) {
    delete mDevices[i];
  }
  mDevices.clear();
  mMapDevices.clear();
}

void InputServer::detect_device_providers(const String &provider, bool reset) {
  DeviceProvider* deviceProvider = get_device_provider(provider);

  if (!deviceProvider) {
#if defined(LINUX)
    if (provider == "core::LinuxDeviceProvider") {
      deviceProvider = new LinuxDeviceProvider(*this);
    }
#elif defined(WIN32)

#endif

    if (deviceProvider != nullptr) {
      mDeviceProviders.push_back(deviceProvider);
      mMapDeviceProviders.set(provider, deviceProvider);
    }
  }

  if (deviceProvider) {
    deviceProvider->detect_devices(reset);
  }
}

bool InputServer::add_device(Device *device) {
  if (device && mMapDevices.find(device->get_name()) == mMapDevices.end()) {
    mDevices.push_back(device);
    mMapDevices.set(device->get_name(), device);

    return true;
  }
  // Something went wrong
  return false;
}

bool InputServer::remove_device(Device *device) {
  if (device) {
    // Find device
    Vector<Device*>::const_iterator iterator = core::find(mDevices.cbegin(), mDevices.cend(), device);
    if (mDevices.cend() != iterator) {
      mDevices.erase(iterator);
      mMapDevices.erase(device->get_name());
      return true;
    }
  }
  // Something went wrong
  return false;
}

void InputServer::remove_control(Control *control) {
  if (control) {
    // Remove control from list (if it's within the list at all)
    LockGuard<Mutex> mutexLock(*mMutex);
    Vector<Control*>::const_iterator iterator = std::find(mUpdatedControls.cbegin(), mUpdatedControls.cend(), control);
    if (mUpdatedControls.cend() != iterator) {
      mUpdatedControls.erase(iterator);
    }
  }
}

void InputServer::update_control(Control *control) {
  if (control) {
    // Add control to list, but only if it's not already within the list!
    LockGuard<Mutex> mutexLock(*mMutex);
    if (core::find(mUpdatedControls.cbegin(), mUpdatedControls.cend(), control) == mUpdatedControls.cend()) {
      mUpdatedControls.push_back(control);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core