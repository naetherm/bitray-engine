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
#include "rhi_vulkan/query/rhiquery_pool.h"
#include "rhi_vulkan/rhidynamicrhi.h"
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
RHIQueryPool::RHIQueryPool(RHIDevice &vulkanRhi, rhi::QueryType queryType, core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIQueryPool(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mQueryType(queryType)
, mVkQueryPool(VK_NULL_HANDLE) {
  // Get Vulkan query pool create information
  VkQueryPoolCreateInfo vkQueryPoolCreateInfo;
  vkQueryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;  // VkStructureType
  vkQueryPoolCreateInfo.pNext = nullptr;                    // const void*
  vkQueryPoolCreateInfo.flags = 0;                      // VkQueryPoolCreateFlags
  vkQueryPoolCreateInfo.queryCount = numberOfQueries;                // core::uint32
  switch (queryType) {
    case rhi::QueryType::OCCLUSION:
      vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_OCCLUSION;  // VkQueryType
      vkQueryPoolCreateInfo.pipelineStatistics = 0;            // VkQueryPipelineStatisticFlags
      break;

    case rhi::QueryType::PIPELINE_STATISTICS:
      // This setup results in the same structure layout as used by "D3D11_QUERY_DATA_PIPELINE_STATISTICS" which we use for "rhi::PipelineStatisticsQueryResult"
      vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;  // VkQueryType
      vkQueryPoolCreateInfo.pipelineStatistics =                    // VkQueryPipelineStatisticFlags
        VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
        VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
        VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
        VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT |
        VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT |
        VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
        VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
        VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
        VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT |
        VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT |
        VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
      break;

    case rhi::QueryType::TIMESTAMP:
      vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;  // VkQueryType
      vkQueryPoolCreateInfo.pipelineStatistics = 0;            // VkQueryPipelineStatisticFlags
      break;
  }

  // Create Vulkan query pool
  if (vkCreateQueryPool(vulkanRhi.get_vulkan_context().get_vk_device(), &vkQueryPoolCreateInfo,
                        vulkanRhi.get_vk_allocation_callbacks(), &mVkQueryPool) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT) {
      switch (queryType) {
        case rhi::QueryType::OCCLUSION:
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
          VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (core::uint64)mVkQueryPool, detailedDebugName);
          break;
        }

        case rhi::QueryType::PIPELINE_STATISTICS:
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Pipeline statistics query", 28)	// 28 = "Pipeline statistics query: " including terminating zero
          VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (core::uint64)mVkQueryPool, detailedDebugName);
          break;
        }

        case rhi::QueryType::TIMESTAMP:
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
          VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (core::uint64)mVkQueryPool, detailedDebugName);
          break;
        }
      }
    }
#endif
  } else {
    BE_LOG(Critical, "Failed to create Vulkan query pool")
  }
}

/**
*  @brief
*    Destructor
*/
RHIQueryPool::~RHIQueryPool() {
  // Destroy Vulkan query pool instance
  if (VK_NULL_HANDLE != mVkQueryPool) {
    const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
    vkDestroyQueryPool(vulkanRhi.get_vulkan_context().get_vk_device(), mVkQueryPool, vulkanRhi.get_vk_allocation_callbacks());
  }
}


rhi::QueryType RHIQueryPool::get_query_type() const {
  return mQueryType;
}

VkQueryPool RHIQueryPool::get_vk_query_pool() const {
  return mVkQueryPool;
}

void RHIQueryPool::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
