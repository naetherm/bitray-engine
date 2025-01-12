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
#include "rhi/buffer/rhibuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract indirect buffer object interface; array of draw call data
 *
 * @note
 * - Contains instances of "rhi::DrawArguments" and "rhi::DrawIndexedArguments"
 * - Indirect buffers where originally introduced to be able to let the GPU have some more control over draw commands,
 * but with the introduction of multi indirect draw it became also interesting for reducing RHI implementation overhead (AZDO ("Almost Zero Driver Overhead"))
 */
class RHIIndirectBuffer : public RHIBuffer {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIIndirectBuffer() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfIndirectBuffers;
#endif
  }

  // Public virtual rhi::RHIIndirectBuffer methods
public:
  /**
   * @brief
   * Return indirect buffer emulation data pointer
   *
   * @return
   * Indirect buffer emulation data pointer, can be a null pointer, don't destroy the returned instance
   */
  [[nodiscard]] virtual const core::uint8 *get_emulation_data() const = 0;

  // Protected methods
protected:
#ifdef DEBUG

  /**
      *  @brief
      *    Default constructor
      *
      *  @note
      *    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
      */
  inline explicit RHIIndirectBuffer(const char debugName[])
  : RHIBuffer(ResourceType::INDIRECT_BUFFER, debugName) {
    
  }

#else

  /**
   * @brief
   * Default constructor
   *
   * @note
   * - Only used for rare border cases, use the constructor with the RHI reference whenever possible
   */
  inline RHIIndirectBuffer()
  : RHIBuffer(ResourceType::INDIRECT_BUFFER) {
    
  }

#endif

  /**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * Owner RHI instance
   */
  inline explicit RHIIndirectBuffer(RHIDevice &rhi, rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHIBuffer(ResourceType::INDIRECT_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER), mDescriptor(descriptor) {
#ifdef DEBUG
    // Update the statistics
    ++rhi.get_statistics().numberOfCreatedIndirectBuffers;
    ++rhi.get_statistics().currentNumberOfIndirectBuffers;
#endif
  }

  explicit RHIIndirectBuffer(const RHIIndirectBuffer &source) = delete;

  RHIIndirectBuffer &operator=(const RHIIndirectBuffer &source) = delete;

protected:

  rhi::IndirectBufferDescriptor mDescriptor;
};

typedef core::Ptr<RHIIndirectBuffer> RHIIndirectBufferPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
