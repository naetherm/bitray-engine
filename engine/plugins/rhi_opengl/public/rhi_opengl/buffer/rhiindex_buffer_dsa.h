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
#include "rhi_opengl/buffer/rhiindex_buffer.h"


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
 * OpenGL index buffer object (IBO, "element array buffer" in OpenGL terminology) class, effective direct state access (DSA)
 */
class RHIIndexBufferDsa final : public RHIIndexBuffer {


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
   * @param[in] numberOfBytes
   * Number of bytes within the index buffer, must be valid
   * @param[in] data
   * Index buffer data, can be a null pointer (empty buffer)
   * @param[in] bufferUsage
   * Indication of the buffer usage
   * @param[in] indexBufferFormat
   * Index buffer data format
   */
  RHIIndexBufferDsa(RHIDevice &openGLRhi,
                    rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIIndexBufferDsa() override = default;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIIndexBufferDsa(const RHIIndexBufferDsa &source) = delete;

  RHIIndexBufferDsa &operator=(const RHIIndexBufferDsa &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
