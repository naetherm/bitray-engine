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
#include "rhi_vulkan/texture/rhitexture_manager.h"
#include "rhi_vulkan/texture/rhitexture_1d.h"
#include "rhi_vulkan/texture/rhitexture_1d_array.h"
#include "rhi_vulkan/texture/rhitexture_2d.h"
#include "rhi_vulkan/texture/rhitexture_2d_array.h"
#include "rhi_vulkan/texture/rhitexture_3d.h"
#include "rhi_vulkan/texture/rhitexture_cube.h"
#include "rhi_vulkan/texture/rhitexture_cube_array.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHITextureManager::RHITextureManager(RHIDevice &vulkanRhi)
: rhi::RHITextureManager(vulkanRhi) {
  
}

RHITextureManager::~RHITextureManager() {
  
}


rhi::RHITexture1D *RHITextureManager::create_texture_1d(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0, "Vulkan create texture 1D was called with invalid parameters")

  // Create 1D texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITexture1D(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITexture1DArray *RHITextureManager::create_texture_1d_array(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.depth > 0, "Vulkan create texture 1D array was called with invalid parameters")

  // Create 1D texture array resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITexture1DArray(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITexture2D *RHITextureManager::create_texture_2d(rhi::TextureDescriptor descriptor, const void* data, [[maybe_unused]] const rhi::OptimizedTextureClearValue* optimizedTextureClearValue RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0, "Vulkan create texture 2D was called with invalid parameters")

  // Create 2D texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITexture2D(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITexture2DArray *RHITextureManager::create_texture_2d_array(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0 && descriptor.size.depth > 0,
            "Vulkan create texture 2D array was called with invalid parameters")

  // Create 2D texture array resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITexture2DArray(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITexture3D *RHITextureManager::create_texture_3d(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.height > 0 && descriptor.size.depth > 0, "Vulkan create texture 3D was called with invalid parameters")

  // Create 3D texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITexture3D(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITextureCube *RHITextureManager::create_texture_cube(rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0, "Vulkan create texture cube was called with invalid parameters")

  // Create cube texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITextureCube(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITextureCubeArray *RHITextureManager::create_texture_cube_array(rhi::TextureDescriptor descriptor, [[maybe_unused]] const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(descriptor.size.width > 0 && descriptor.size.depth > 0, "Vulkan create texture cube was called with invalid parameters")

  // Create cube texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return new RHITextureCubeArray(vulkanRhi, descriptor, data RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

void RHITextureManager::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
