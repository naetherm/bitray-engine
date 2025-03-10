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
#include "rhi_opengl/buffer/rhivertex_array.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;

class RHIVertexBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract OpenGL vertex array interface, effective vertex array object (VAO)
 */
class RHIVertexArrayVao : public RHIVertexArray {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  virtual ~RHIVertexArrayVao() override;

  /**
   * @brief
   * Return the OpenGL vertex array
   * 
   * @return
   * The OpenGL vertex array, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_vertex_array() const {
    return mOpenGLVertexArray;
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
   * @param[in] numberOfVertexBuffers
   * Number of vertex buffers, having zero vertex buffers is valid
   * @param[in] vertexBuffers
   * At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] indexBuffer
   * Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
   * @param[in] id
   * The unique compact vertex array ID
   */
  RHIVertexArrayVao(RHIDevice &openGLRhi,
                    rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint mOpenGLVertexArray;    ///< OpenGL vertex array ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  core::uint32 mNumberOfVertexBuffers;  ///< Number of vertex buffers
  RHIVertexBuffer **mVertexBuffers;      ///< Vertex buffers (we keep a reference to it) used by this vertex array, can be a null pointer


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIVertexArrayVao(const RHIVertexArrayVao &source) = delete;

  RHIVertexArrayVao &operator=(const RHIVertexArrayVao &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
