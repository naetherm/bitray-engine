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
 * More advanced tessellation example
 *
 * @remarks
 * Demonstrates:
 * - Vertex buffer object (VBO)
 * - Index buffer object (IBO)
 * - Vertex array object (VAO)
 * - Root signature
 * - Graphics pipeline state object (PSO)
 * - Uniform buffer object (UBO)
 * - Vertex shader (VS), tessellation control shader (TCS), tessellation evaluation shader (TES), geometry shader (GS) and fragment shader (FS)
 */
class IcosahedronTessellation final : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Default constructor
   */
	inline IcosahedronTessellation(Application& cApplication, const core::String& name)
  : ExampleBase(cApplication, name)
  , mTessellationLevelOuter(2.0f)
  , mTessellationLevelInner(3.0f) {
		// Nothing here
	}

	/**
   * @brief
   * Destructor
   */
	inline ~IcosahedronTessellation() override {
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
  /** Command buffer */
	rhi::RHICommandBuffer				mCommandBuffer;
  /** Root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr			mRootSignature;
  /** Dynamic tessellation control shader uniform buffer object (UBO), can be a null pointer */
	rhi::RHIUniformBufferPtr			mUniformBufferDynamicTcs;
  /** Uniform buffer group with tessellation control shader visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr			mUniformBufferGroupTcs;
  /** Uniform buffer group with tessellation evaluation shader visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr			mUniformBufferGroupTes;
  /** Uniform buffer group with geometry visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr			mUniformBufferGroupGs;
  /** Uniform buffer group with fragment shader visibility, can be a null pointer */
	rhi::RHIResourceGroupPtr			mUniformBufferGroupFs;
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArray;
  /** Outer tessellation level */
	float							mTessellationLevelOuter;
  /** Inner tessellation level */
	float							mTessellationLevelInner;
};
