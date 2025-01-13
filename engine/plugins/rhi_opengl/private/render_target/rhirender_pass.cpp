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
#include "rhi_opengl/render_target/rhirender_pass.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIRenderPass::RHIRenderPass(rhi::RHIDevice &rhi, core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum *colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIRenderPass(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mNumberOfColorAttachments(numberOfColorAttachments)
, mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat)
, mNumberOfMultisamples(numberOfMultisamples) {
  BE_ASSERT(mNumberOfColorAttachments < 8, "Invalid number of OpenGL color attachments")
  memcpy(mColorAttachmentTextureFormats, colorAttachmentTextureFormats, sizeof(rhi::TextureFormat::Enum) * mNumberOfColorAttachments);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
