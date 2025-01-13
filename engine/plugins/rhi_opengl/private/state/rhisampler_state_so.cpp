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
#include "rhi_opengl/state/rhisampler_state_so.h"
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
RHISamplerStateSo::RHISamplerStateSo(RHIDevice& openGLRhi, const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHISamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLSampler(0) {
  // Sanity check
  BE_ASSERT(samplerState.maxAnisotropy <= openGLRhi.get_capabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

  // Create the OpenGL sampler
  glGenSamplers(1, &mOpenGLSampler);

  // rhi::SamplerState::filter
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MAG_FILTER,Mapping::get_opengl_mag_filter_mode(openGLRhi.get_context(), samplerState.filter));
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MIN_FILTER, Mapping::get_opengl_min_filter_mode(openGLRhi.get_context(), samplerState.filter, samplerState.maxLod > 0.0f));

  // rhi::SamplerState::addressU
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_S, Mapping::get_opengl_texture_address_mode(samplerState.addressU));

  // rhi::SamplerState::addressV
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_T, Mapping::get_opengl_texture_address_mode(samplerState.addressV));

  // rhi::SamplerState::addressW
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_R, Mapping::get_opengl_texture_address_mode(samplerState.addressW));

  // rhi::SamplerState::mipLodBias
  // -> "GL_EXT_texture_lod_bias"-extension
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

  // rhi::SamplerState::maxAnisotropy
  // -> Maximum anisotropy is "core::uint32" in Direct3D 10 & 11
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(samplerState.maxAnisotropy));

  // rhi::SamplerState::comparisonFunc
  // -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_MODE, Mapping::get_opengl_compare_mode(openGLRhi.get_context(), samplerState.filter));
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(Mapping::get_opengl_comparison_func( samplerState.comparisonFunc)));

  // rhi::SamplerState::borderColor[4]
  glSamplerParameterfv(mOpenGLSampler, GL_TEXTURE_BORDER_COLOR, samplerState.borderColor);

  // rhi::SamplerState::minLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MIN_LOD, samplerState.minLod);

  // rhi::SamplerState::maxLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_LOD, samplerState.maxLod);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
    glObjectLabel(GL_SAMPLER, mOpenGLSampler, -1, detailedDebugName);
  }
#endif
}

RHISamplerStateSo::~RHISamplerStateSo() {
  // Destroy the OpenGL sampler
  // -> Silently ignores 0's and names that do not correspond to existing samplers
  glDeleteSamplers(1, &mOpenGLSampler);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
