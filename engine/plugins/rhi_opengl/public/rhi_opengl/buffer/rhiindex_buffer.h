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
#include <rhi/buffer/rhiindex_buffer.h>


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
 * Abstract OpenGL index buffer object (IBO, "element array buffer" in OpenGL terminology) interface
 */
class RHIIndexBuffer : public rhi::RHIIndexBuffer {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  virtual ~RHIIndexBuffer() override;

  /**
   * @brief
   * Return the OpenGL element array buffer
   * 
   * @return
   * The OpenGL element array buffer, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_element_array_buffer() const {
    return mOpenGLElementArrayBuffer;
  }

  /**
   * @brief
   * Return the OpenGL element array buffer data type
   * 
   * @return
   * The OpenGL element array buffer data type
   */
  [[nodiscard]] inline GLenum get_opengl_type() const {
    return mOpenGLType;
  }

  /**
   * @brief
   * Return the number of bytes of an index
   * 
   * @return
   * The number of bytes of an index
   */
  [[nodiscard]] inline core::uint32 get_index_size_in_bytes() const {
    return mIndexSizeInBytes;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods             ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override
  {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] indexBufferFormat
   * Index buffer data format
   */
  RHIIndexBuffer(RHIDevice &openGLRhi,
                 rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint mOpenGLElementArrayBuffer;  ///< OpenGL element array buffer, can be zero if no resource is allocated
  GLenum mOpenGLType;        ///< OpenGL element array buffer data type
  core::uint32 mIndexSizeInBytes;      ///< Number of bytes of an index


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIIndexBuffer(const RHIIndexBuffer &source) = delete;

  RHIIndexBuffer &operator=(const RHIIndexBuffer &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
