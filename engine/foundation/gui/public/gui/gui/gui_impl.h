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
#include "gui/gui.h"
#include <core/core/refcounted.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiImpl
 *
 * @brief
 * Serves as a base class for platform-specific GUI implementations.
 *
 * This class provides a foundation for implementing graphical user interface (GUI) functionality
 * that is specific to an operating system or platform. It is designed to work in conjunction
 * with a `Gui` object, which represents the platform-agnostic interface for GUI operations.
 */
class GuiImpl : public core::RefCounted {
public:

  /**
   * @brief
   * Constructor.
   *
   * Initializes the `GuiImpl` instance with a reference to a `Gui` object. This object represents
   * the platform-agnostic GUI interface that this implementation will serve.
   *
   * @param[in] gui
   * A pointer to the `Gui` instance associated with this implementation.
   */
  GuiImpl(Gui* gui);

  /**
   * @brief
   * Virtual destructor.
   *
   * Ensures proper cleanup of resources in derived classes. This enables polymorphic behavior
   * and allows subclasses to implement platform-specific cleanup logic.
   */
  virtual ~GuiImpl();

protected:
  /** Pointer to the associated `Gui` instance for platform-agnostic operations. */
  Gui* mGui;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}