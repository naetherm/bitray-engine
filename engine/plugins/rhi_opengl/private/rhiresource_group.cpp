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
#include "rhi_opengl/rhiresource_group.h"
#include "rhi_opengl/rhidynamicrhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIResourceGroup::RHIResourceGroup(rhi::RHIDevice& rhi, const rhi::RootSignatureDescriptor& rootSignature, core::uint32 rootParameterIndex, core::uint32 numberOfResources, rhi::RHIResource** resources, rhi::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIResourceGroup(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mRootParameterIndex(rootParameterIndex)
, mNumberOfResources(numberOfResources)
, mResources(re_typed_alloc<rhi::RHIResource*>(mNumberOfResources))
, mSamplerStates(nullptr)
, mResourceIndexToUniformBlockBindingIndex(nullptr) {
  // get the uniform block binding start index
  const rhi::RHIContext& context = rhi.get_context();
  core::uint32 uniformBlockBindingIndex = 0;
  for (core::uint32 currentRootParameterIndex = 0; currentRootParameterIndex < rootParameterIndex; ++currentRootParameterIndex) {
    const rhi::RootParameter& rootParameter = rootSignature.parameters[currentRootParameterIndex];
    if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
      BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
      const core::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
      for (core::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex) {
        if (rhi::DescriptorRangeType::UBV == reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex].rangeType) {
          ++uniformBlockBindingIndex;
        }
      }
    }
  }

  // Process all resources and add our reference to the RHI resource
  const rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
  for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources) {
    rhi::RHIResource* resource = *resources;
    BE_ASSERT(nullptr != resource, "Invalid OpenGL resource")
    mResources[resourceIndex] = resource;
    resource->add_ref();

    // Uniform block binding index handling
    const rhi::DescriptorRange& descriptorRange = reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];
    if (rhi::DescriptorRangeType::UBV == descriptorRange.rangeType) {
      if (nullptr == mResourceIndexToUniformBlockBindingIndex) {
        mResourceIndexToUniformBlockBindingIndex = re_typed_alloc<core::uint32>(mNumberOfResources);
        memset(mResourceIndexToUniformBlockBindingIndex, 0, sizeof(core::uint32) * mNumberOfResources);
      }
      mResourceIndexToUniformBlockBindingIndex[resourceIndex] = uniformBlockBindingIndex;
      ++uniformBlockBindingIndex;
    }
  }
  if (nullptr != samplerStates) {
    mSamplerStates = re_typed_alloc<rhi::RHISamplerState*>(mNumberOfResources);
    for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      rhi::RHISamplerState* samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->add_ref();
      }
    }
  }
}

/**
 * @brief
 * Destructor
 */
RHIResourceGroup::~RHIResourceGroup() {
  // Remove our reference from the RHI resources
  const rhi::RHIContext& context = get_rhi().get_context();
  if (nullptr != mSamplerStates) {
    for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      rhi::RHISamplerState* samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->release();
      }
    }
    re_free(mSamplerStates);
  }
  for (core::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
    mResources[resourceIndex]->release();
  }
  if (mResources) {
    re_free(mResources);
  }
  if (mResourceIndexToUniformBlockBindingIndex) {
    re_free(mResourceIndexToUniformBlockBindingIndex);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
