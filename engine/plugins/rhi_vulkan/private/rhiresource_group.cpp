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
#include "rhi_vulkan/rhiresource_group.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/rhiroot_signature.h"
#include "rhi_vulkan/buffer/rhiindex_buffer.h"
#include "rhi_vulkan/buffer/rhiindirect_buffer.h"
#include "rhi_vulkan/buffer/rhivertex_buffer.h"
#include "rhi_vulkan/buffer/rhivertex_array.h"
#include "rhi_vulkan/buffer/rhitexture_buffer.h"
#include "rhi_vulkan/buffer/rhistructured_buffer.h"
#include "rhi_vulkan/buffer/rhiuniform_buffer.h"
#include "rhi_vulkan/texture/rhitexture_1d.h"
#include "rhi_vulkan/texture/rhitexture_1d_array.h"
#include "rhi_vulkan/texture/rhitexture_2d.h"
#include "rhi_vulkan/texture/rhitexture_2d_array.h"
#include "rhi_vulkan/texture/rhitexture_3d.h"
#include "rhi_vulkan/texture/rhitexture_cube_array.h"
#include "rhi_vulkan/texture/rhitexture_cube.h"
#include "rhi_vulkan/state/rhisampler_state.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIResourceGroup::RHIResourceGroup(RHIRootSignature &rootSignature, core::uint32 rootParameterIndex, VkDescriptorSet vkDescriptorSet, core::uint32 numberOfResources, rhi::RHIResource **resources, rhi::RHISamplerState **samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  rhi::RHIResourceGroup(rootSignature.get_rhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(rootSignature),
  mVkDescriptorSet(vkDescriptorSet),
  mNumberOfResources(numberOfResources),
  mResources(re_typed_alloc<rhi::RHIResource*>(mNumberOfResources)),
  mSamplerStates(nullptr) {
  mRootSignature.add_ref();

  // Process all resources and add our reference to the RHI resource
  const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
  if (nullptr != samplerStates) {
    mSamplerStates = re_typed_alloc<rhi::RHISamplerState*>(mNumberOfResources);
    for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      rhi::RHISamplerState *samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->add_ref();
      }
    }
  }
  for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources) {
    rhi::RHIResource *resource = *resources;
    BE_ASSERT(nullptr != resource, "Invalid Vulkan resource")
    mResources[resourceIndex] = resource;
    resource->add_ref();

    // Check the type of resource to set
    // TODO(naetherm) Some additional resource type root signature security checks in debug build?
    const rhi::ResourceType resourceType = resource->get_resource_type();
    switch (resourceType) {
      case rhi::ResourceType::VERTEX_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo = {
          static_cast<RHIVertexBuffer *>(resource)->get_vk_buffer(),  // buffer (VkBuffer)
          0,                            // offset (VkDeviceSize)
          VK_WHOLE_SIZE                      // range (VkDeviceSize)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
          nullptr,                  // pNext (const void*)
          mVkDescriptorSet,              // dstSet (VkDescriptorSet)
          resourceIndex,                // dstBinding (core::uint32)
          0,                      // dstArrayElement (core::uint32)
          1,                      // descriptorCount (core::uint32)
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
          nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
          &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                    // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::INDEX_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo = {
          static_cast<RHIIndexBuffer *>(resource)->get_vk_buffer(),  // buffer (VkBuffer)
          0,                          // offset (VkDeviceSize)
          VK_WHOLE_SIZE                    // range (VkDeviceSize)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
          nullptr,                  // pNext (const void*)
          mVkDescriptorSet,              // dstSet (VkDescriptorSet)
          resourceIndex,                // dstBinding (core::uint32)
          0,                      // dstArrayElement (core::uint32)
          1,                      // descriptorCount (core::uint32)
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
          nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
          &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                    // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::TEXTURE_BUFFER: {
        const rhi::DescriptorRange &descriptorRange = reinterpret_cast<const rhi::DescriptorRange *>(rootSignature.get_root_signature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
        BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "Vulkan texture buffer must bound at SRV or UAV descriptor range type")
        const VkBufferView vkBufferView = static_cast<RHITextureBuffer *>(resource)->get_vk_buffer_view();
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                                        // sType (VkStructureType)
          nullptr,                                                                      // pNext (const void*)
          mVkDescriptorSet,                                                                  // dstSet (VkDescriptorSet)
          resourceIndex,                                                                    // dstBinding (core::uint32)
          0,                                                                          // dstArrayElement (core::uint32)
          1,                                                                          // descriptorCount (core::uint32)
          (rhi::DescriptorRangeType::SRV == descriptorRange.rangeType) ? VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
                                                                         : VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,  // descriptorType (VkDescriptorType)
          nullptr,                                                                      // pImageInfo (const VkDescriptorImageInfo*)
          nullptr,                                                                      // pBufferInfo (const VkDescriptorBufferInfo*)
          &vkBufferView                                                                    // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::STRUCTURED_BUFFER: {
        [[maybe_unused]] const rhi::DescriptorRange &descriptorRange = reinterpret_cast<const rhi::DescriptorRange *>(rootSignature.get_root_signature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
        BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "Vulkan structured buffer must bound at SRV or UAV descriptor range type")
        const VkDescriptorBufferInfo vkDescriptorBufferInfo = {
          static_cast<RHIStructuredBuffer *>(resource)->get_vk_buffer(),  // buffer (VkBuffer)
          0,                              // offset (VkDeviceSize)
          VK_WHOLE_SIZE                        // range (VkDeviceSize)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,  // sType (VkStructureType)
          nullptr,                // pNext (const void*)
          mVkDescriptorSet,            // dstSet (VkDescriptorSet)
          resourceIndex,              // dstBinding (core::uint32)
          0,                    // dstArrayElement (core::uint32)
          1,                    // descriptorCount (core::uint32)
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,    // descriptorType (VkDescriptorType)
          nullptr,                // pImageInfo (const VkDescriptorImageInfo*)
          &vkDescriptorBufferInfo,        // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                  // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::INDIRECT_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo = {
          static_cast<RHIIndirectBuffer *>(resource)->get_vk_buffer(),  // buffer (VkBuffer)
          0,                            // offset (VkDeviceSize)
          VK_WHOLE_SIZE                      // range (VkDeviceSize)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
          nullptr,                  // pNext (const void*)
          mVkDescriptorSet,              // dstSet (VkDescriptorSet)
          resourceIndex,                // dstBinding (core::uint32)
          0,                      // dstArrayElement (core::uint32)
          1,                      // descriptorCount (core::uint32)
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
          nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
          &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                    // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::UNIFORM_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo = {
          static_cast<RHIUniformBuffer *>(resource)->get_vk_buffer(),  // buffer (VkBuffer)
          0,                            // offset (VkDeviceSize)
          VK_WHOLE_SIZE                      // range (VkDeviceSize)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,  // sType (VkStructureType)
          nullptr,                // pNext (const void*)
          mVkDescriptorSet,            // dstSet (VkDescriptorSet)
          resourceIndex,              // dstBinding (core::uint32)
          0,                    // dstArrayElement (core::uint32)
          1,                    // descriptorCount (core::uint32)
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,    // descriptorType (VkDescriptorType)
          nullptr,                // pImageInfo (const VkDescriptorImageInfo*)
          &vkDescriptorBufferInfo,        // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                  // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::TEXTURE_1D:
      case rhi::ResourceType::TEXTURE_1D_ARRAY:
      case rhi::ResourceType::TEXTURE_2D:
      case rhi::ResourceType::TEXTURE_2D_ARRAY:
      case rhi::ResourceType::TEXTURE_3D:
      case rhi::ResourceType::TEXTURE_CUBE:
      case rhi::ResourceType::TEXTURE_CUBE_ARRAY: {
        // Evaluate the texture type and get the Vulkan image view
        VkImageView vkImageView = VK_NULL_HANDLE;
        VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        switch (resourceType) {
          case rhi::ResourceType::TEXTURE_1D: {
            const RHITexture1D *texture1D = static_cast<RHITexture1D *>(resource);
            vkImageView = texture1D->get_vk_image_view();
            vkImageLayout = texture1D->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_1D_ARRAY: {
            const RHITexture1DArray *texture1DArray = static_cast<RHITexture1DArray *>(resource);
            vkImageView = texture1DArray->get_vk_image_view();
            vkImageLayout = texture1DArray->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_2D: {
            const RHITexture2D *texture2D = static_cast<RHITexture2D *>(resource);
            vkImageView = texture2D->get_vk_image_view();
            vkImageLayout = texture2D->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_2D_ARRAY: {
            const RHITexture2DArray *texture2DArray = static_cast<RHITexture2DArray *>(resource);
            vkImageView = texture2DArray->get_vk_image_view();
            vkImageLayout = texture2DArray->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_3D: {
            const RHITexture3D *texture3D = static_cast<RHITexture3D *>(resource);
            vkImageView = texture3D->get_vk_image_view();
            vkImageLayout = texture3D->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_CUBE: {
            const RHITextureCube *textureCube = static_cast<RHITextureCube *>(resource);
            vkImageView = textureCube->get_vk_image_view();
            vkImageLayout = textureCube->get_vk_image_layout();
            break;
          }

          case rhi::ResourceType::TEXTURE_CUBE_ARRAY: {
            const RHITextureCubeArray *textureCubeArray = static_cast<RHITextureCubeArray *>(resource);
            vkImageView = textureCubeArray->get_vk_image_view();
            vkImageLayout = textureCubeArray->get_vk_image_layout();
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
          case rhi::ResourceType::INDIRECT_BUFFER:
          case rhi::ResourceType::TEXTURE_BUFFER:
          case rhi::ResourceType::STRUCTURED_BUFFER:
          case rhi::ResourceType::UNIFORM_BUFFER:
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
            BE_ASSERT(false, "Invalid Vulkan RHI implementation resource type")
            break;
        }

        // Get the sampler state
        const RHISamplerState *samplerState = (nullptr != mSamplerStates) ? static_cast<const RHISamplerState *>(mSamplerStates[resourceIndex]) : nullptr;

        // Update Vulkan descriptor sets
        const VkDescriptorImageInfo vkDescriptorImageInfo = {
          (nullptr != samplerState) ? samplerState->get_vk_sampler() : VK_NULL_HANDLE,  // sampler (VkSampler)
          vkImageView,                                // imageView (VkImageView)
          vkImageLayout                                // imageLayout (VkImageLayout)
        };
        const VkWriteDescriptorSet vkWriteDescriptorSet = {
          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                    // sType (VkStructureType)
          nullptr,                                                  // pNext (const void*)
          mVkDescriptorSet,                                              // dstSet (VkDescriptorSet)
          resourceIndex,                                                // dstBinding (core::uint32)
          0,                                                      // dstArrayElement (core::uint32)
          1,                                                      // descriptorCount (core::uint32)
          (nullptr != samplerState) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                                    : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,  // descriptorType (VkDescriptorType)
          &vkDescriptorImageInfo,                                            // pImageInfo (const VkDescriptorImageInfo*)
          nullptr,                                                  // pBufferInfo (const VkDescriptorBufferInfo*)
          nullptr                                                    // pTexelBufferView (const VkBufferView*)
        };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case rhi::ResourceType::SAMPLER_STATE:
        // Nothing to do in here, Vulkan is using combined image samplers
        break;

      case rhi::ResourceType::ROOT_SIGNATURE:
      case rhi::ResourceType::RESOURCE_GROUP:
      case rhi::ResourceType::GRAPHICS_PROGRAM:
      case rhi::ResourceType::VERTEX_ARRAY:
      case rhi::ResourceType::RENDER_PASS:
      case rhi::ResourceType::QUERY_POOL:
      case rhi::ResourceType::SWAP_CHAIN:
      case rhi::ResourceType::FRAMEBUFFER:
      case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
      case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
      case rhi::ResourceType::VERTEX_SHADER:
      case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
      case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case rhi::ResourceType::GEOMETRY_SHADER:
      case rhi::ResourceType::FRAGMENT_SHADER:
      case rhi::ResourceType::TASK_SHADER:
      case rhi::ResourceType::MESH_SHADER:
      case rhi::ResourceType::COMPUTE_SHADER:
        BE_ASSERT(false, "Invalid Vulkan RHI implementation resource type")
        break;
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Resource group", 17)	// 17 = "Resource group: " including terminating zero
    VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, (core::uint64)mVkDescriptorSet, detailedDebugName);
  }
#endif
}

RHIResourceGroup::~RHIResourceGroup() {
  // Remove our reference from the RHI resources
  const rhi::RHIContext &context = get_rhi().get_context();
  if (nullptr != mSamplerStates) {
    for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      rhi::RHISamplerState *samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->release();
      }
    }
    re_free(mSamplerStates);
  }
  for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
    mResources[resourceIndex]->release();
  }
  re_free(mResources);

  // Free Vulkan descriptor set
  if (VK_NULL_HANDLE != mVkDescriptorSet) {
    vkFreeDescriptorSets(static_cast<RHIDevice &>(mRootSignature.get_rhi()).get_vulkan_context().get_vk_device(),
                         mRootSignature.get_vk_descriptor_pool(), 1, &mVkDescriptorSet);
  }
  mRootSignature.release();
}

VkDescriptorSet RHIResourceGroup::get_vk_descriptor_set() const {
  return mVkDescriptorSet;
}

void RHIResourceGroup::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
