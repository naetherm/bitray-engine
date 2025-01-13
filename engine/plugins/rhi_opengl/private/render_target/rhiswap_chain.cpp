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
#include "rhi_opengl/render_target/rhiswap_chain.h"
#include "rhi_opengl/render_target/rhirender_pass.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#if defined(LINUX)
#include "rhi_opengl/linux/rhiopengl_context_linux.h"
#elif defined(WIN32)
#include "rhi_opengl/windows/rhiopengl_context_windows.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHISwapChain::RHISwapChain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData frontendPlatformData, [[maybe_unused]] bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mFrontendPlatformData(frontendPlatformData)
//, mNativeWindowHandle(windowHandle.nativeWindowHandle)
#ifdef _WIN32
, mOpenGLContext(re_new<OpenGLContextWindows>(static_cast<const RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat(), windowHandle.nativeWindowHandle, static_cast<const OpenGLContextWindows*>(&static_cast<RHIDevice&>(renderPass.get_rhi()).getOpenGLContext())))
#elif defined LINUX
, mNativeWindowHandle(frontendPlatformData.os_linux.window)
, mRenderWindow(nullptr)
, mOpenGLContext(
  re_new<OpenGLContextLinux>(
    static_cast<RHIDevice&>(renderPass.get_rhi()),
    static_cast<const RHIRenderPass &>(renderPass).get_depth_stencil_attachment_texture_format(), frontendPlatformData.os_linux.window, useExternalContext, static_cast<const OpenGLContextLinux*>(&static_cast<RHIDevice &>(renderPass.get_rhi()).get_opengl_context())))
#else
#error "Unsupported platform"
#endif
, mOwnsOpenGLContext(true)
//, mRenderWindow(windowHandle.renderWindow)
, mVerticalSynchronizationInterval(0)
, mNewVerticalSynchronizationInterval(0)	// 0 instead of ~0u to ensure that we always set the swap interval at least once to have a known initial setting
{
#if defined(LINUX)
  glXSwapIntervalEXT(static_cast<const OpenGLContextLinux &>(static_cast<RHIDevice &>(get_rhi()).get_opengl_context()).get_display(), mNativeWindowHandle, mVerticalSynchronizationInterval);
#endif
}

RHISwapChain::~RHISwapChain() {
  if (mOwnsOpenGLContext) {
    re_delete(mOpenGLContext);
  }
}

void RHISwapChain::get_width_and_height(core::uint32& width, core::uint32& height) const {
  // Return stored width and height when both valid
  if (nullptr != mRenderWindow) {
    mRenderWindow->get_width_and_height(width, height);
    return;
  }
#ifdef _WIN32
  // Is there a valid native OS window?
  if (NULL_HANDLE != mNativeWindowHandle) {
    // Get the width and height
    long swapChainWidth  = 1;
    long swapChainHeight = 1;
    {
      // Get the client rectangle of the native output window
      // -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
      //    because it might have been modified in order to avoid zero values
      RECT rect;
      ::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);

      // get the width and height...
      swapChainWidth  = rect.right  - rect.left;
      swapChainHeight = rect.bottom - rect.top;

      // ... and ensure that none of them is ever zero
      if (swapChainWidth < 1) {
        swapChainWidth = 1;
      }
      if (swapChainHeight < 1) {
        swapChainHeight = 1;
      }
    }

    // Write out the width and height
    width  = static_cast<UINT>(swapChainWidth);
    height = static_cast<UINT>(swapChainHeight);
  } else
#elif defined LINUX
  if (NULL_HANDLE != mNativeWindowHandle) {
    RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
    Display* display = static_cast<const OpenGLContextLinux &>(openGLRhi.get_opengl_context()).get_display();

    // get the width and height...
    ::Window rootWindow = 0;
    int positionX = 0, positionY = 0;
    unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
    if (nullptr != display) {
      XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight, &border, &depth);
    }

    // ... and ensure that none of them is ever zero
    if (unsignedWidth < 1) {
      unsignedWidth = 1;
    }
    if (unsignedHeight < 1) {
      unsignedHeight = 1;
    }

    // Done
    width = unsignedWidth;
    height = unsignedHeight;
  }
  else
#else
#error "Unsupported platform"
#endif
  {
    // Set known default return values
    width  = 1;
    height = 1;
  }
}

void RHISwapChain::present() {
  if (nullptr != mRenderWindow) {
    mRenderWindow->present();
    return;
  }
#ifdef _WIN32
  {
    // Set new vertical synchronization interval?
    // -> We do this in here to avoid having to use "wglMakeCurrent()"/"glXMakeCurrent()" to often at multiple places
    if (~0u != mNewVerticalSynchronizationInterval) {
      const Extensions& extensions = static_cast<RHIDevice&>(get_rhi()).get_extensions();
      if (extensions.isWGL_EXT_swap_control()) {
        // Use adaptive vertical synchronization if possible
        wglSwapIntervalEXT((extensions.isWGL_EXT_swap_control_tear() && mNewVerticalSynchronizationInterval > 0) ? -static_cast<int>(mNewVerticalSynchronizationInterval) : static_cast<int>(mNewVerticalSynchronizationInterval));
      }
      mVerticalSynchronizationInterval = mNewVerticalSynchronizationInterval;
      mNewVerticalSynchronizationInterval = ~0u;
    }

    // Swap buffers
    ::SwapBuffers(static_cast<OpenGLContextWindows*>(mOpenGLContext)->getDeviceContext());
    if (mVerticalSynchronizationInterval > 0) {
      glFinish();
    }
  }
#elif defined LINUX
  // TODO(naetherm) Add support for vertical synchronization and adaptive vertical synchronization: "GLX_EXT_swap_control" and "GLX_EXT_swap_control_tear"
  if (NULL_HANDLE != mNativeWindowHandle) {
    const Extensions& extensions = static_cast<RHIDevice&>(get_rhi()).get_extensions();

    if (mVerticalSynchronizationInterval != mNewVerticalSynchronizationInterval) {
      mVerticalSynchronizationInterval = mNewVerticalSynchronizationInterval;
      glXSwapIntervalEXT(static_cast<const OpenGLContextLinux &>(static_cast<RHIDevice &>(get_rhi()).get_opengl_context()).get_display(), mNativeWindowHandle, mVerticalSynchronizationInterval);
    }

    glXSwapBuffers( static_cast<const OpenGLContextLinux &>(static_cast<RHIDevice &>(get_rhi()).get_opengl_context()).get_display(), mNativeWindowHandle);

    if (mVerticalSynchronizationInterval) {
      glFinish();
    }
  }
#else
#error "Unsupported platform"
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
