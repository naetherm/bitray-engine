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
#include "rhi/rhiresource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract vertex array object (VAO) interface
 *
 * @remarks
 * Encapsulates all the data that is associated with the vertex processor.
 *
 * Direct3D specifies input stream slot indices within "vertex declaration"/"input layout" without binding them to a particular
 * vertex buffer object (VBO). Vertex buffers are bound to particular input stream slots. OpenGL "vertex array object" (VAO) has no
 * concept of input stream slot indices, vertex attributes are directly bound to a particular vertex buffer to feed the data into
 * the input assembly (IA). To be able to map this interface efficiently to Direct3D as well as OpenGL, this interface has to stick
 * to the OpenGL "vertex array object"-concept. As a result, we have to directly define vertex buffer objects as data source.
 */
class RHIVertexArray : public RHIResource {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIVertexArray() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfVertexArrays;
#endif
  }

  inline rhi::VertexArrayDescriptor &descriptor() {
    return mDescriptor;
  }

  /**
   * @brief
   * Return the unique compact vertex array ID
   *
   * @return
   * The unique compact vertex array ID
   */
  inline core::uint16 get_id() const {
    return mDescriptor.id;
  }

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * Owner RHI instance
   * @param[in] id
   * The unique compact vertex array ID
   */
  inline RHIVertexArray(RHIDevice &rhi, rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHIResource(ResourceType::VERTEX_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  , mDescriptor(descriptor) {
#ifdef DEBUG
    // Update the statistics
    ++rhi.get_statistics().numberOfCreatedVertexArrays;
    ++rhi.get_statistics().currentNumberOfVertexArrays;
#endif
  }

  explicit RHIVertexArray(const RHIVertexArray &source) = delete;

  RHIVertexArray &operator=(const RHIVertexArray &source) = delete;

  // Private data
protected:
  rhi::VertexArrayDescriptor mDescriptor;
};

typedef core::Ptr<RHIVertexArray> RHIVertexArrayPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
