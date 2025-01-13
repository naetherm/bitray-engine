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
#include "rhi_opengl/rhi_opengl.h"
#include "rhi_opengl/state/rhisampler_state.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL sampler state class, traditional bind version to emulate a sampler object
 */
class RHISamplerStateBind final : public RHISamplerState {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] samplerState
   * Sampler state to use
   */
  RHISamplerStateBind(RHIDevice &openGLRhi,
                      const rhi::SamplerStateDescriptor &samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  ~RHISamplerStateBind() override = default;

  /**
   * @brief
   * Set the OpenGL sampler states
   */
  void set_opengl_sampler_states() const;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHISamplerStateBind(const RHISamplerStateBind &source) = delete;

  RHISamplerStateBind &operator=(const RHISamplerStateBind &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // "rhi::SamplerState" translated into OpenGL
  GLint mOpenGLMagFilterMode;    ///< rhi::SamplerState::filter
  GLint mOpenGLMinFilterMode;    ///< rhi::SamplerState::filter
  GLint mOpenGLTextureAddressModeS;  ///< rhi::SamplerState::addressU
  GLint mOpenGLTextureAddressModeT;  ///< rhi::SamplerState::addressV
  GLint mOpenGLTextureAddressModeR;  ///< rhi::SamplerState::addressW
  float mMipLodBias;          ///< rhi::SamplerState::mipLodBias
  float mMaxAnisotropy;        ///< rhi::SamplerState::maxAnisotropy
  GLint mOpenGLCompareMode;      ///< rhi::SamplerState::comparisonFunc
  GLenum mOpenGLComparisonFunc;    ///< rhi::SamplerState::comparisonFunc
  float mBorderColor[4];        ///< rhi::SamplerState::borderColor[4]
  float mMinLod;            ///< rhi::SamplerState::minLod
  float mMaxLod;            ///< rhi::SamplerState::maxLod


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
