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
#include "rhi_vulkan/render_target/rhiframebuffer.h"
#include "rhi_vulkan/render_target/rhirender_pass.h"
#include "rhi_vulkan/texture/rhitexture_2d.h"
#include "rhi_vulkan/texture/rhitexture_2d_array.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"
#include <core/container/vector.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIFramebuffer::RHIFramebuffer(rhi::RHIRenderPass &renderPass, const rhi::FramebufferAttachment *colorFramebufferAttachments, const rhi::FramebufferAttachment *depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mNumberOfColorTextures(static_cast<RHIRenderPass &>(renderPass).get_number_of_color_attachments())
, mColorTextures(nullptr)
, mDepthStencilTexture(nullptr)
, mWidth(UINT_MAX)
, mHeight(UINT_MAX)
, mVkRenderPass(static_cast<RHIRenderPass &>(renderPass).get_vk_render_pass())
, mVkFramebuffer(VK_NULL_HANDLE) {
  const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(renderPass.get_rhi());

  // Vulkan attachment descriptions and views to fill
  core::Vector<VkImageView> vkImageViews;
  vkImageViews.resize(mNumberOfColorTextures + ((nullptr != depthStencilFramebufferAttachment) ? 1u : 0u));
  core::uint32 currentVkAttachmentDescriptionIndex = 0;

  // Add a reference to the used color textures
  if (mNumberOfColorTextures > 0) {
    mColorTextures = re_typed_alloc<rhi::RHITexture*>(mNumberOfColorTextures);

    // Loop through all color textures
    rhi::RHITexture **colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (rhi::RHITexture **colorTexture = mColorTextures;
         colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments) {
      // Sanity check
      BE_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid Vulkan color framebuffer attachment texture")

      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *colorTexture = colorFramebufferAttachments->texture;
      (*colorTexture)->add_ref();

      // Evaluate the color texture type
      VkImageView vkImageView = VK_NULL_HANDLE;
      switch ((*colorTexture)->get_resource_type()) {
        case rhi::ResourceType::TEXTURE_2D: {
          const RHITexture2D *texture2D = static_cast<RHITexture2D *>(*colorTexture);

          // Sanity checks
          BE_ASSERT(colorFramebufferAttachments->mipmapIndex <
                      RHITexture2D::get_number_of_mipmaps(texture2D->get_width(), texture2D->get_height()),
                    "Invalid Vulkan color framebuffer attachment mipmap index")
          BE_ASSERT(0 == colorFramebufferAttachments->layerIndex,
                    "Invalid Vulkan color framebuffer attachment layer index")

          // Update the framebuffer width and height if required
          vkImageView = texture2D->get_vk_image_view();
          VulkanHelper::update_width_and_height(colorFramebufferAttachments->mipmapIndex, texture2D->get_width(),
                                            texture2D->get_height(), mWidth, mHeight);
          break;
        }

        case rhi::ResourceType::TEXTURE_2D_ARRAY: {
          // Update the framebuffer width and height if required
          const RHITexture2DArray *texture2DArray = static_cast<RHITexture2DArray *>(*colorTexture);
          vkImageView = texture2DArray->get_vk_image_view();
          VulkanHelper::update_width_and_height(colorFramebufferAttachments->mipmapIndex, texture2DArray->get_width(),
                                            texture2DArray->get_height(), mWidth, mHeight);
          break;
        }

        case rhi::ResourceType::ROOT_SIGNATURE:
        case rhi::ResourceType::RESOURCE_GROUP:
        case rhi::ResourceType::GRAPHICS_PROGRAM:
        case rhi::ResourceType::VERTEX_ARRAY:
        case rhi::ResourceType::RENDER_PASS:
        case rhi::ResourceType::QUERY_POOL:
        case rhi::ResourceType::SWAP_CHAIN:
        case rhi::ResourceType::FRAMEBUFFER:
        case rhi::ResourceType::VERTEX_BUFFER:
        case rhi::ResourceType::INDEX_BUFFER:
        case rhi::ResourceType::TEXTURE_BUFFER:
        case rhi::ResourceType::STRUCTURED_BUFFER:
        case rhi::ResourceType::INDIRECT_BUFFER:
        case rhi::ResourceType::UNIFORM_BUFFER:
        case rhi::ResourceType::TEXTURE_1D:
        case rhi::ResourceType::TEXTURE_1D_ARRAY:
        case rhi::ResourceType::TEXTURE_3D:
        case rhi::ResourceType::TEXTURE_CUBE:
        case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
        case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
        case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
        case rhi::ResourceType::SAMPLER_STATE:
        case rhi::ResourceType::VERTEX_SHADER:
        case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
        case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case rhi::ResourceType::GEOMETRY_SHADER:
        case rhi::ResourceType::FRAGMENT_SHADER:
        case rhi::ResourceType::TASK_SHADER:
        case rhi::ResourceType::MESH_SHADER:
        case rhi::ResourceType::COMPUTE_SHADER:
        default:
          // Nothing here
          break;
      }

      // Remember the Vulkan image view
      vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;

      // Advance current Vulkan attachment description index
      ++currentVkAttachmentDescriptionIndex;
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment) {
    mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
    BE_ASSERT(nullptr != mDepthStencilTexture, "Invalid Vulkan depth stencil framebuffer attachment texture")
    mDepthStencilTexture->add_ref();

    // Evaluate the depth stencil texture type
    VkImageView vkImageView = VK_NULL_HANDLE;
    switch (mDepthStencilTexture->get_resource_type()) {
      case rhi::ResourceType::TEXTURE_2D: {
        const RHITexture2D *texture2D = static_cast<RHITexture2D *>(mDepthStencilTexture);

        // Sanity checks
        BE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex <
                    RHITexture2D::get_number_of_mipmaps(texture2D->get_width(), texture2D->get_height()),
                  "Invalid Vulkan depth stencil framebuffer attachment mipmap index")
        BE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex,
                  "Invalid Vulkan depth stencil framebuffer attachment layer index")

        // Update the framebuffer width and height if required
        vkImageView = texture2D->get_vk_image_view();
        VulkanHelper::update_width_and_height(depthStencilFramebufferAttachment->mipmapIndex, texture2D->get_width(),
                                          texture2D->get_height(), mWidth, mHeight);
        break;
      }

      case rhi::ResourceType::TEXTURE_2D_ARRAY: {
        // Update the framebuffer width and height if required
        const RHITexture2DArray *texture2DArray = static_cast<RHITexture2DArray *>(mDepthStencilTexture);
        vkImageView = texture2DArray->get_vk_image_view();
        VulkanHelper::update_width_and_height(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->get_width(),
                                          texture2DArray->get_height(), mWidth, mHeight);
        break;
      }

      case rhi::ResourceType::ROOT_SIGNATURE:
      case rhi::ResourceType::RESOURCE_GROUP:
      case rhi::ResourceType::GRAPHICS_PROGRAM:
      case rhi::ResourceType::VERTEX_ARRAY:
      case rhi::ResourceType::RENDER_PASS:
      case rhi::ResourceType::QUERY_POOL:
      case rhi::ResourceType::SWAP_CHAIN:
      case rhi::ResourceType::FRAMEBUFFER:
      case rhi::ResourceType::VERTEX_BUFFER:
      case rhi::ResourceType::INDEX_BUFFER:
      case rhi::ResourceType::TEXTURE_BUFFER:
      case rhi::ResourceType::STRUCTURED_BUFFER:
      case rhi::ResourceType::INDIRECT_BUFFER:
      case rhi::ResourceType::UNIFORM_BUFFER:
      case rhi::ResourceType::TEXTURE_1D:
      case rhi::ResourceType::TEXTURE_1D_ARRAY:
      case rhi::ResourceType::TEXTURE_3D:
      case rhi::ResourceType::TEXTURE_CUBE:
      case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
      case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
      case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
      case rhi::ResourceType::SAMPLER_STATE:
      case rhi::ResourceType::VERTEX_SHADER:
      case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
      case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case rhi::ResourceType::GEOMETRY_SHADER:
      case rhi::ResourceType::FRAGMENT_SHADER:
      case rhi::ResourceType::TASK_SHADER:
      case rhi::ResourceType::MESH_SHADER:
      case rhi::ResourceType::COMPUTE_SHADER:
      default:
        // Nothing here
        break;
    }

    // Remember the Vulkan image view
    vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;
  }

  // Validate the framebuffer width and height
  if (0 == mWidth || UINT_MAX == mWidth) {
    BE_ASSERT(false, "Invalid Vulkan framebuffer width")
    mWidth = 1;
  }
  if (0 == mHeight || UINT_MAX == mHeight) {
    BE_ASSERT(false, "Invalid Vulkan framebuffer height")
    mHeight = 1;
  }

  // Create Vulkan framebuffer
  const VkFramebufferCreateInfo vkFramebufferCreateInfo =
    {
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                  // pNext (const void*)
      0,                      // flags (VkFramebufferCreateFlags)
      mVkRenderPass,                // renderPass (VkRenderPass)
      static_cast<core::uint32>(vkImageViews.size()),  // attachmentCount (core::uint32)
      vkImageViews.data(),            // pAttachments (const VkImageView*)
      mWidth,                    // width (core::uint32)
      mHeight,                  // height (core::uint32
      1                      // layers (core::uint32)
    };
  if (vkCreateFramebuffer(vulkanRhi.get_vulkan_context().get_vk_device(), &vkFramebufferCreateInfo,
                          vulkanRhi.get_vk_allocation_callbacks(), &mVkFramebuffer) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
            const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (core::uint64)mVkRenderPass, detailedDebugName);
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, (core::uint64)mVkFramebuffer, detailedDebugName);
          }
#endif
  } else {
    BE_LOG(Critical, "Failed to create Vulkan framebuffer")
  }
}

/**
*  @brief
*    Destructor
*/
RHIFramebuffer::~RHIFramebuffer() {
  const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();

  // Destroy Vulkan framebuffer instance
  if (VK_NULL_HANDLE != mVkFramebuffer) {
    vkDestroyFramebuffer(vkDevice, mVkFramebuffer, vulkanRhi.get_vk_allocation_callbacks());
  }

  // Release the reference to the used color textures
  if (nullptr != mColorTextures) {
    // Release references
    rhi::RHITexture **colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (rhi::RHITexture **colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture) {
      (*colorTexture)->release();
    }

    // Cleanup
    re_free(mColorTextures);
  }

  // Release the reference to the used depth stencil texture
  if (nullptr != mDepthStencilTexture) {
    // Release reference
    mDepthStencilTexture->release();
  }
}


VkRenderPass RHIFramebuffer::get_vk_render_pass() const {
  return mVkRenderPass;
}

VkFramebuffer RHIFramebuffer::get_vk_framebuffer() const {
  return mVkFramebuffer;
}

void RHIFramebuffer::get_width_and_height(core::uint32& width, core::uint32& height) const override {
  // No fancy implementation in here, just copy over the internal information
  width  = mWidth;
  height = mHeight;
}

void RHIFramebuffer::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
