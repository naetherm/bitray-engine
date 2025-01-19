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
 * Shows some vertex buffer object (VBO) use cases
 *
 * @remarks
 * Demonstrates:
 * - Vertex buffer object (VBO)
 * - Vertex array object (VAO)
 * - Vertex shader (VS) and fragment shader (FS)
 * - Root signature
 * - Graphics pipeline state object (PSO)
 * - Multiple vertex attributes within a single vertex buffer object (VBO), vertex array object (VAO) is only using one vertex buffer object (VBO)
 * - One vertex buffer object (VBO) per vertex attribute, vertex array object (VAO) is using multiple vertex buffer objects (VBO)
 */
class VertexBuffer final : public ExampleBase {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
   * @brief
   * Default constructor
   */
	inline VertexBuffer(Application& cApplication, const core::String& name)
  : ExampleBase(cApplication, name) {
		// Nothing here
	}

	/**
   * @brief
   * Destructor
   */
	inline ~VertexBuffer() override {
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
	// Using one vertex buffer object (VBO)
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateVbo;
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayVbo;
	// Using multiple vertex buffer objects (VBO)
  /** Vertex array object (VAO), can be a null pointer */
	rhi::RHIVertexArrayPtr			mVertexArrayVbos;
  /** Graphics pipeline state object (PSO), can be a null pointer */
	rhi::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateVbos;
};
