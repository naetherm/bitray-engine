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
#include "example_base.h"

#include <rhi/rhi_headers.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * An example showing how to render into a texture
 *
 * @remarks
 * Demonstrates:
 * - Vertex buffer object (VBO)
 * - Vertex array object (VAO)
 * - 2D texture
 * - Sampler state object (SO)
 * - Vertex shader (VS) and fragment shader (FS)
 * - Root signature
 * - Graphics pipeline state object (PSO)
 * - Framebuffer object (FBO) used for render to texture
 */
class RenderToTexture final : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Default constructor
   */
	inline RenderToTexture(Application& cApplication, const core::String& name)
  : ExampleBase(cApplication, name) {
		// Nothing here
	}

	/**
   * @brief
   * Destructor
   */
	inline ~RenderToTexture() override {
		// The resources are released within "on_deinitialization()"
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	virtual void on_initialization() override;
	virtual void on_deinitialization() override;
	virtual void on_draw(rhi::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void fill_command_buffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  /** Buffer manager, can be a null pointer */
	rhi::RHIBufferManagerPtr			mBufferManager;
  /** Texture manager, can be a null pointer */
	rhi::RHITextureManagerPtr			mTextureManager;
  /** Command buffer */
	rhi::RHICommandBuffer				mCommandBuffer;
  /** Root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr			mRootSignature;
  /** Framebuffer object (FBO), can be a null pointer */
	rhi::RHIFramebufferPtr			mFramebuffer;
  /** Texture group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mTextureGroup;
  /** Sampler state resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mSamplerStateGroup;
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArray;
};
