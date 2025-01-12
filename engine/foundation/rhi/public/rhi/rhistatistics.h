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
#include <atomic>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Statistics class
 *
 * @note
 * - The data is public by intent in order to make it easier to use this class,
 *   no issues involved because the user only gets a constant instance
 */
class RHIStatistics final {

  // Public data
public:
  // Resources
  /// Current number of root signature instances
  std::atomic<core::uint32> currentNumberOfRootSignatures;
  /// Number of created root signature instances
  std::atomic<core::uint32> numberOfCreatedRootSignatures;
  /// Current number of resource group instances
  std::atomic<core::uint32> currentNumberOfResourceGroups;
  /// Number of created resource group instances
  std::atomic<core::uint32> numberOfCreatedResourceGroups;
  /// Current number of graphics program instances
  std::atomic<core::uint32> currentNumberOfGraphicsPrograms;
  /// Number of created graphics program instances
  std::atomic<core::uint32> numberOfCreatedGraphicsPrograms;
  /// Current number of vertex array object (VAO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> currentNumberOfVertexArrays;
  /// Number of created vertex array object (VAO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> numberOfCreatedVertexArrays;
  /// Current number of render pass instances
  std::atomic<core::uint32> currentNumberOfRenderPasses;
  /// Number of created render pass instances
  std::atomic<core::uint32> numberOfCreatedRenderPasses;
  /// Current number of asynchronous query pool instances
  std::atomic<core::uint32> currentNumberOfQueryPools;
  /// Number of created asynchronous query pool instances
  std::atomic<core::uint32> numberOfCreatedQueryPools;
  // IRenderTarget
  /// Current number of swap chain instances
  std::atomic<core::uint32> currentNumberOfSwapChains;
  /// Number of created swap chain instances
  std::atomic<core::uint32> numberOfCreatedSwapChains;
  /// Current number of framebuffer object (FBO) instances
  std::atomic<core::uint32> currentNumberOfFramebuffers;
  /// Number of created framebuffer object (FBO) instances
  std::atomic<core::uint32> numberOfCreatedFramebuffers;
  // IBuffer
  /// Current number of vertex buffer object (VBO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> currentNumberOfVertexBuffers;
  /// Number of created vertex buffer object (VBO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> numberOfCreatedVertexBuffers;
  /// Current number of index buffer object (IBO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> currentNumberOfIndexBuffers;
  /// Number of created index buffer object (IBO, input-assembler (IA) stage) instances
  std::atomic<core::uint32> numberOfCreatedIndexBuffers;
  /// Current number of texture buffer object (TBO) instances
  std::atomic<core::uint32> currentNumberOfTextureBuffers;
  /// Number of created texture buffer object (TBO) instances
  std::atomic<core::uint32> numberOfCreatedTextureBuffers;
  /// Current number of structured buffer object (SBO) instances
  std::atomic<core::uint32> currentNumberOfStructuredBuffers;
  /// Number of created structured buffer object (SBO) instances
  std::atomic<core::uint32> numberOfCreatedStructuredBuffers;
  /// Current number of indirect buffer object instances
  std::atomic<core::uint32> currentNumberOfIndirectBuffers;
  /// Number of created indirect buffer object instances
  std::atomic<core::uint32> numberOfCreatedIndirectBuffers;
  /// Current number of uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
  std::atomic<core::uint32> currentNumberOfUniformBuffers;
  /// Number of created uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
  std::atomic<core::uint32> numberOfCreatedUniformBuffers;
  // ITexture
  /// Current number of texture 1D instances
  std::atomic<core::uint32> currentNumberOfTexture1Ds;
  /// Number of created texture 1D instances
  std::atomic<core::uint32> numberOfCreatedTexture1Ds;
  /// Current number of texture 1D array instances
  std::atomic<core::uint32> currentNumberOfTexture1DArrays;
  /// Number of created texture 1D array instances
  std::atomic<core::uint32> numberOfCreatedTexture1DArrays;
  /// Current number of texture 2D instances
  std::atomic<core::uint32> currentNumberOfTexture2Ds;
  /// Number of created texture 2D instances
  std::atomic<core::uint32> numberOfCreatedTexture2Ds;
  /// Current number of texture 2D array instances
  std::atomic<core::uint32> currentNumberOfTexture2DArrays;
  /// Number of created texture 2D array instances
  std::atomic<core::uint32> numberOfCreatedTexture2DArrays;
  /// Current number of texture 3D instances
  std::atomic<core::uint32> currentNumberOfTexture3Ds;
  /// Number of created texture 3D instances
  std::atomic<core::uint32> numberOfCreatedTexture3Ds;
  /// Current number of texture cube instances
  std::atomic<core::uint32> currentNumberOfTextureCubes;
  /// Number of created texture cube instances
  std::atomic<core::uint32> numberOfCreatedTextureCubes;
  /// Current number of texture cube array instances
  std::atomic<core::uint32> currentNumberOfTextureCubeArrays;
  /// Number of created texture cube array instances
  std::atomic<core::uint32> numberOfCreatedTextureCubeArrays;
  // IState
  /// Current number of graphics pipeline state (PSO) instances
  std::atomic<core::uint32> currentNumberOfGraphicsPipelineStates;
  /// Number of created graphics pipeline state (PSO) instances
  std::atomic<core::uint32> numberOfCreatedGraphicsPipelineStates;
  /// Current number of compute pipeline state (PSO) instances
  std::atomic<core::uint32> currentNumberOfComputePipelineStates;
  /// Number of created compute pipeline state (PSO) instances
  std::atomic<core::uint32> numberOfCreatedComputePipelineStates;
  /// Current number of sampler state instances
  std::atomic<core::uint32> currentNumberOfSamplerStates;
  /// Number of created sampler state instances
  std::atomic<core::uint32> numberOfCreatedSamplerStates;
  // IShader
  /// Current number of vertex shader (VS) instances
  std::atomic<core::uint32> currentNumberOfVertexShaders;
  /// Number of created vertex shader (VS) instances
  std::atomic<core::uint32> numberOfCreatedVertexShaders;
  /// Current number of tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
  std::atomic<core::uint32> currentNumberOfTessellationControlShaders;
  /// Number of created tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
  std::atomic<core::uint32> numberOfCreatedTessellationControlShaders;
  /// Current number of tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
  std::atomic<core::uint32> currentNumberOfTessellationEvaluationShaders;
  /// Number of created tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
  std::atomic<core::uint32> numberOfCreatedTessellationEvaluationShaders;
  /// Current number of geometry shader (GS) instances
  std::atomic<core::uint32> currentNumberOfGeometryShaders;
  /// Number of created geometry shader (GS) instances
  std::atomic<core::uint32> numberOfCreatedGeometryShaders;
  /// Current number of fragment shader (FS, "pixel shader" in Direct3D terminology) instances
  std::atomic<core::uint32> currentNumberOfFragmentShaders;
  /// Number of created fragment shader (FS, "pixel shader" in Direct3D terminology) instances
  std::atomic<core::uint32> numberOfCreatedFragmentShaders;
  /// Current number of task shader (TS) instances
  std::atomic<core::uint32> currentNumberOfTaskShaders;
  /// Number of created task shader (TS) instances
  std::atomic<core::uint32> numberOfCreatedTaskShaders;
  /// Current number of mesh shader (MS) instances
  std::atomic<core::uint32> currentNumberOfMeshShaders;
  /// Number of created mesh shader (MS) instances
  std::atomic<core::uint32> numberOfCreatedMeshShaders;
  /// Current number of compute shader (CS) instances
  std::atomic<core::uint32> currentNumberOfComputeShaders;
  /// Number of created compute shader (CS) instances
  std::atomic<core::uint32> numberOfCreatedComputeShaders;

  // Public methods
public:
  /**
   * @brief
   * Default constructor
   */
  inline RHIStatistics()
  : currentNumberOfRootSignatures(0)
  , numberOfCreatedRootSignatures(0)
  , currentNumberOfResourceGroups(0)
  , numberOfCreatedResourceGroups(0)
  , currentNumberOfGraphicsPrograms(0)
  , numberOfCreatedGraphicsPrograms(0)
  , currentNumberOfVertexArrays(0)
  , numberOfCreatedVertexArrays(0)
  , currentNumberOfRenderPasses(0)
  , numberOfCreatedRenderPasses(0)
  , currentNumberOfQueryPools(0)
  , numberOfCreatedQueryPools(0)
// RHIRenderTarget
  , currentNumberOfSwapChains(0)
  , numberOfCreatedSwapChains(0)
  , currentNumberOfFramebuffers(0)
  , numberOfCreatedFramebuffers(0)
// RHIBuffer
  , currentNumberOfVertexBuffers(0)
  , numberOfCreatedVertexBuffers(0)
  , currentNumberOfIndexBuffers(0)
  , numberOfCreatedIndexBuffers(0)
  , currentNumberOfTextureBuffers(0)
  , numberOfCreatedTextureBuffers(0)
  , currentNumberOfStructuredBuffers(0)
  , numberOfCreatedStructuredBuffers(0)
  , currentNumberOfIndirectBuffers(0)
  , numberOfCreatedIndirectBuffers(0)
  , currentNumberOfUniformBuffers(0)
  , numberOfCreatedUniformBuffers(0)
// RHITexture
  , currentNumberOfTexture1Ds(0)
  , numberOfCreatedTexture1Ds(0)
  , currentNumberOfTexture1DArrays(0)
  , numberOfCreatedTexture1DArrays(0)
  , currentNumberOfTexture2Ds(0)
  , numberOfCreatedTexture2Ds(0)
  , currentNumberOfTexture2DArrays(0)
  , numberOfCreatedTexture2DArrays(0)
  , currentNumberOfTexture3Ds(0)
  , numberOfCreatedTexture3Ds(0)
  , currentNumberOfTextureCubes(0)
  , numberOfCreatedTextureCubes(0)
  , currentNumberOfTextureCubeArrays(0)
  , numberOfCreatedTextureCubeArrays(0)
// RHIState
  , currentNumberOfGraphicsPipelineStates(0)
  , numberOfCreatedGraphicsPipelineStates(0)
  , currentNumberOfComputePipelineStates(0)
  , numberOfCreatedComputePipelineStates(0)
  , currentNumberOfSamplerStates(0)
  , numberOfCreatedSamplerStates(0)
// RHIShader
  , currentNumberOfVertexShaders(0)
  , numberOfCreatedVertexShaders(0)
  , currentNumberOfTessellationControlShaders(0)
  , numberOfCreatedTessellationControlShaders(0)
  , currentNumberOfTessellationEvaluationShaders(0)
  , numberOfCreatedTessellationEvaluationShaders(0)
  , currentNumberOfGeometryShaders(0)
  , numberOfCreatedGeometryShaders(0)
  , currentNumberOfFragmentShaders(0)
  , numberOfCreatedFragmentShaders(0)
  , currentNumberOfTaskShaders(0)
  , numberOfCreatedTaskShaders(0)
  , currentNumberOfMeshShaders(0)
  , numberOfCreatedMeshShaders(0)
  , currentNumberOfComputeShaders(0)
  , numberOfCreatedComputeShaders(0) {

  }

  /**
   * @brief
   * Destructor
   */
  inline ~RHIStatistics() = default;

  /**
   * @brief
   * Return the number of current resource instances
   *
   * @return
   * The number of current resource instances
   *
   * @note
   * - Primarily for debugging
   * - The result is calculated by using the current statistics, do only call this method if you have to
   */
  [[nodiscard]] inline core::uint32 get_number_of_current_resources() const {
    // Calculate the current number of resource instances
    return currentNumberOfRootSignatures +
           currentNumberOfResourceGroups +
           currentNumberOfGraphicsPrograms +
           currentNumberOfVertexArrays +
           currentNumberOfRenderPasses +
           currentNumberOfQueryPools +
           // IRenderTarget
           currentNumberOfSwapChains +
           currentNumberOfFramebuffers +
           // IBuffer
           currentNumberOfVertexBuffers +
           currentNumberOfIndexBuffers +
           currentNumberOfTextureBuffers +
           currentNumberOfStructuredBuffers +
           currentNumberOfIndirectBuffers +
           currentNumberOfUniformBuffers +
           // ITexture
           currentNumberOfTexture1Ds +
           currentNumberOfTexture1DArrays +
           currentNumberOfTexture2Ds +
           currentNumberOfTexture2DArrays +
           currentNumberOfTexture3Ds +
           currentNumberOfTextureCubes +
           currentNumberOfTextureCubeArrays +
           // IState
           currentNumberOfGraphicsPipelineStates +
           currentNumberOfComputePipelineStates +
           currentNumberOfSamplerStates +
           // IShader
           currentNumberOfVertexShaders +
           currentNumberOfTessellationControlShaders +
           currentNumberOfTessellationEvaluationShaders +
           currentNumberOfGeometryShaders +
           currentNumberOfFragmentShaders +
           currentNumberOfTaskShaders +
           currentNumberOfMeshShaders +
           currentNumberOfComputeShaders;
  }
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
