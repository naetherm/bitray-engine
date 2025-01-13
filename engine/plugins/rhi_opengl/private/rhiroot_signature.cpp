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
#include "rhi_opengl/rhiroot_signature.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/rhiresource_group.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIRootSignature::RHIRootSignature(RHIDevice &openGLRhi, const rhi::RootSignatureDescriptor &rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIRootSignature(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
,
  mRootSignature(rootSignature) {
  const rhi::RHIContext &context = openGLRhi.get_context();

  { // Copy the parameter data
    const core::uint32 numberOfParameters = mRootSignature.numberOfParameters;
    if (numberOfParameters > 0) {
      mRootSignature.parameters = re_typed_alloc<rhi::RootParameter>(numberOfParameters);
      rhi::RootParameter *destinationRootParameters = const_cast<rhi::RootParameter *>(mRootSignature.parameters);
      memcpy(destinationRootParameters, rootSignature.parameters, sizeof(rhi::RootParameter) * numberOfParameters);

      // Copy the descriptor table data
      for (core::uint32 i = 0; i < numberOfParameters; ++i) {
        rhi::RootParameter &destinationRootParameter = destinationRootParameters[i];
        const rhi::RootParameter &sourceRootParameter = rootSignature.parameters[i];
        if (rhi::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType) {
          const core::uint32 numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
          destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(re_typed_alloc<
            rhi::DescriptorRange>(numberOfDescriptorRanges));
          memcpy(reinterpret_cast<rhi::DescriptorRange *>(destinationRootParameter.descriptorTable.descriptorRanges),
                 reinterpret_cast<const rhi::DescriptorRange *>(sourceRootParameter.descriptorTable.descriptorRanges),
                 sizeof(rhi::DescriptorRange) * numberOfDescriptorRanges);
        }
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
}

/**
 * @brief
 * Destructor
 */
RHIRootSignature::~RHIRootSignature() {
  // Destroy the root signature data
  const rhi::RHIContext &context = get_rhi().get_context();
  if (nullptr != mRootSignature.parameters) {
    for (core::uint32 i = 0; i < mRootSignature.numberOfParameters; ++i) {
      const rhi::RootParameter &rootParameter = mRootSignature.parameters[i];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        re_free(reinterpret_cast<rhi::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges));
      }
    }
    re_free(const_cast<rhi::RootParameter *>(mRootSignature.parameters));
  }
  if (mRootSignature.staticSamplers) {
    re_free(const_cast<rhi::StaticSampler *>(mRootSignature.staticSamplers));
  }
}

rhi::RHIResourceGroup *RHIRootSignature::create_resource_group(
  core::uint32 rootParameterIndex,
  core::uint32 numberOfResources,
  rhi::RHIResource **resources,
  rhi::RHISamplerState **samplerStates
  RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  rhi::RHIDevice &rhi = get_rhi();

  // Sanity checks
  BE_ASSERT(rootParameterIndex < mRootSignature.numberOfParameters, "The OpenGL root parameter index is out-of-bounds")
  BE_ASSERT(numberOfResources > 0, "The number of OpenGL resources must not be zero")
  BE_ASSERT(nullptr != resources, "The OpenGL resource pointers must be valid")

  // Create resource group
  return re_new<RHIResourceGroup>(rhi, mRootSignature, rootParameterIndex, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
