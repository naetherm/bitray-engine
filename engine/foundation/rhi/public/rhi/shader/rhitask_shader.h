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
#include "rhi/shader/rhishader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract task shader (TS) interface
 */
class RHITaskShader : public RHIShader {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHITaskShader() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfTaskShaders;
#endif
  }

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * Owner RHI instance
   */
  inline explicit RHITaskShader(RHIDevice &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHIShader(ResourceType::TASK_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef DEBUG
    // Update the statistics
    ++rhi.get_statistics().numberOfCreatedTaskShaders;
    ++rhi.get_statistics().currentNumberOfTaskShaders;
#endif
  }

  explicit RHITaskShader(const RHITaskShader &source) = delete;

  RHITaskShader &operator=(const RHITaskShader &source) = delete;

};

typedef core::Ptr<RHITaskShader> RHITaskShaderPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
