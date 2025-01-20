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
#include "rhi_vulkan/rhiroot_signature.h"
#include "rhi_vulkan/rhiresource_group.h"
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
RHIRootSignature::RHIRootSignature(RHIDevice &vulkanRhi, const rhi::RootSignatureDescriptor &rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIRootSignature(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mRootSignature(rootSignature)
, mVkPipelineLayout(VK_NULL_HANDLE)
, mVkDescriptorPool(VK_NULL_HANDLE) {
  static constexpr core::uint32 maxSets = 4242;  // TODO(naetherm) We probably need to get this provided from the outside

  // Copy the parameter data
  const rhi::RHIContext &context = vulkanRhi.get_context();
  const core::uint32 numberOfRootParameters = mRootSignature.numberOfParameters;
  if (numberOfRootParameters > 0) {
    mRootSignature.parameters = re_typed_alloc<rhi::RootParameter>(numberOfRootParameters);
    rhi::RootParameter *destinationRootParameters = const_cast<rhi::RootParameter *>(mRootSignature.parameters);
    memcpy(destinationRootParameters, rootSignature.parameters, sizeof(rhi::RootParameter) * numberOfRootParameters);

    // Copy the descriptor table data
    for (core::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      rhi::RootParameter &destinationRootParameter = destinationRootParameters[rootParameterIndex];
      const rhi::RootParameter &sourceRootParameter = rootSignature.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType) {
        const core::uint32 numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
        destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(re_typed_alloc<rhi::DescriptorRange>(numberOfDescriptorRanges));
        memcpy(reinterpret_cast<rhi::DescriptorRange *>(destinationRootParameter.descriptorTable.descriptorRanges),
               reinterpret_cast<const rhi::DescriptorRange *>(sourceRootParameter.descriptorTable.descriptorRanges),
               sizeof(rhi::DescriptorRange) * numberOfDescriptorRanges);
      }
    }
  }

  { // Copy the static sampler data
    const core::uint32 numberOfStaticSamplers = mRootSignature.numberOfStaticSamplers;
    if (numberOfStaticSamplers > 0) {
      mRootSignature.staticSamplers = re_typed_alloc<rhi::StaticSampler>(numberOfStaticSamplers);
      memcpy(const_cast<rhi::StaticSampler *>(mRootSignature.staticSamplers), rootSignature.staticSamplers,
             sizeof(rhi::StaticSampler) * numberOfStaticSamplers);
    }
  }

  // Create the Vulkan descriptor set layout
  const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
  VkDescriptorSetLayouts vkDescriptorSetLayouts;
  core::uint32 numberOfUniformTexelBuffers = 0;   // "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
  core::uint32 numberOfStorageTexelBuffers = 0;   // "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
  core::uint32 numberOfStorageImage = 0;          // "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
  core::uint32 numberOfStorageBuffers = 0;        // "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
  core::uint32 numberOfUniformBuffers = 0;        // "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
  core::uint32 numberOfCombinedImageSamplers = 0; // "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
  if (numberOfRootParameters > 0) {
    // Fill the Vulkan descriptor set layout bindings
    vkDescriptorSetLayouts.reserve(numberOfRootParameters);
    mVkDescriptorSetLayouts.resize(numberOfRootParameters);
    std::fill(mVkDescriptorSetLayouts.begin(), mVkDescriptorSetLayouts.end(), static_cast<VkDescriptorSetLayout>(VK_NULL_HANDLE));  // TODO(naetherm) Get rid of this
    typedef core::Vector<VkDescriptorSetLayoutBinding> VkDescriptorSetLayoutBindings;
    VkDescriptorSetLayoutBindings vkDescriptorSetLayoutBindings;
    vkDescriptorSetLayoutBindings.reserve(numberOfRootParameters);
    for (core::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      vkDescriptorSetLayoutBindings.clear();

      // TODO(naetherm) For now we only support descriptor tables
      const rhi::RootParameter &rootParameter = rootSignature.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        // Process descriptor ranges
        const rhi::DescriptorRange *descriptorRange = reinterpret_cast<const rhi::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges);
        for (core::uint32 descriptorRangeIndex = 0; descriptorRangeIndex <
                                                rootParameter.descriptorTable.numberOfDescriptorRanges; ++descriptorRangeIndex, ++descriptorRange) {
          // Evaluate parameter type
          VkDescriptorType vkDescriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
          switch (descriptorRange->resourceType) {
            case rhi::ResourceType::TEXTURE_BUFFER:
              BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         rhi::DescriptorRangeType::UAV == descriptorRange->rangeType,
                        "Vulkan RHI implementation: Invalid descriptor range type")
              if (rhi::DescriptorRangeType::SRV == descriptorRange->rangeType) {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                ++numberOfUniformTexelBuffers;
              } else {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                ++numberOfStorageTexelBuffers;
              }
              break;

            case rhi::ResourceType::VERTEX_BUFFER:
            case rhi::ResourceType::INDEX_BUFFER:
            case rhi::ResourceType::STRUCTURED_BUFFER:
            case rhi::ResourceType::INDIRECT_BUFFER:
              BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         rhi::DescriptorRangeType::UAV == descriptorRange->rangeType,
                        "Vulkan RHI implementation: Invalid descriptor range type")
              vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
              ++numberOfStorageBuffers;
              break;

            case rhi::ResourceType::UNIFORM_BUFFER:
              BE_ASSERT(rhi::DescriptorRangeType::UBV == descriptorRange->rangeType ||
                         rhi::DescriptorRangeType::UAV == descriptorRange->rangeType,
                        "Vulkan RHI implementation: Invalid descriptor range type")
              vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
              ++numberOfUniformBuffers;
              break;

            case rhi::ResourceType::TEXTURE_1D:
            case rhi::ResourceType::TEXTURE_1D_ARRAY:
            case rhi::ResourceType::TEXTURE_2D:
            case rhi::ResourceType::TEXTURE_2D_ARRAY:
            case rhi::ResourceType::TEXTURE_3D:
            case rhi::ResourceType::TEXTURE_CUBE:
            case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
              BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         rhi::DescriptorRangeType::UAV == descriptorRange->rangeType,
                        "Vulkan RHI implementation: Invalid descriptor range type")
              if (rhi::DescriptorRangeType::SRV == descriptorRange->rangeType) {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ++numberOfCombinedImageSamplers;
              } else {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                ++numberOfStorageImage;
              }
              break;

            case rhi::ResourceType::SAMPLER_STATE:
              // Nothing here due to usage of "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
              BE_ASSERT(rhi::DescriptorRangeType::SAMPLER == descriptorRange->rangeType,
                        "Vulkan RHI implementation: Invalid descriptor range type")
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
              BE_ASSERT(false, "Vulkan RHI implementation: Invalid resource type")
              break;
          }

          // Evaluate shader visibility
          VkShaderStageFlags vkShaderStageFlags = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
          switch (descriptorRange->shaderVisibility) {
            case rhi::ShaderVisibility::ALL:
              vkShaderStageFlags = VK_SHADER_STAGE_ALL;
              break;

            case rhi::ShaderVisibility::VERTEX:
              vkShaderStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
              break;

            case rhi::ShaderVisibility::TESSELLATION_CONTROL:
              vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
              break;

            case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
              vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
              break;

            case rhi::ShaderVisibility::GEOMETRY:
              vkShaderStageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
              break;

            case rhi::ShaderVisibility::FRAGMENT:
              vkShaderStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
              break;

            case rhi::ShaderVisibility::TASK:
              vkShaderStageFlags = VK_SHADER_STAGE_TASK_BIT_NV;
              break;

            case rhi::ShaderVisibility::MESH:
              vkShaderStageFlags = VK_SHADER_STAGE_MESH_BIT_NV;
              break;

            case rhi::ShaderVisibility::COMPUTE:
              vkShaderStageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
              break;

            case rhi::ShaderVisibility::ALL_GRAPHICS:
              vkShaderStageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
              break;
          }

          // Add the Vulkan descriptor set layout binding
          if (VK_DESCRIPTOR_TYPE_MAX_ENUM != vkDescriptorType) {
            const VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding = {
              descriptorRangeIndex,  // binding (core::uint32)
              vkDescriptorType,    // descriptorType (VkDescriptorType)
              1,            // descriptorCount (core::uint32)
              vkShaderStageFlags,    // stageFlags (VkShaderStageFlags)
              nullptr          // pImmutableSamplers (const VkSampler*)
            };
            vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);
          }
        }
      }

      // Create the Vulkan descriptor set layout
      if (!vkDescriptorSetLayoutBindings.empty()) {
        const VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo = {
          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,      // sType (VkStructureType)
          nullptr,                            // pNext (const void*)
          0,                                // flags (VkDescriptorSetLayoutCreateFlags)
          static_cast<core::uint32>(vkDescriptorSetLayoutBindings.size()),  // bindingCount (core::uint32)
          vkDescriptorSetLayoutBindings.data()              // pBindings (const VkDescriptorSetLayoutBinding*)
        };
        if (
          vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo,
                                      vulkanRhi.get_vk_allocation_callbacks(),
                                      &mVkDescriptorSetLayouts[rootParameterIndex]) != VK_SUCCESS) {
          BE_LOG(Critical, "Failed to create the Vulkan descriptor set layout")
        }
        vkDescriptorSetLayouts.push_back(mVkDescriptorSetLayouts[rootParameterIndex]);
      }
    }
  }

  { // Create the Vulkan pipeline layout
    const VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo = {
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,                // sType (VkStructureType)
      nullptr,                                  // pNext (const void*)
      0,                                      // flags (VkPipelineLayoutCreateFlags)
      static_cast<core::uint32>(vkDescriptorSetLayouts.size()),            // setLayoutCount (core::uint32)
      vkDescriptorSetLayouts.empty() ? nullptr
                                     : vkDescriptorSetLayouts.data(),  // pSetLayouts (const VkDescriptorSetLayout*)
      0,                                      // pushConstantRangeCount (core::uint32)
      nullptr                                    // pPushConstantRanges (const VkPushConstantRange*)
    };
    if (vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mVkPipelineLayout) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to create the Vulkan pipeline layout")
    }
  }

  { // Create the Vulkan descriptor pool
    typedef core::Array<VkDescriptorPoolSize, 3> VkDescriptorPoolSizes;
    VkDescriptorPoolSizes vkDescriptorPoolSizes;
    core::uint32 numberOfVkDescriptorPoolSizes = 0;

    // "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
    if (numberOfCombinedImageSamplers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfCombinedImageSamplers;    // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
    if (numberOfUniformTexelBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformTexelBuffers;  // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
    if (numberOfStorageTexelBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageTexelBuffers;  // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
    if (numberOfUniformBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformBuffers;  // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
    if (numberOfStorageImage > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageImage;    // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
    if (numberOfStorageBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageBuffers;  // descriptorCount (core::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // Create the Vulkan descriptor pool
    if (numberOfVkDescriptorPoolSizes > 0) {
      const VkDescriptorPoolCreateInfo VkDescriptorPoolCreateInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,    // sType (VkStructureType)
        nullptr,                      // pNext (const void*)
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,  // flags (VkDescriptorPoolCreateFlags)
        maxSets,                      // maxSets (core::uint32)
        numberOfVkDescriptorPoolSizes,            // poolSizeCount (core::uint32)
        vkDescriptorPoolSizes.data()            // pPoolSizes (const VkDescriptorPoolSize*)
      };
      if (vkCreateDescriptorPool(vkDevice, &VkDescriptorPoolCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mVkDescriptorPool) != VK_SUCCESS) {
        BE_LOG(Critical, "Failed to create the Vulkan descriptor pool")
      }
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Root signature", 17)	// 17 = "Root signature: " including terminating zero
    for (VkDescriptorSetLayout vkDescriptorSetLayout : mVkDescriptorSetLayouts)
    {
      VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, (core::uint64)vkDescriptorSetLayout, detailedDebugName);
    }
    VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, (core::uint64)mVkPipelineLayout, detailedDebugName);
    VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, (core::uint64)mVkDescriptorPool, detailedDebugName);
  }
#endif
}

RHIRootSignature::~RHIRootSignature() {
  const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();

  // Destroy the Vulkan descriptor pool
  if (VK_NULL_HANDLE != mVkDescriptorPool) {
    vkDestroyDescriptorPool(vkDevice, mVkDescriptorPool, vulkanRhi.get_vk_allocation_callbacks());
  }

  // Destroy the Vulkan pipeline layout
  if (VK_NULL_HANDLE != mVkPipelineLayout) {
    vkDestroyPipelineLayout(vkDevice, mVkPipelineLayout, vulkanRhi.get_vk_allocation_callbacks());
  }

  // Destroy the Vulkan descriptor set layout
  for (VkDescriptorSetLayout vkDescriptorSetLayout: mVkDescriptorSetLayouts) {
    if (VK_NULL_HANDLE != vkDescriptorSetLayout) {
      vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, vulkanRhi.get_vk_allocation_callbacks());
    }
  }

  // Destroy the root signature data
  const rhi::RHIContext &context = vulkanRhi.get_context();
  if (nullptr != mRootSignature.parameters) {
    for (core::uint32 rootParameterIndex = 0;
         rootParameterIndex < mRootSignature.numberOfParameters; ++rootParameterIndex) {
      const rhi::RootParameter &rootParameter = mRootSignature.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        re_free(reinterpret_cast<rhi::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges));
      }
    }
    re_free(const_cast<rhi::RootParameter *>(mRootSignature.parameters));
  }
  re_free(const_cast<rhi::StaticSampler *>(mRootSignature.staticSamplers));
}


// TODO(naetherm) Try to somehow simplify the internal dependencies to be able to put this method directly into the class
rhi::RHIResourceGroup *RHIRootSignature::create_resource_group(core::uint32 rootParameterIndex, core::uint32 numberOfResources, rhi::RHIResource **resources, rhi::RHISamplerState **samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  const rhi::RHIContext &context = vulkanRhi.get_context();

  // Sanity checks
  BE_ASSERT(VK_NULL_HANDLE != mVkDescriptorPool, "The Vulkan descriptor pool instance must be valid")
  BE_ASSERT(rootParameterIndex < mVkDescriptorSetLayouts.size(), "The Vulkan root parameter index is out-of-bounds")
  BE_ASSERT(numberOfResources > 0, "The number of Vulkan resources must not be zero")
  BE_ASSERT(nullptr != resources, "The Vulkan resource pointers must be valid")

  // Allocate Vulkan descriptor set
  VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
  if ((*resources)->get_resource_type() != rhi::ResourceType::SAMPLER_STATE) {
    const VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,  // sType (VkStructureType)
      nullptr,                    // pNext (const void*)
      mVkDescriptorPool,                // descriptorPool (VkDescriptorPool)
      1,                        // descriptorSetCount (core::uint32)
      &mVkDescriptorSetLayouts[rootParameterIndex]  // pSetLayouts (const VkDescriptorSetLayout*)
    };
    if (vkAllocateDescriptorSets(vulkanRhi.get_vulkan_context().get_vk_device(), &vkDescriptorSetAllocateInfo, &vkDescriptorSet) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to allocate the Vulkan descriptor set")
    }
  }

  // Create resource group
  return new RHIResourceGroup(*this, rootParameterIndex, vkDescriptorSet, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

const rhi::RootSignatureDescriptor& RHIRootSignature::get_root_signature() const {
  return mRootSignature;
}

VkPipelineLayout RHIRootSignature::get_vk_pipeline_layout() const {
  return mVkPipelineLayout;
}

VkDescriptorPool RHIRootSignature::get_vk_descriptor_pool() const {
  return mVkDescriptorPool;
}

void RHIRootSignature::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
