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
#include "core/container/vector.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Device;
class InputServer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DeviceProvider
 *
 * @brief
 * Device provider base implementation.
 */
class DeviceProvider {
public:
  /**
   * @brief
   * Destructor.
   */
  virtual ~DeviceProvider();


  /**
   * @brief
   * Detect devices.
   *
   * @param[in] reset
   * If 'true', delete all input devices and re-detect them.
   */
  void detect_devices(bool reset = false);

  /**
   * @brief
   * Returns list of all devices.
   *
   * @return
   * List of all devices.
   */
  [[nodiscard]] const Vector<Device*>& get_devices() const;

protected:
  /**
   * @brief
   * Constructor.
   */
  DeviceProvider() = delete;

  /**
   * @brief
   * Constructor.
   */
  explicit DeviceProvider(InputServer& inputManager);


  DeviceProvider& operator=(const DeviceProvider& rhs) = delete;


  /**
   * @brief
   * Remove all devices.
   */
  void clear();

  /**
   * @brief
   * Check if a device is already known and if to, update it.
   *
   * @param[in] name
   * The name of the input device.
   */
  [[nodiscard]] bool check_device(const String& name);

  /**
   * @brief
   * Adds a new input device.
   *
   * @param[in] name
   * The name of the input device.
   * @param[in] device
   * Pointer to the device to add.
   */
  bool add_device(const String& name, Device* device);

protected:
  /**
   * @brief
   * Query devices.
   */
  virtual void query_devices() = 0;

protected:
  /** Reference to input manager */
  InputServer& mInputManager;
  /** List of devices of this provider */
  Vector<Device*> mDevices;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core