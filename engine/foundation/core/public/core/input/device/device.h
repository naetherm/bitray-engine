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
#include "core/input/controller.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DeviceImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Device
 *
 * @brief
 * Default device implementation.
 */
class Device : public Controller {
  
  friend class DeviceImpl;

public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] inputManager
   * Input manager reference.
   * @param[in] name
   * The name of the device.
   * @param[in] description
   * The description of the device.
   * @þaram[in] deviceImpl
   * The platform specific implementation of a device, e.g. mouse or keyboard.
   */
  Device(InputServer& inputManager, const String& name, const String& description, DeviceImpl* deviceImpl);
  
  /**
   * @brief
   * Destructor.
   */
  ~Device() override;
  
  
  /**
   * @brief
   * Returns pointer to the internally device implementation.
   *
   * @return
   * Pointer to internally used device implementation.
   */
  [[nodiscard]] inline DeviceImpl* get_impl() const;
  
  Device& operator=(const Device& rhs) = delete;
  
protected:
  /** Pointer to system specific device implementation */
  DeviceImpl* mDeviceImpl;
  /** Delete the device implementation automatically */
  bool mDeleteImpl;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/input/device/device.inl"