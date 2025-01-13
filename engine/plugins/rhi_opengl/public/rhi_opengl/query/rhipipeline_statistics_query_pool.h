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
#include "rhi_opengl/rhi_opengl.h"
#include "rhi_opengl/query/rhiquery_pool.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL asynchronous pipeline statistics query pool interface
 */
class RHIPipelineStatisticsQueryPool final : public RHIQueryPool {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] queryType
   * Query type
   * @param[in] numberOfQueries
   * Number of queries
   */
  RHIPipelineStatisticsQueryPool(RHIDevice &openGLRhi, rhi::QueryType queryType,
                                 core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIPipelineStatisticsQueryPool() override;

  void begin_query(core::uint32 queryIndex) const;

  void end_query() const;

  bool get_query_pool_results(core::uint8 *data, core::uint32 firstQueryIndex, core::uint32 numberOfQueries,
                              core::uint32 strideInBytes, bool waitForResult) const;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override
  {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIPipelineStatisticsQueryPool(const RHIPipelineStatisticsQueryPool &source) = delete;

  RHIPipelineStatisticsQueryPool &operator=(const RHIPipelineStatisticsQueryPool &source) = delete;

  bool get_query_pool_result(GLuint openGLQuery, bool waitForResult, core::uint64 &queryResult) const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint *mVerticesSubmittedOpenGLQueries;        ///< "GL_VERTICES_SUBMITTED_ARB", "rhi::PipelineStatisticsQueryResult::numberOfInputAssemblerVertices"
  GLuint *mPrimitivesSubmittedOpenGLQueries;        ///< "GL_PRIMITIVES_SUBMITTED_ARB", "rhi::PipelineStatisticsQueryResult::numberOfInputAssemblerPrimitives"
  GLuint *mVertexShaderInvocationsOpenGLQueries;      ///< "GL_VERTEX_SHADER_INVOCATIONS_ARB", "rhi::PipelineStatisticsQueryResult::numberOfVertexShaderInvocations"
  GLuint *mGeometryShaderInvocationsOpenGLQueries;    ///< "GL_GEOMETRY_SHADER_INVOCATIONS", "rhi::PipelineStatisticsQueryResult::numberOfGeometryShaderInvocations"
  GLuint *mGeometryShaderPrimitivesEmittedOpenGLQueries;  ///< "GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB", "rhi::PipelineStatisticsQueryResult::numberOfGeometryShaderOutputPrimitives"
  GLuint *mClippingInputPrimitivesOpenGLQueries;      ///< "GL_CLIPPING_INPUT_PRIMITIVES_ARB", "rhi::PipelineStatisticsQueryResult::numberOfClippingInputPrimitives"
  GLuint *mClippingOutputPrimitivesOpenGLQueries;      ///< "GL_CLIPPING_OUTPUT_PRIMITIVES_ARB", "rhi::PipelineStatisticsQueryResult::numberOfClippingOutputPrimitives"
  GLuint *mFragmentShaderInvocationsOpenGLQueries;    ///< "GL_FRAGMENT_SHADER_INVOCATIONS_ARB", "rhi::PipelineStatisticsQueryResult::numberOfFragmentShaderInvocations"
  GLuint *mTessControlShaderPatchesOpenGLQueries;      ///< "GL_TESS_CONTROL_SHADER_PATCHES_ARB", "rhi::PipelineStatisticsQueryResult::numberOfTessellationControlShaderInvocations"
  GLuint *mTesEvaluationShaderInvocationsOpenGLQueries;  ///< "GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB", "rhi::PipelineStatisticsQueryResult::numberOfTessellationEvaluationShaderInvocations"
  GLuint *mComputeShaderInvocationsOpenGLQueries;      ///< "GL_COMPUTE_SHADER_INVOCATIONS_ARB", "rhi::PipelineStatisticsQueryResult::numberOfComputeShaderInvocations"


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
