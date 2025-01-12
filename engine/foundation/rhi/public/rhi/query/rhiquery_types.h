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
 * Asynchronous query type
 */
enum class QueryType {
  /// Occlusion query, result is a "core::uint64" containing the number of passed samples
  OCCLUSION = 0,
  /// Pipeline statistics query, result is a "rhi::PipelineStatisticsQueryResult"
  PIPELINE_STATISTICS = 1,
  /// Timestamp query, result is a "core::uint64" containing a timestamp in nanosecond
  TIMESTAMP = 2
};

/**
 * @brief
 * Asynchronous query control flags
 */
struct QueryControlFlags final {
  enum Enum {
    /// Query control precise
    PRECISE = 1 << 0
  };
};

/**
 * @brief
 * Asynchronous query result flags
 */
struct QueryResultFlags final {
  enum Enum {
    /// Wait for the result
    WAIT = 1 << 0
  };
};

/**
 * @brief
 * Asynchronous pipeline statistics query result
 *
 * @note
 * - This pipeline statistics structure maps directly to Direct3D 11 and Direct3D 12, do not change it
 *
 * @see
 * - "D3D11_QUERY_DATA_PIPELINE_STATISTICS"-documentation for details
 */
struct PipelineStatisticsQueryResult final {
  /// Number of vertices read by input assembler
  core::uint64 numberOfInputAssemblerVertices;
  /// Number of primitives read by the input assembler
  core::uint64 numberOfInputAssemblerPrimitives;
  /// Number of times a vertex shader was invoked
  core::uint64 numberOfVertexShaderInvocations;
  /// Number of times a geometry shader was invoked
  core::uint64 numberOfGeometryShaderInvocations;
  /// Number of primitives output by a geometry shader
  core::uint64 numberOfGeometryShaderOutputPrimitives;
  /// Number of primitives that were sent to the rasterizer
  core::uint64 numberOfClippingInputPrimitives;
  /// Number of primitives that were rendered
  core::uint64 numberOfClippingOutputPrimitives;
  /// Number of times a fragment shader (FS, "pixel shader" in Direct3D terminology) was invoked
  core::uint64 numberOfFragmentShaderInvocations;
  /// Number of times a hull shader (TCS, "hull shader" in Direct3D terminology) was invoked
  core::uint64 numberOfTessellationControlShaderInvocations;
  /// Number of times a domain shader (TES, "domain shader" in Direct3D terminology) was invoked
  core::uint64 numberOfTessellationEvaluationShaderInvocations;
  /// Number of times a compute shader was invoked
  core::uint64 numberOfComputeShaderInvocations;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
