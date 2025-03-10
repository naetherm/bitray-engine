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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "gui/gui.h"
#include <core/core/refcounted.h>
#include <core/core/abstract_context.h>
#include <core/core/ptr.h>
#include <core/string/string.h>
#include <core/platform/library.h>
#include "gui/gui/gui_settings.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiContext
 *
 * @brief
 * Basic gui context implementation, containing the rhi name, the corresponding
 * rhi library name and the loaded dynamic rhi library.
 */
class GuiContext : public core::AbstractContext, public core::RefCounted {
public:

  /**
   * @brief
   * Constructor.
   */
  GUI_API GuiContext();

  /**
   * @brief
   * Destructor.
   */
  ~GuiContext() override;


  /**
   * @brief
   * Initializes the gui context.
   * This will set all names correctly and that tries to create and load the corresponding
   * dynamic rhi library.
   *
   * @param[in] rhiName
   * The name of the rhi interface to use.
   *
   * @return
   * True if everything went fine, false otherwise.
   */
  [[nodiscard]] bool initialize_by_name(const core::String& rhiName);

  [[nodiscard]] bool initialize(const GuiSettings& guiSettings);
  
  [[nodiscard]] bool initialize(const core::Ptr<rhi::RHIDevice>& rhi);


  /**
   * @brief
   * Returns the name of the rhi that should be used for rendering.
   *
   * @return
   * The name of the rhi interface.
   */
  [[nodiscard]] const core::String& get_rhi_name() const;

  /**
   * @brief
   * Returns the full rhi library name.
   *
   * @return
   * Full rhi library name.
   */
  [[nodiscard]] const core::String& get_rhi_library_name() const;

  /**
   * @brief
   * Returns reference to the dynamic library of the rhi interface.
   *
   * @return
   * Reference to dynamic rhi library.
   */
  [[nodiscard]] core::Library& get_rhi_library() const;
  
  [[nodiscard]] const core::Ptr<rhi::RHIDevice>& get_rhi_device() const;
  
  [[nodiscard]] core::Ptr<rhi::RHIDevice>& get_rhi_device();

protected:
private:
  GuiSettings mGuiSettings;
  /** The rhi interface name, e.g. "opengl", "vulkan", etc */
  core::String mRhiName;
  /** The fully established rhi library name, e.g. "librerhiopengl.so" for linux and "rerhiopengl.dll" for windows */
  core::String mRhiLibraryName;
  /** Pointer to the dynamic rhi library, can be a nullptr if something went wrong */
  core::Library* mRhiLibrary;
  
  core::Ptr<rhi::RHIDevice> mRhiDynamicRhi;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}