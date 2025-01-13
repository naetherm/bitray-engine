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
#include "rhi_opengl/state/rhisampler_state_bind.h"
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
RHISamplerStateBind::RHISamplerStateBind(RHIDevice& openGLRhi, const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHISamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLMagFilterMode(Mapping::get_opengl_mag_filter_mode(openGLRhi.get_context(), samplerState.filter))
, mOpenGLMinFilterMode(Mapping::get_opengl_min_filter_mode(openGLRhi.get_context(), samplerState.filter, samplerState.maxLod > 0.0f))
, mOpenGLTextureAddressModeS(Mapping::get_opengl_texture_address_mode(samplerState.addressU))
, mOpenGLTextureAddressModeT(Mapping::get_opengl_texture_address_mode(samplerState.addressV))
, mOpenGLTextureAddressModeR(Mapping::get_opengl_texture_address_mode(samplerState.addressW))
, mMipLodBias(samplerState.mipLodBias)
, mMaxAnisotropy(static_cast<float>(samplerState.maxAnisotropy))	// Maximum anisotropy is "core::uint32" in Direct3D 10 & 11
, mOpenGLCompareMode(Mapping::get_opengl_compare_mode(openGLRhi.get_context(), samplerState.filter))
, mOpenGLComparisonFunc(Mapping::get_opengl_comparison_func(samplerState.comparisonFunc))
, mMinLod(samplerState.minLod)
, mMaxLod(samplerState.maxLod) {
  // Sanity check
  BE_ASSERT(samplerState.maxAnisotropy <= openGLRhi.get_capabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

  // rhi::SamplerState::borderColor[4]
  mBorderColor[0] = samplerState.borderColor[0];
  mBorderColor[1] = samplerState.borderColor[1];
  mBorderColor[2] = samplerState.borderColor[2];
  mBorderColor[3] = samplerState.borderColor[3];
}

void RHISamplerStateBind::set_opengl_sampler_states() const {
  // TODO(naetherm) Support other targets, not just "GL_TEXTURE_2D"

  // rhi::SamplerState::filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mOpenGLMagFilterMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mOpenGLMinFilterMode);

  // rhi::SamplerState::addressU
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mOpenGLTextureAddressModeS);

  // rhi::SamplerState::addressV
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mOpenGLTextureAddressModeT);

  // TODO(naetherm) Support for 3D textures
  // rhi::SamplerState::addressW
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mOpenGLTextureAddressModeR);

  // rhi::SamplerState::mipLodBias
  // -> "GL_EXT_texture_lod_bias"-extension
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, mMipLodBias);

  // rhi::SamplerState::maxAnisotropy
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropy);

  // rhi::SamplerState::comparisonFunc
  // -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mOpenGLCompareMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, mOpenGLComparisonFunc);

  // rhi::SamplerState::borderColor[4]
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, mBorderColor);

  // rhi::SamplerState::minLod
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, mMinLod);

  // rhi::SamplerState::maxLod
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, mMaxLod);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
