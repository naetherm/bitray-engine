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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL vertex array class, traditional version
 */
class RHIVertexArrayNoVao final : public RHIVertexArray {


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
   * @param[in] vertexAttributes
   * Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
   * @param[in] numberOfVertexBuffers
   * Number of vertex buffers, having zero vertex buffers is valid
   * @param[in] vertexBuffers
   * At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] indexBuffer
   * Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
   * @param[in] id
   * The unique compact vertex array ID
   */
  RHIVertexArrayNoVao(RHIDevice &openGLRhi,
                      rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIVertexArrayNoVao() override;

  /**
   * @brief
   * Enable OpenGL vertex attribute arrays
   */
  void enable_opengl_vertex_attrib_arrays();

  /**
   * @brief
   * Disable OpenGL vertex attribute arrays
   */
  void disable_opengl_vertex_attrib_arrays();


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIVertexArrayNoVao(const RHIVertexArrayNoVao &source) = delete;

  RHIVertexArrayNoVao &operator=(const RHIVertexArrayNoVao &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  core::uint32 mNumberOfAttributes;      ///< Number of attributes (position, color, texture coordinate, normal...), having zero attributes is valid
  rhi::VertexAttribute *mAttributes;          ///< At least "numberOfAttributes" instances of vertex attributes, can be a null pointer in case there are zero attributes
  core::uint32 mNumberOfVertexBuffers;    ///< Number of vertex buffers, having zero vertex buffers is valid
  rhi::VertexArrayVertexBuffer *mVertexBuffers;        ///< At least mNumberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
  bool mIsGL_ARB_instanced_arrays;  ///< Is the "GL_ARB_instanced_arrays"-extension supported?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
