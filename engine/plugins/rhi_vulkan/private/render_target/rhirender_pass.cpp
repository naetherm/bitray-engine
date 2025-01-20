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
#include "rhi_vulkan/render_target/rhirender_pass.h"
#include "rhi_vulkan/vulkan_context.h"
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
RHIRenderPass::RHIRenderPass(RHIDevice& vulkanRhi, core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIRenderPass(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkRenderPass(VK_NULL_HANDLE)
, mNumberOfColorAttachments(numberOfColorAttachments)
, mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat)
, mVkSampleCountFlagBits(Mapping::get_vulkan_sample_count_flag_bits(numberOfMultisamples)) {
  const bool hasDepthStencilAttachment = (rhi::TextureFormat::Enum::UNKNOWN != depthStencilAttachmentTextureFormat);

  // Vulkan attachment descriptions
  core::Vector<VkAttachmentDescription> vkAttachmentDescriptions;
  vkAttachmentDescriptions.resize(mNumberOfColorAttachments + (hasDepthStencilAttachment ? 1u : 0u));
  core::uint32 currentVkAttachmentDescriptionIndex = 0;

  // Handle color attachments
  typedef core::Vector<VkAttachmentReference> VkAttachmentReferences;
  VkAttachmentReferences colorVkAttachmentReferences;
  if (mNumberOfColorAttachments > 0) {
    colorVkAttachmentReferences.resize(mNumberOfColorAttachments);
    for (core::uint32 i = 0; i < mNumberOfColorAttachments; ++i) {
      { // Setup Vulkan color attachment references
        VkAttachmentReference &vkAttachmentReference = colorVkAttachmentReferences[currentVkAttachmentDescriptionIndex];
        vkAttachmentReference.attachment = currentVkAttachmentDescriptionIndex;      // attachment (core::uint32)
        vkAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // layout (VkImageLayout)
      }

      { // Setup Vulkan color attachment description
        VkAttachmentDescription &vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
        vkAttachmentDescription.flags = 0;                                // flags (VkAttachmentDescriptionFlags)
        vkAttachmentDescription.format = Mapping::get_vulkan_format(
          colorAttachmentTextureFormats[i]);  // format (VkFormat)
        vkAttachmentDescription.samples = mVkSampleCountFlagBits;                    // samples (VkSampleCountFlagBits)
        vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                  // loadOp (VkAttachmentLoadOp)
        vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;                  // storeOp (VkAttachmentStoreOp)
        vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // stencilLoadOp (VkAttachmentLoadOp)
        vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                // stencilStoreOp (VkAttachmentStoreOp)
        vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                    // initialLayout (VkImageLayout)
        vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;            // finalLayout (VkImageLayout)
      }

      // Advance current Vulkan attachment description index
      ++currentVkAttachmentDescriptionIndex;
    }
  }

  // Handle depth stencil attachments
  const VkAttachmentReference depthVkAttachmentReference =
    {
      currentVkAttachmentDescriptionIndex,        // attachment (core::uint32)
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // layout (VkImageLayout)
    };
  if (hasDepthStencilAttachment) {
    // Setup Vulkan depth attachment description
    VkAttachmentDescription &vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
    vkAttachmentDescription.flags = 0;                                // flags (VkAttachmentDescriptionFlags)
    vkAttachmentDescription.format = Mapping::get_vulkan_format(
      depthStencilAttachmentTextureFormat);  // format (VkFormat)
    vkAttachmentDescription.samples = mVkSampleCountFlagBits;                    // samples (VkSampleCountFlagBits)
    vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                  // loadOp (VkAttachmentLoadOp)
    vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;                  // storeOp (VkAttachmentStoreOp)
    vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // stencilLoadOp (VkAttachmentLoadOp)
    vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                // stencilStoreOp (VkAttachmentStoreOp)
    vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                    // initialLayout (VkImageLayout)
    vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;        // finalLayout (VkImageLayout)
    // ++currentVkAttachmentDescriptionIndex;	// Not needed since we're the last
  }

  // Create Vulkan create render pass
  const VkSubpassDescription vkSubpassDescription = {
      0,                                        // flags (VkSubpassDescriptionFlags)
      VK_PIPELINE_BIND_POINT_GRAPHICS,                        // pipelineBindPoint (VkPipelineBindPoint)
      0,                                        // inputAttachmentCount (core::uint32)
      nullptr,                                    // pInputAttachments (const VkAttachmentReference*)
      mNumberOfColorAttachments,                            // colorAttachmentCount (core::uint32)
      (mNumberOfColorAttachments > 0) ? colorVkAttachmentReferences.data()
                                      : nullptr,  // pColorAttachments (const VkAttachmentReference*)
      nullptr,                                    // pResolveAttachments (const VkAttachmentReference*)
      hasDepthStencilAttachment ? &depthVkAttachmentReference
                                : nullptr,        // pDepthStencilAttachment (const VkAttachmentReference*)
      0,                                        // preserveAttachmentCount (core::uint32)
      nullptr                                      // pPreserveAttachments (const core::uint32*)
    };
  static constexpr core::Array<VkSubpassDependency, 2> vkSubpassDependencies = {{
       {
         VK_SUBPASS_EXTERNAL,                            // srcSubpass (core::uint32)
         0,                                      // dstSubpass (core::uint32)
         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,                    // srcStageMask (VkPipelineStageFlags)
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                // dstStageMask (VkPipelineStageFlags)
         VK_ACCESS_MEMORY_READ_BIT,                          // srcAccessMask (VkAccessFlags)
         VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,  // dstAccessMask (VkAccessFlags)
         VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags (VkDependencyFlags)
       },
       {
         0,                                      // srcSubpass (core::uint32)
         VK_SUBPASS_EXTERNAL,                            // dstSubpass (core::uint32)
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                // srcStageMask (VkPipelineStageFlags)
         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,                    // dstStageMask (VkPipelineStageFlags)
         VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,  // srcAccessMask (VkAccessFlags)
         VK_ACCESS_MEMORY_READ_BIT,                          // dstAccessMask (VkAccessFlags)
         VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags (VkDependencyFlags)
       }
     }};
  const VkRenderPassCreateInfo vkRenderPassCreateInfo = {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,        // sType (VkStructureType)
      nullptr,                        // pNext (const void*)
      0,                            // flags (VkRenderPassCreateFlags)
      static_cast<core::uint32>(vkAttachmentDescriptions.size()),  // attachmentCount (core::uint32)
      vkAttachmentDescriptions.data(),            // pAttachments (const VkAttachmentDescription*)
      1,                            // subpassCount (core::uint32)
      &vkSubpassDescription,                  // pSubpasses (const VkSubpassDescription*)
      static_cast<core::uint32>(vkSubpassDependencies.size()),  // dependencyCount (core::uint32)
      vkSubpassDependencies.data()              // pDependencies (const VkSubpassDependency*)
    };
  if (vkCreateRenderPass(vulkanRhi.get_vulkan_context().get_vk_device(), &vkRenderPassCreateInfo,
                         vulkanRhi.get_vk_allocation_callbacks(), &mVkRenderPass) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT) {
      RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Render pass", 14)	// 14 = "Render pass: " including terminating zero
      VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (core::uint64)mVkRenderPass, detailedDebugName);
    }
#endif
  } else {
    BE_LOG(Critical, "Failed to create Vulkan render pass")
  }
}

RHIRenderPass::~RHIRenderPass() override {
  // Destroy Vulkan render pass instance
  if (VK_NULL_HANDLE != mVkRenderPass) {
    const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
    vkDestroyRenderPass(vulkanRhi.get_vulkan_context().get_vk_device(), mVkRenderPass,
                        vulkanRhi.get_vk_allocation_callbacks());
  }
}


[[nodiscard]] VkRenderPass RHIRenderPass::get_vk_render_pass() const {
  return mVkRenderPass;
}

[[nodiscard]] core::uint32 RHIRenderPass::get_number_of_color_attachments() const {
  return mNumberOfColorAttachments;
}

[[nodiscard]] core::uint32 RHIRenderPass::get_number_of_attachments() const {
  return (mDepthStencilAttachmentTextureFormat != rhi::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments + 1) : mNumberOfColorAttachments;
}

[[nodiscard]] rhi::TextureFormat::Enum RHIRenderPass::get_depth_stencil_attachment_texture_format() const {
  return mDepthStencilAttachmentTextureFormat;
}

[[nodiscard]] VkSampleCountFlagBits RHIRenderPass::get_vk_sample_count_flag_bits() const {
  return mVkSampleCountFlagBits;
}


void RHIRenderPass::self_destruct() override {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan