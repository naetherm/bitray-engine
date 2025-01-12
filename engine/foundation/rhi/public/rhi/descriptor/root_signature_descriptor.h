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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi/rhi.h"
#include "rhi/rhiroot_signature_types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Root signature
 *
 * @note
 * - "rhi::RootSignatureDescriptor" is not totally identical to "D3D12_ROOT_SIGNATURE_DESC" because it had to be extended by information required by OpenGL, so can't cast to Direct3D 12 structure
 * - In order to be RHI implementation independent, do always define and set samplers first
 * - "rhi::DescriptorRange": In order to be RHI implementation independent, do always provide "baseShaderRegisterName" for "rhi::DescriptorRangeType::SRV" range types
 *
 * @see
 * - "D3D12_ROOT_SIGNATURE_DESC"-documentation for details
 */
struct RootSignatureDescriptor {
  core::uint32 numberOfParameters;
  const RootParameter *parameters;
  core::uint32 numberOfStaticSamplers;
  const StaticSampler *staticSamplers;
  RootSignatureFlags::Enum flags;
};

struct RootSignatureDescriptorBuilder final : public RootSignatureDescriptor {
  static inline void initialize(
    RootSignatureDescriptor &rootSignature,
    core::uint32 _numberOfParameters,
    const RootParameter *_parameters,
    core::uint32 _numberOfStaticSamplers = 0,
    const StaticSampler *_staticSamplers = nullptr,
    RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE) {
    rootSignature.numberOfParameters = _numberOfParameters;
    rootSignature.parameters = _parameters;
    rootSignature.numberOfStaticSamplers = _numberOfStaticSamplers;
    rootSignature.staticSamplers = _staticSamplers;
    rootSignature.flags = _flags;
  }

  inline RootSignatureDescriptorBuilder() = default;

  inline explicit RootSignatureDescriptorBuilder(const RootSignatureDescriptorBuilder &) {

  }

  inline RootSignatureDescriptorBuilder(
    core::uint32 _numberOfParameters,
    const RootParameter *_parameters,
    core::uint32 _numberOfStaticSamplers = 0,
    const StaticSampler *_staticSamplers = nullptr,
    RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE) {
    initialize(_numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
  }

  inline void initialize(
    core::uint32 _numberOfParameters,
    const RootParameter *_parameters,
    core::uint32 _numberOfStaticSamplers = 0,
    const StaticSampler *_staticSamplers = nullptr,
    RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE) {
    initialize(*this, _numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
