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
#include "instanced_cubes/icube_renderer.h"

#include <rhi/rhi_headers.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class BatchDrawInstanced;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Cube renderer class using instancing
 *
 * @remarks
 * Required RHI features:
 * - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
 * - 2D array texture
 * - Texture buffer
 */
class CubeRendererDrawInstanced final : public ICubeRenderer {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * RHI instance to use
   * @param[in] renderPass
   * Render pass to use
   * @param[in] numberOfTextures
   * Number of textures, must be <ICubeRenderer::MAXIMUM_NUMBER_OF_TEXTURES
   * @param[in] sceneRadius
   * Scene radius
   */
	CubeRendererDrawInstanced(rhi::RHIDevice& rhi, rhi::RHIRenderPass& renderPass, core::uint32 numberOfTextures, core::uint32 sceneRadius);

	/**
   * @brief
   * Destructor
   */
	virtual ~CubeRendererDrawInstanced() override;


//[-------------------------------------------------------]
//[ Public virtual ICubeRenderer methods                  ]
//[-------------------------------------------------------]
public:
	virtual void setNumberOfCubes(core::uint32 numberOfCubes) override;
	virtual void fillCommandBuffer(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ, rhi::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit CubeRendererDrawInstanced(const CubeRendererDrawInstanced& source) = delete;
	CubeRendererDrawInstanced& operator =(const CubeRendererDrawInstanced& source) = delete;
	void fillReusableCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  /** RHI instance to use, always valid */
	rhi::RHIDevicePtr			 mRhi;
  /** Render pass to use, always valid */
	rhi::RHIRenderPass&		 mRenderPass;
  /** Buffer manager, can be a null pointer */
	rhi::RHIBufferManagerPtr	 mBufferManager;
  /** Texture manager, can be a null pointer */
	rhi::RHITextureManagerPtr  mTextureManager;
  /** Command buffer which is recorded once and then used multiple times */
	rhi::RHICommandBuffer		 mCommandBuffer;
  /** Number of textures */
	core::uint32				 mNumberOfTextures;
  /** Scene radius */
	core::uint32				 mSceneRadius;
  /** Maximum number of instances per batch */
	core::uint32				 mMaximumNumberOfInstancesPerBatch;
  /** Root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr	 mRootSignature;
  /** 2D texture array, can be a null pointer */
	rhi::RHITexture2DArrayPtr  mTexture2DArray;
  /** Static vertex shader uniform buffer object (UBO), can be a null pointer */
	rhi::RHIUniformBufferPtr	 mUniformBufferStaticVs;
  /** Dynamic vertex shader uniform buffer object (UBO), can be a null pointer */
	rhi::RHIUniformBufferPtr	 mUniformBufferDynamicVs;
  /** Dynamic fragment shader uniform buffer object (UBO), can be a null pointer */
	rhi::RHIUniformBufferPtr	 mUniformBufferDynamicFs;
  /** Resource group with vertex shader visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr	 mResourceGroupVs;
  /** Resource group with fragment shader visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr	 mResourceGroupFs;
  /** Sampler state resource group, can be a null pointer */
	rhi::RHIResourceGroupPtr	 mSamplerStateGroup;
  /** Graphics program, can be a null pointer */
	rhi::RHIGraphicsProgramPtr mGraphicsProgram;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr	 mVertexArray;
  /** Current number of batches */
	core::uint32				 mNumberOfBatches;
  /** Batches, can be a null pointer */
	BatchDrawInstanced*		 mBatches;
};
