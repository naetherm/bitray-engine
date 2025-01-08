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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/core/server_impl.h"
#include "core/container/map.h"
#include "core/container/vector.h"
#include "core/input/device/keyboard.h"
#include "core/input/device/mouse.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Control;
class Device;
class DeviceProvider;
class Mutex;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * InputManager
 *
 * @brief
 * Input manager.
 */
class InputServer : public ServerImpl {

  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class Control;
  friend class DeviceProvider;

public:
  /**
   * @brief
   * Static method, returning the unique input manager instance.
   *
   * @return
   * Reference to unique input manager instance.
   */
  static InputServer& instance();

public:
  /**
   * @brief
   * Constructor.
   */
  InputServer();

  /**
   * @brief
   * Destructor.
   */
  ~InputServer() override;


  /**
   * @brief
   * Update input manager once per frame.
   */
  void update();

  /**
   * @brief
   * Detect devices.
   *
   * @param[in] reset
   * If ture, delete all input devices an re-detect them.
   */
  void detect_devices(bool reset = false);

  /**
   * @rbief
   * Returns list of all input device providers.
   *
   * @return
   * All input device providers.
   */
  [[nodiscard]] const Vector<DeviceProvider*>& get_device_providers() const;

  /**
   * @brief
   * Returns pointer to specific device provider.
   *
   * @param[in] provider
   * Name of device provider.
   *
   * @return
   * Pointer to device provider, can ne a nullptr.
   */
  [[nodiscard]] DeviceProvider* get_device_provider(const String& provider);


  /**
   * @brief
   * Returns all devices.
   *
   * @return
   * All devices.
   */
  [[nodiscard]] Vector<Device*>& get_devices();

  /**
   * @brief
   * Returns a specific device by its name.
   *
   * @param[in] device
   * The name of the device.
   *
   * @return
   * Pointer to the specified device, can be a nullptr.
   */
  [[nodiscard]] Device* get_device(const String& device) const;

  /**
   * @brief
   * Returns the default keyboard device.
   *
   * @return
   * Default keyboard device.
   */
  [[nodiscard]] Keyboard* get_keyboard() const;

  /**
   * @brief
   * Returns the default mouse device.
   *
   * @return
   * Default mouse device.
   */
  [[nodiscard]] Mouse* get_mouse() const;

private:
  /**
   * @brief
   * Destroy all devices and device providers.
   */
  void clear();

  /**
   * @brief
   * Detect devices from a given specific device provider.
   *
   * @param[in] provider
   * The name of the provider.
   * @param[in] reset
   * If true, clear everything before.
   */
  void detect_device_providers(const String& provider, bool reset);

  /**
   * @brief
   * Add a new input device.
   *
   * @param[in] device
   * The input device.
   *
   * @return
   * 'true' if everything went fine.
   */
  bool add_device(Device* device);

  /**
   * @brief
   * Removes the given input device.
   *
   * @param[in] device
   * The device to remove.
   *
   * @return
   * 'true' if everything went fine.
   */
  bool remove_device(Device* device);

  /**
   * @brief
   * Removes the given control.
   *
   * @param[in] control
   * The control to remove.
   */
  void remove_control(Control* control);

  /**
   * @brief
   * Updates the given control.
   *
   * @param[in] control
   * The control to update.
   */
  void update_control(Control* control);

private:
  /** List of all device providers */
  Vector<DeviceProvider*> mDeviceProviders;
  /** Map of all update providers */
  Map<String, DeviceProvider*> mMapDeviceProviders;
  /** List of all devices */
  Vector<Device*> mDevices;
  /** Map of all devices */
  Map<String, Device*> mMapDevices;
  /** List of all controls that needs an update */
  Vector<Control*> mUpdatedControls;
  /** Pointer to mutex */
  Mutex* mMutex;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core