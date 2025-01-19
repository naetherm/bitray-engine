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
 * Instancing example
 *
 * @remarks
 * Demonstrates:
 * - Vertex buffer object (VBO)
 * - Index buffer object (IBO)
 * - Vertex array object (VAO)
 * - Vertex shader (VS) and fragment shader (FS)
 * - Root signature
 * - Graphics pipeline state object (PSO)
 * - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
 * - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
 */
class Instancing final : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Default constructor
   */
	inline Instancing(Application& cApplication, const core::String& name)
  : ExampleBase(cApplication, name) {
		// Nothing here
	}

	/**
   * @brief
   * Destructor
   */
	inline ~Instancing() override {
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
	rhi::RHIBufferManagerPtr	mBufferManager;
  /** Command buffer */
	rhi::RHICommandBuffer		mCommandBuffer;
  /** Root signature, can be a null pointer */
	rhi::RHIRootSignaturePtr	mRootSignature;
	// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateInstancedArrays;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayInstancedArrays;
	// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateDrawInstanced;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayDrawInstanced;


};
