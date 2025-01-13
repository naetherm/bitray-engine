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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi_opengl/shader/separate/rhigraphics_program_separate_dsa.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/shader_compiler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIGraphicsProgramSeparateDsa::RHIGraphicsProgramSeparateDsa(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHIVertexShaderSeparate* vertexShaderSeparate, RHITessellationControlShaderSeparate* tessellationControlShaderSeparate, RHITessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, RHIGeometryShaderSeparate* geometryShaderSeparate, RHIFragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIGraphicsProgramSeparate(openGLRhi, rootSignature, vertexShaderSeparate, tessellationControlShaderSeparate, tessellationEvaluationShaderSeparate, geometryShaderSeparate, fragmentShaderSeparate RHI_RESOURCE_DEBUG_PASS_PARAMETER) {

}

RHIGraphicsProgramSeparateDsa::RHIGraphicsProgramSeparateDsa(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHITaskShaderSeparate* taskShaderSeparate, RHIMeshShaderSeparate& meshShaderSeparate, RHIFragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIGraphicsProgramSeparate(openGLRhi, rootSignature, taskShaderSeparate, meshShaderSeparate, fragmentShaderSeparate RHI_RESOURCE_DEBUG_PASS_PARAMETER) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
