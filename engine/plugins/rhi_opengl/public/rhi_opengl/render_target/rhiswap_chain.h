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
#include "rhi_opengl/rhi_opengl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;

class RHIOpenGLContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL swap chain class
 */
class RHISwapChain final : public rhi::RHISwapChain {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] renderPass
   * Render pass to use, the swap chain keeps a reference to the render pass
   * @param[in] windowHandle
   * Information about the window to render into
   * @param[in] useExternalContext
   * Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
   */
  RHISwapChain(rhi::RHIRenderPass &renderPass, core::FrontendPlatformData frontendPlatformData,
               [[maybe_unused]] bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  ~RHISwapChain() override;

  /**
   * @brief
   * Return the OpenGL context
   * 
   * @return
   * The OpenGL context
   */
  [[nodiscard]] inline RHIOpenGLContext &get_opengl_context() const {
    return *mOpenGLContext;
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  virtual void get_width_and_height(core::uint32 &width, core::uint32 &height) const override;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHISwapChain methods                ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual core::handle get_native_window_handle() const override {
    return mNativeWindowHandle;
  }

  inline virtual void set_vertical_synchronization_interval(core::uint32 synchronizationInterval) override {
    mNewVerticalSynchronizationInterval = synchronizationInterval;
  }

  virtual void present() override;

  inline virtual void resize_buffers() override {
    // Nothing here
  }

  [[nodiscard]] inline virtual bool get_fullscreen_state() const override {
    // TODO(naetherm) Implement me
    return false;
  }

  inline virtual void set_fullscreen_state(bool) override {
    // TODO(naetherm) Implement me
  }

  inline virtual void set_render_window(rhi::IRenderWindow *renderWindow) override {
    mRenderWindow = renderWindow;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHISwapChain(const RHISwapChain &source) = delete;

  RHISwapChain &operator=(const RHISwapChain &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  core::FrontendPlatformData mFrontendPlatformData;
  core::handle mNativeWindowHandle;  ///< Native window handle window, can be a null handle
  RHIOpenGLContext *mOpenGLContext;      ///< OpenGL context, must be valid
  bool mOwnsOpenGLContext;    ///< Does this swap chain own the OpenGL context?
  rhi::IRenderWindow *mRenderWindow;      ///< Render window instance, can be a null pointer, don't destroy the instance since we don't own it
  core::uint32 mVerticalSynchronizationInterval;
  core::uint32 mNewVerticalSynchronizationInterval;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
