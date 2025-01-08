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
#include "core/input/types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Device;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DeviceImpl
 *
 * @brief
 * Device interface implementation.
 */
class DeviceImpl {
  
  friend class Device;
  
public:
  /**
   * @brief
   * Constructor.
   */
  DeviceImpl();
  
  /**
   * @brief
   * Destructor.
   */
  virtual ~DeviceImpl();
  
  
  /**
   * @brief
   * Get device backend type.
   *
   * @return
   * Device backend type.
   */
  [[nodiscard]] inline DeviceBackendType get_device_backend_type() const;
  
  /**
   * @brief
   * Get device.
   *
   * @¶eturn
   * Owning device.
   */
  [[nodiscard]] inline Device* get_device() const;
  
protected:
  /**
   * @brief
   * Sets the device.
   *
   * @param[in] device
   * Pointer to the device to set.
   */
  inline void set_device(Device* device);
  
protected:
  /** The device backend type */
  DeviceBackendType mDeviceBackendType;
  /** Pointer to the owning device implementation */
  Device* mDevice;
  /** If true, the device implementation will be destroyed automatically */
  bool mDelete;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/input/device/device_impl.inl"