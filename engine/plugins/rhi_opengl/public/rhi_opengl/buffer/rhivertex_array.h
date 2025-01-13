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
#include <rhi/buffer/rhivertex_array.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;

class RHIIndexBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract OpenGL vertex array interface
 */
class RHIVertexArray : public rhi::RHIVertexArray {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Internal resource type
   */
  class InternalResourceType {
  public:
    enum Enum {
      NO_VAO = 0,  ///< No vertex array object
      VAO = 1  ///< Vertex array object
    };
  };


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  virtual ~RHIVertexArray() override;

  /**
   * @brief
   * Return the used index buffer
   * 
   * @return
   * The used index buffer, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] inline RHIIndexBuffer *get_index_buffer() const {
    return mIndexBuffer;
  }

  /**
   * @brief
   * Return the internal resource type
   * 
   * @return
   * The internal resource type
   */
  [[nodiscard]] inline InternalResourceType::Enum get_internal_resource_type() const {
    return mInternalResourceType;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
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
   * @param[in] indexBuffer
   * Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
   * @param[in] internalResourceType
   * Internal resource type
   * @param[in] id
   * The unique compact vertex array ID
   */
  RHIVertexArray(RHIDevice &openGLRhi, rhi::VertexArrayDescriptor descriptor,
                 InternalResourceType::Enum internalResourceType RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIVertexArray(const RHIVertexArray &source) = delete;

  RHIVertexArray &operator=(const RHIVertexArray &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIIndexBuffer *mIndexBuffer;      ///< Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
  InternalResourceType::Enum mInternalResourceType;  ///< Internal resource type


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
