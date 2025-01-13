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
#include "rhi_opengl/texture/rhitexture_manager.h"
#include "rhi_opengl/texture/rhitexture_1d_bind.h"
#include "rhi_opengl/texture/rhitexture_1d_dsa.h"
#include "rhi_opengl/texture/rhitexture_1d_array_bind.h"
#include "rhi_opengl/texture/rhitexture_1d_array_dsa.h"
#include "rhi_opengl/texture/rhitexture_2d_bind.h"
#include "rhi_opengl/texture/rhitexture_2d_dsa.h"
#include "rhi_opengl/texture/rhitexture_2d_array_bind.h"
#include "rhi_opengl/texture/rhitexture_2d_array_dsa.h"
#include "rhi_opengl/texture/rhitexture_3d_bind.h"
#include "rhi_opengl/texture/rhitexture_3d_dsa.h"
#include "rhi_opengl/texture/rhitexture_cube_bind.h"
#include "rhi_opengl/texture/rhitexture_cube_dsa.h"
#include "rhi_opengl/texture/rhitexture_cube_array_bind.h"
#include "rhi_opengl/texture/rhitexture_cube_array_dsa.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
TextureManager::TextureManager(RHIDevice& openGLRhi)
: RHITextureManager(openGLRhi)
, mExtensions(&openGLRhi.get_extensions()) {
  
}

rhi::RHITexture1D* TextureManager::create_texture_1d(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0, "OpenGL create texture 1D was called with invalid parameters")

  // Create 1D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHITexture1DDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHITexture1DBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

rhi::RHITexture1DArray* TextureManager::create_texture_1d_array(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.depth > 0, "OpenGL create texture 1D array was called with invalid parameters")

  // Create 1D texture array resource, "GL_EXT_texture_array"-extension required
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_texture_array()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHITexture1DArrayDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHITexture1DArrayBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    return nullptr;
  }
}

rhi::RHITexture2D* TextureManager::create_texture_2d(rhi::TextureDescriptor descriptor, const void* data, [[maybe_unused]] const rhi::OptimizedTextureClearValue* optimizedTextureClearValue RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0, "OpenGL create texture 2D was called with invalid parameters")

  // Create 2D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHITexture2DDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHITexture2DBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

rhi::RHITexture2DArray* TextureManager::create_texture_2d_array(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0 && descriptor.size.depth > 0, "OpenGL create texture 2D array was called with invalid parameters")

  // Create 2D texture array resource, "GL_EXT_texture_array"-extension required
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_texture_array()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHITexture2DArrayDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHITexture2DArrayBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    return nullptr;
  }
}

rhi::RHITexture3D* TextureManager::create_texture_3d(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0 && descriptor.size.depth > 0, "OpenGL create texture 3D was called with invalid parameters")

  // Create 3D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHITexture3DDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHITexture3DBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

rhi::RHITextureCube* TextureManager::create_texture_cube(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0, "OpenGL create texture cube was called with invalid parameters")

  // Create cube texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  // Is "GL_EXT_direct_state_access" or "GL_ARB_direct_state_access" there?
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHITextureCubeDsa>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHITextureCubeBind>(openGLRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

rhi::RHITextureCubeArray* TextureManager::create_texture_cube_array(rhi::TextureDescriptor descriptor, [[maybe_unused]] const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // TODO(naetherm) Implement me
#ifdef DEBUG
  debugName = debugName;
#endif
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
