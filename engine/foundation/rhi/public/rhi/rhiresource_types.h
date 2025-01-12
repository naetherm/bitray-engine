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
#include "rhi/rhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Resource type
 */
enum class ResourceType {
  /// Root signature
  ROOT_SIGNATURE = 0,
  /// Resource group
  RESOURCE_GROUP = 1,
  /// Graphics program, "rhi::IShader"-related
  GRAPHICS_PROGRAM = 2,
  /// Vertex array object (VAO, input-assembler (IA) stage), "rhi::IBuffer"-related
  VERTEX_ARRAY = 3,
  /// Render pass
  RENDER_PASS = 4,
  /// Asynchronous query pool
  QUERY_POOL = 5,    
  // IRenderTarget
  /// Swap chain
  SWAP_CHAIN = 6,
  /// Framebuffer object (FBO)
  FRAMEBUFFER = 7,    
  // IBuffer
  /// Vertex buffer object (VBO, input-assembler (IA) stage)
  VERTEX_BUFFER = 8,
  /// Index buffer object (IBO, input-assembler (IA) stage)
  INDEX_BUFFER = 9,
  /// Texture buffer object (TBO)
  TEXTURE_BUFFER = 10,
  /// Structured buffer object (SBO)
  STRUCTURED_BUFFER = 11,
  /// Indirect buffer object
  INDIRECT_BUFFER = 12,
  /// Uniform buffer object (UBO, "constant buffer" in Direct3D terminology)
  UNIFORM_BUFFER = 13,  
  // ITexture
  /// Texture 1D
  TEXTURE_1D = 14,
  /// Texture 1D array
  TEXTURE_1D_ARRAY = 15,
  /// Texture 2D
  TEXTURE_2D = 16,
  /// Texture 2D array
  TEXTURE_2D_ARRAY = 17,
  /// Texture 3D
  TEXTURE_3D = 18,
  /// Texture cube
  TEXTURE_CUBE = 19,
  /// Texture cube array
  TEXTURE_CUBE_ARRAY = 20,  
  // IState
  // IPipelineState
  /// Graphics pipeline state (PSO)
  GRAPHICS_PIPELINE_STATE = 21,
  /// Compute pipeline state (PSO)
  COMPUTE_PIPELINE_STATE = 22,
  /// Sampler state
  SAMPLER_STATE = 23,  
  // IShader
  /// Vertex shader (VS)
  VERTEX_SHADER = 24,
  /// Tessellation control shader (TCS, "hull shader" in Direct3D terminology)
  TESSELLATION_CONTROL_SHADER = 25,
  /// Tessellation evaluation shader (TES, "domain shader" in Direct3D terminology)
  TESSELLATION_EVALUATION_SHADER = 26,
  /// Geometry shader (GS)
  GEOMETRY_SHADER = 27,
  /// Fragment shader (FS, "pixel shader" in Direct3D terminology)
  FRAGMENT_SHADER = 28,
  /// Task shader (TS, "amplification shader" in Direct3D terminology)
  TASK_SHADER = 29,
  /// Mesh shader (MS)
  MESH_SHADER = 30,
  /// Compute shader (CS)
  COMPUTE_SHADER = 31    
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
