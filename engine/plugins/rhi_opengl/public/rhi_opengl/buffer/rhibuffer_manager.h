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
class Extensions;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL buffer manager interface
 */
class RHIBufferManager final : public rhi::RHIBufferManager
{


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
   */
  explicit RHIBufferManager(RHIDevice& openGLRhi);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIBufferManager() override = default;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIBufferManager methods        ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual rhi::RHIVertexBuffer* create_vertex_buffer(rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIIndexBuffer* create_index_buffer(rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIVertexArray* create_vertex_array(rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITextureBuffer* create_texture_buffer(rhi::TextureBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIStructuredBuffer* create_structured_buffer(rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIIndirectBuffer* create_indirect_buffer(rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIUniformBuffer* create_uniform_buffer(rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIBufferManager(const RHIBufferManager& source) = delete;
  RHIBufferManager& operator =(const RHIBufferManager& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  const Extensions* mExtensions;	///< Extensions instance, always valid
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
