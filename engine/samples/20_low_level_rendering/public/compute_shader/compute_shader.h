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
 * A compute shader (CS, suited e.g. for General Purpose Computation on Graphics Processing Unit (GPGPU)) example
 *
 * @remarks
 * Demonstrates compute shader use-cases:
 * - Texture image processing
 * - Multi-draw indirect buffer written by a compute shader
 *
 * Demonstrates infrastructure usage:
 * - Vertex buffer object (VBO)
 * - Index buffer object (IBO)
 * - Vertex array object (VAO)
 * - Texture buffer object (TBO)
 * - Structured buffer object (SBO)
 * - Indirect buffer
 * - Uniform buffer object (UBO)
 * - 2D texture
 * - Sampler state object (SO)
 * - Vertex shader (VS), fragment shader (FS) and compute shader (CS)
 * - Root signature
 * - Graphics pipeline state object (PSO)
 * - Framebuffer object (FBO) used for render to texture
 *
 * @note
 * - The compute shader had to be split due to OpenGL number of binding point limitations
 */
class ComputeShader final : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Default constructor
   */
	inline ComputeShader(Application& cApplication, const core::String& name)
  : ExampleBase(cApplication, name) {
		// Nothing here
	}

	/**
   * @brief
   * Destructor
   */
	inline ~ComputeShader() override {
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
  /** Graphics root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr			mGraphicsRootSignature;
  /** First compute root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr			mComputeRootSignature1;
  /** Second compute root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr			mComputeRootSignature2;
  /** Graphics framebuffer object (FBO), can be a null pointer */
	rhi::RHIFramebufferPtr			mFramebuffer;
  /** First compute resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mComputeResourceGroup1;
  /** Second compute resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mComputeResourceGroup2;
  /** Graphics resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mGraphicsResourceGroup;
  /** Graphics sampler state resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr			mGraphicsSamplerStateGroup;
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;
  /** First compute pipeline state object (PSO), can be a null pointer */
	rhi::RHIComputePipelineStatePtr	mComputePipelineState1;
  /** Second compute pipeline state object (PSO), can be a null pointer */
	rhi::RHIComputePipelineStatePtr	mComputePipelineState2;
  /** Graphics vertex buffer object (VBO) read by compute shader, can be a null pointer */
	rhi::RHIVertexBufferPtr			mComputeInputVertexBuffer;
  /** Graphics vertex buffer object (VBO) written by compute shader, can be a null pointer */
	rhi::RHIVertexBufferPtr			mComputeOutputVertexBuffer;
  /** Graphics index buffer object (IBO) read by compute shader, can be a null pointer */
	rhi::RHIIndexBufferPtr			mComputeInputIndexBuffer;
  /** Graphics index buffer object (IBO) written by compute shader, can be a null pointer */
	rhi::RHIIndexBufferPtr			mComputeOutputIndexBuffer;
  /** Graphics vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArray;
  /** Graphics texture buffer (TBO) read by compute shader, can be a null pointer */
	rhi::RHITextureBufferPtr			mComputeInputTextureBuffer;
  /** Graphics texture buffer (TBO) written by compute shader, can be a null pointer */
	rhi::RHITextureBufferPtr			mComputeOutputTextureBuffer;
  /** Graphics structured buffer read by compute shader, can be a null pointer */
	rhi::RHIStructuredBufferPtr		mComputeInputStructuredBuffer;
  /** Graphics structured buffer written by compute shader, can be a null pointer */
	rhi::RHIStructuredBufferPtr		mComputeOutputStructuredBuffer;
  /** Graphics indirect buffer read by compute shader, can be a null pointer */
	rhi::RHIIndirectBufferPtr			mComputeInputIndirectBuffer;
  /** Graphics indirect buffer written by compute shader, can be a null pointer */
	rhi::RHIIndirectBufferPtr			mComputeOutputIndirectBuffer;
  /** Graphics uniform buffer (UBO) read by compute shader, can be a null pointer */
	rhi::RHIUniformBufferPtr			mComputeInputUniformBuffer;
};
