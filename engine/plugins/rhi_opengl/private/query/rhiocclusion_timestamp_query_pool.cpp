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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi_opengl/query/rhiocclusion_timestamp_query_pool.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIOcclusionTimestampQueryPool::RHIOcclusionTimestampQueryPool(RHIDevice &openGLRhi, rhi::QueryType queryType, core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIQueryPool(openGLRhi, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLQueries(re_typed_alloc<GLuint>(numberOfQueries)) {
  // If possible, use "glCreateQueries()" (OpenGL 4.5) in order to create the query instance at once
  if (nullptr != glCreateQueries) {
    switch (queryType) {
      case rhi::QueryType::OCCLUSION:
        glCreateQueries(GL_SAMPLES_PASSED_ARB, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
        break;

      case rhi::QueryType::PIPELINE_STATISTICS:
        BE_ASSERT(false, "Use \"RHIDevice::RHIPipelineStatisticsQueryPool\"")
        break;

      case rhi::QueryType::TIMESTAMP:
        glCreateQueries(GL_TIMESTAMP, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
        break;
    }
  } else {
    glGenQueriesARB(static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    switch (queryType) {
      case rhi::QueryType::OCCLUSION:
      {
        RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
        for (core::uint32 i = 0; i < mNumberOfQueries; ++i) {
          glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
        }
        break;
      }

      case rhi::QueryType::PIPELINE_STATISTICS:
        BE_ASSERT(false, "Use \"RHIDevice::RHIPipelineStatisticsQueryPool\"")
        break;

      case rhi::QueryType::TIMESTAMP:
      {
        RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
        for (core::uint32 i = 0; i < mNumberOfQueries; ++i) {
          glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
        }
        break;
      }
    }
  }
#endif
}

RHIOcclusionTimestampQueryPool::~RHIOcclusionTimestampQueryPool() {
  // Destroy the OpenGL queries
  // -> Silently ignores 0's and names that do not correspond to existing queries
  glDeleteQueriesARB(static_cast<GLsizei>(mNumberOfQueries), mOpenGLQueries);
  re_free(mOpenGLQueries);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
