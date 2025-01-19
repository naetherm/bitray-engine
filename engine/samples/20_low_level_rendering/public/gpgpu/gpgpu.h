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

#include <string_view>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace engine
{
	class RhiInstance;
}
class ExampleRunner;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * An example showing how to use do General Purpose Computation on Graphics Processing Unit (GPGPU) by using the RHI and shaders without having any output window (also known as headless rendering)
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
 * - General Purpose Computation on Graphics Processing Unit (GPGPU) by using the RHI and shaders without having any output window
 */
class Gpgpu : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Constructor
   *
   * @param[in] exampleRunner
   * Example runner
   * @param[in] rhiName
   * Case sensitive ASCII name of the RHI to instance, if null pointer or unknown RHI no RHI will be used.
   * Example RHI names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
   * @param[in] exampleName
   * Example name
   */
	Gpgpu(Application& cApplication, const core::String& name);

	/**
   * @brief
   * Destructor
   */
	inline ~Gpgpu() override {
		// The resources are released within "on_deinitialization()"
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	Gpgpu& operator=(const Gpgpu& gpgpu) = delete;

	/**
   * @brief
   * Called on application initialization
   *
   * @note
   * - When this method is called it's ensured that the RHI instance "mRhi" is valid
   */
	void on_initialization();

	/**
   * @brief
   * Called on application de-initialization
   *
   * @note
   * - When this method is called it's ensured that the RHI instance "mRhi" is valid
   */
	void on_deinitialization();

	/**
   * @brief
   * Fill command buffer content generation
   *
   * @note
   * - When this method is called it's ensured that the RHI instance "mRhi" is valid
   */
	void fillCommandBufferContentGeneration();

	/**
   * @brief
   * Fill command buffer content processing
   *
   * @note
   * - When this method is called it's ensured that the RHI instance "mRhi" is valid
   */
	void fillCommandBufferContentProcessing();

	/**
   * @brief
   * Called on application should to its job
   *
   * @note
   * - When this method is called it's ensured that the RHI instance "mRhi" is valid
   */
	void onDoJob();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  /** RHI instance, can be a null pointer */
	rhi::RHIDevicePtr			mRhi;
	const std::string_view	mExampleName;
  /** Buffer manager, can be a null pointer */
	rhi::RHIBufferManagerPtr	mBufferManager;
  /** Buffer manager, can be a null pointer */
	rhi::RHITextureManagerPtr mTextureManager;
  /** Root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr	mRootSignature;
  /** 2D texture, can be a null pointer */
	rhi::RHITexture2DPtr		mTexture2D[2];
  /** Framebuffer object (FBO), can be a null pointer */
	rhi::RHIFramebufferPtr	mFramebuffer[2];
  /** Texture group, can be a null pointer */
	rhi::RHIResourceGroupPtr	mTextureGroup;
  /** Sampler state resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr	mSamplerStateGroup;
	// Content generation
  /** Graphics pipeline state object (PSO) for content generation, can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateContentGeneration;
  /** Vertex array object (VAO) for content generation, can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayContentGeneration;
  /** Command buffer for content generation */
	rhi::RHICommandBuffer				mCommandBufferContentGeneration;
	// Content processing
  /** Graphics pipeline state object (PSO) for content processing, can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateContentProcessing;
  /** Vertex array object (VAO) for content processing, can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayContentProcessing;
  /** Command buffer for content processing */
	rhi::RHICommandBuffer				mCommandBufferContentProcessing;


};
