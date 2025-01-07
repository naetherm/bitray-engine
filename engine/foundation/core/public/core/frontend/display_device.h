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
#include "core/core/refcounted.h"
#include "core/container/vector.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Display;
class DisplayDeviceImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DisplaySystem
 *
 * @brief
 * General display system.
 */
class DisplayDevice : public RefCounted {
public:
  /**
   * @brief
   * Constructor.
   */
  DisplayDevice();

  /**
   * @brief
   * Destructor.
   */
  virtual ~DisplayDevice();


  /**
   * @brief
   * Returns list of all displays.
   *
   * @return
   * List of all displays.
   */
  [[nodiscard]] inline const Vector<Display*>& get_displays() const;

  /**
   * @brief
   * Returns pointer to display as index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to display instance.
   */
  [[nodiscard]] inline Display* get_display(uint32 index) const;

  /**
   * @brief
   * Enumerated all displays available on the current system.
   */
  void enumerate_displays();

  /**
   * @brief
   * Clears the list of displays.
   */
  void clear();

private:
  /** Pointer to internally used Display system implementation */
  DisplayDeviceImpl* mImpl;
  /** List of all displays */
  Vector<Display*> mDisplays;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/display_device.inl"