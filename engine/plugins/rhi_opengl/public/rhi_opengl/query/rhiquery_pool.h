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
 * Abstract OpenGL asynchronous query pool base class
 */
class RHIQueryPool : public rhi::RHIQueryPool {


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
  RHIQueryPool(RHIDevice &openGLRhi, rhi::QueryType queryType,
               core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIQueryPool() override = default;

  /**
   * @brief
   * Return the query type
   * 
   * @return
   * The query type
   */
  [[nodiscard]] inline rhi::QueryType get_query_type() const {
    return mQueryType;
  }

  /**
   * @brief
   * Return the number of queries
   * 
   * @return
   * The number of queries
   */
  [[nodiscard]] inline core::uint32 get_number_of_queries() const {
    return mNumberOfQueries;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIQueryPool(const RHIQueryPool &source) = delete;

  RHIQueryPool &operator=(const RHIQueryPool &source) = delete;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  rhi::QueryType mQueryType;
  core::uint32 mNumberOfQueries;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
