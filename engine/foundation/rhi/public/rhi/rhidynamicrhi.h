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
#include "rhi/rhicapabilities.h"
#include "rhi/rhistatistics.h"
#include "rhi/query/rhiquery_types.h"
#include "rhi/state/rhipipeline_state_types.h"
#include "rhi/state/rhisampler_state_types.h"
#include "rhi/descriptor/root_signature_descriptor.h"
#include <core/core/refcounted.h>
#include <core/core/ptr.h>
#include <core/frontend/types.h>



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RHIContext;
class RHIResource;
class RHIShaderLanguage;
class RHIRenderPass;
class RHIQueryPool;
class RHISwapChain;
class RHIFramebuffer;
class FramebufferAttachment;
class RHIBufferManager;
class RHITextureManager;
class RHIRootSignature;
class RHIGraphicsPipelineState;
class RHIComputePipelineState;
class RHIComputeShader;
class RHISamplerState;
class RHICommandBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * RHI implementation name as ID
 */
enum class NameId : core::uint32 {
  /// Vulkan RHI implementation, same value as renderer STRING_ID("Vulkan")
  VULKAN = 1646768219,
  /// Direct3D 12 RHI implementation, same value as renderer STRING_ID("Direct3D12")
  DIRECT3D12 = 2152506057,
  /// Direct3D 11 RHI implementation, same value as renderer STRING_ID("Direct3D11")
  DIRECT3D11 = 2102173200,
  /// Direct3D 10 RHI implementation, same value as renderer STRING_ID("Direct3D10")
  DIRECT3D10 = 2118950819,
  /// Direct3D 9 RHI implementation, same value as renderer STRING_ID("Direct3D9")
  DIRECT3D9 = 3508528873,
  /// OpenGL RHI implementation, same value as renderer STRING_ID("OpenGL")
  OPENGL = 1149085807,
  /// OpenGL ES 3 RHI implementation, same value as renderer STRING_ID("OpenGLES3")
  OPENGLES3 = 4137012044,
  /// Null RHI implementation, same value as renderer STRING_ID("Null")
  NULL_DUMMY = 3816175889
};


/**
 * @brief
 * Abstract rendering hardware interface (RHI)
 */
class RHIDevice : public core::RefCounted {

  // Friends for none constant statistics access
  friend class RHIRootSignature;
  friend class RHIResourceGroup;
  friend class RHIGraphicsProgram;
  friend class RHIVertexArray;
  friend class RHIRenderPass;
  friend class RHIQueryPool;
  friend class RHISwapChain;
  friend class RHIFramebuffer;
  friend class RHIVertexBuffer;
  friend class RHIIndexBuffer;
  friend class RHITextureBuffer;
  friend class RHIStructuredBuffer;
  friend class RHIIndirectBuffer;
  friend class RHIUniformBuffer;
  friend class RHITexture1D;
  friend class RHITexture1DArray;
  friend class RHITexture2D;
  friend class RHITexture2DArray;
  friend class RHITexture3D;
  friend class RHITextureCube;
  friend class RHITextureCubeArray;
  friend class RHIGraphicsPipelineState;
  friend class RHIComputePipelineState;
  friend class RHISamplerState;
  friend class RHIVertexShader;
  friend class RHITessellationControlShader;
  friend class RHITessellationEvaluationShader;
  friend class RHIGeometryShader;
  friend class RHIFragmentShader;
  friend class RHITaskShader;
  friend class RHIMeshShader;
  friend class RHIComputeShader;

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIDevice() override = default;

  /**
   * @brief
   * Return the RHI implementation name as ID
   *
   * @return
   * The RHI implementation name as ID
   */
  [[nodiscard]] inline NameId get_name_id() const {
    return mNameId;
  }

  /**
   * @brief
   * Return the context of the RHI instance
   *
   * @return
   * The context of the RHI instance, do not free the memory the returned reference is pointing to
   */
  [[nodiscard]] inline const RHIContext &get_context() const {
    return *mContext;
  }

  /**
   * @brief
   * Return the capabilities of the RHI instance
   *
   * @return
   * The capabilities of the RHI instance, do not free the memory the returned reference is pointing to
   */
  [[nodiscard]] inline const Capabilities &get_capabilities() const {
    return mCapabilities;
  }

  /**
   * @brief
   * Return default shader language instance
   *
   * @return
   * The default shader language instance, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] inline RHIShaderLanguage &get_default_shader_language() {
    RHIShaderLanguage *shaderLanguage = get_shader_language();
    BE_ASSERT(nullptr != shaderLanguage, "There's no default shader language")
    return *shaderLanguage;
  }

#ifdef DEBUG
  /**
   * @brief
   * Return the statistics of the RHI instance
   *
   * @return
   * The statistics of the RHI instance, do not free the memory the returned reference is pointing to
   *
   * @note
   * - It's possible that the statistics or part of it are disabled, e.g. due to hight performance constrains
   */
  [[nodiscard]] inline const RHIStatistics& get_statistics() const {
    return mStatistics;
  }

  /**
   * @brief
   * Return the statistics of the RHI instance
   *
   * @return
   * The statistics of the RHI instance
   *
   * @note
   * - Do not free the memory the returned reference is pointing to
   * - It's possible that the statistics or part of it are disabled, e.g. due to hight performance constrains
   */
  [[nodiscard]] inline RHIStatistics& get_statistics() {
    return mStatistics;
  }
#endif

  // Public virtual rhi::RHIDevice methods
public:

  virtual void initialize(RHIContext &rhiContext) = 0;

  /**
   * @brief
   * Return the name of the RHI instance
   *
   * @return
   * The ASCII name of the RHI instance, null pointer on error, do not free the memory the returned pointer is pointing to
   */
  [[nodiscard]] virtual const char *get_name() const = 0;

  /**
   * @brief
   * Return whether or not the RHI instance is properly initialized
   *
   * @return
   * "true" if the RHI instance is properly initialized, else "false"
   *
   * @note
   * - Do never ever use a not properly initialized RHI
   */
  [[nodiscard]] virtual bool is_initialized() const = 0;

  /**
   * @brief
   * Return whether or not debug is enabled
   *
   * @remarks
   * By using
   * "rhi::RHIDevice::is_debug_enabled();"
   * it is possible to check whether or not your application is currently running
   * within a known debug/profile tool like e.g. Direct3D PIX (also works directly within VisualStudio
   * 2017 out-of-the-box). In case you want at least try to protect your asset, you might want to stop
   * the execution of your application when a debug/profile tool is used which can e.g. record your data.
   * Please be aware that this will only make it a little bit harder to debug and e.g. while doing so
   * reading out your asset data. Public articles like
   * "PIX: How to circumvent D3DPERF_SetOptions" at
   * http://www.gamedev.net/blog/1323/entry-2250952-pix-how-to-circumvent-d3dperf-setoptions/
   * describe how to "hack around" this security measurement, so, don't rely on it. Those debug
   * methods work fine when using a Direct3D RHI implementation. OpenGL on the other hand
   * has no Direct3D PIX like functions or extensions, use for instance "gDEBugger" (http://www.gremedy.com/)
   * instead.
   * -> When using Direct3D <11.1, those methods map to the Direct3D 9 PIX functions (D3DPERF_* functions)
   * -> The Direct3D 9 PIX functions are also used for Direct3D 10 and Direct3D 11. Lookout! As soon as using
   * the debug methods within this interface, the Direct3D 9 dll will be loaded.
   * -> Starting with Direct3D 11.1, the Direct3D 9 PIX functions no longer work. Instead, the new
   * "D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY"-flag (does not work with <Direct3D 11.1)
   * is used when creating the device instance, then the "ID3DUserDefinedAnnotation"-API is used.
   * -> Optimization: You might want to use those methods only via macros to make it easier to avoid using them
   * within e.g. a final release build
   *
   * @return
   * "true" if debug is enabled, else "false"
   */
  [[nodiscard]] virtual bool is_debug_enabled() = 0;

  //[-------------------------------------------------------]
  //[ Shader language                                       ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Return the number of supported shader languages
   *
   * @return
   * The number of supported shader languages
   */
  [[nodiscard]] virtual core::uint32 get_number_of_shader_languages() const = 0;

  /**
   * @brief
   * Return the name of a supported shader language at the provided index
   *
   * @param[in] index
   * Index of the supported shader language to return the name from ([0, get_number_of_shader_languages()-1])
   *
   * @return
   * The ASCII name (for example "GLSL" or "HLSL") of the supported shader language at the provided index, can be a null pointer
   *
   * @note
   * - Do not free the memory the returned pointer is pointing to
   * - The default shader language is always at index 0
   */
  [[nodiscard]] virtual const char *get_shader_language_name(core::uint32 index) const = 0;

  /**
   * @brief
   * Return a shader language instance
   *
   * @param[in] shaderLanguageName
   * The ASCII name of the shader language (for example "GLSL" or "HLSL"), if null pointer or empty string,
   * the default RHI shader language is used (see "get_shader_language_name(0)")
   *
   * @return
   * The shader language instance, a null pointer on error, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] virtual RHIShaderLanguage *get_shader_language(const char *shaderLanguageName = nullptr) = 0;

  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Create a render pass instance
   *
   * @param[in] numberOfColorAttachments
   * Number of color render target textures, must be <="rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
   * @param[in] colorAttachmentTextureFormats
   * The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   * least "numberOfColorAttachments" textures in the provided C-array of pointers
   * @param[in] depthStencilAttachmentTextureFormat
   * The optional depth stencil render target texture format, can be a "rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
   * @param[in] numberOfMultisamples
   * The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   *
   * @return
   * The created render pass instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIRenderPass* create_render_pass(core::uint32 numberOfColorAttachments, const TextureFormat::Enum *colorAttachmentTextureFormats, TextureFormat::Enum depthStencilAttachmentTextureFormat = TextureFormat::UNKNOWN, core::uint8 numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a asynchronous query pool instance
   *
   * @param[in] queryType
   * Query type (e.g. "rhi::QueryType::OCCLUSION")
   * @param[in] numberOfQueries
   * The number of queries the query pool contains (e.g. 1)
   *
   * @return
   * The created query pool instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIQueryPool* create_query_pool(QueryType queryType, core::uint32 numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a swap chain instance
   *
   * @param[in] renderPass
   * Render pass to use, the swap chain keeps a reference to the render pass
   * @param[in] windowHandle
   * Information about the window to render into
   * @param[in] useExternalContext
   * Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
   *
   * @return
   * The created swap chain instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHISwapChain *create_swap_chain(RHIRenderPass &renderPass, core::FrontendPlatformData frontendPlatformData, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a framebuffer object (FBO) instance
   *
   * @param[in] renderPass
   * Render pass to use, the framebuffer keeps a reference to the render pass
   * @param[in] colorFramebufferAttachments
   * The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   * least "rhi::RHIRenderPass::get_number_of_color_attachments()" textures in the provided C-array of pointers
   * @param[in] depthStencilFramebufferAttachment
   * The optional depth stencil render target texture, can be a null pointer
   *
   * @return
   * The created FBO instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - Only supported if "rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets" is not 0
   * - The framebuffer keeps a reference to the provided texture instances
   * - It's invalid to set the same color texture to multiple render targets at one and the same time
   * - Depending on the used RHI implementation and feature set, there might be the requirement that all provided textures have the same size
   * (in order to be on the save side, ensure that all provided textures have the same size and same MSAA sample count)
   */
  [[nodiscard]] virtual RHIFramebuffer* create_framebuffer(RHIRenderPass &renderPass, const FramebufferAttachment *colorFramebufferAttachments, const FramebufferAttachment *depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a buffer manager instance
   *
   * @return
   * The created buffer manager instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIBufferManager *create_buffer_manager() = 0;

  /**
   * @brief
   * Create a texture manager instance
   *
   * @return
   * The created texture manager instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHITextureManager *create_texture_manager() = 0;

  /**
   * @brief
   * Create a root signature instance
   *
   * @param[in] rootSignature
   * Root signature to use
   *
   * @return
   * The root signature instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIRootSignature* create_root_signature(const RootSignatureDescriptor &rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a graphics pipeline state instance
   *
   * @param[in] graphicsPipelineState
   * Graphics pipeline state to use
   *
   * @return
   * The graphics pipeline state instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIGraphicsPipelineState *create_graphics_pipeline_state(const GraphicsPipelineStateDescriptor &graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a compute pipeline state instance
   *
   * @param[in] rootSignature
   * Root signature (compute pipeline state instances keep a reference to the root signature)
   * @param[in] computeShader
   * Compute shader used by the compute pipeline state (compute pipeline state instances keep a reference to the shader)
   *
   * @return
   * The compute pipeline state instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIComputePipelineState *create_compute_pipeline_state(RHIRootSignature &rootSignature, RHIComputeShader &computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a sampler state instance
   *
   * @param[in] samplerState
   * Sampler state to use
   *
   * @return
   * The sampler state instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHISamplerState* create_sampler_state(const SamplerStateDescriptor &samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  //[-------------------------------------------------------]
  //[ Resource handling                                     ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Map a resource
   *
   * @param[in]  resource
   * Resource to map, there's no internal resource validation, so, do only use valid resources in here!
   * @param[in]  subresource
   * Subresource
   * @param[in]  mapType
   * Map type
   * @param[in]  mapFlags
   * Map flags, see "rhi::MapFlag"-flags
   * @param[out] mappedSubresource
   * Receives the mapped subresource information, do only use this data in case this method returns successfully
   *
   * @return
   * "true" if all went fine, else "false"
   */
  [[nodiscard]] virtual bool map(RHIResource &resource, core::uint32 subresource, MapType mapType, core::uint32 mapFlags, MappedSubresource &mappedSubresource) = 0;

  /**
   * @brief
   * Unmap a resource
   *
   * @param[in] resource
   * Resource to unmap, there's no internal resource validation, so, do only use valid resources in here!
   * @param[in] subresource
   * Subresource
   */
  virtual void unmap(RHIResource &resource, core::uint32 subresource) = 0;

  /**
   * @brief
   * get asynchronous query pool results
   *
   * @param[in] queryPool
   * Query pool
   * @param[in] numberOfDataBytes
   * Number of data bytes
   * @param[out] data
   * Receives the query data
   * @param[in] firstQueryIndex
   * First query index (e.g. 0)
   * @param[in] numberOfQueries
   * Number of queries (e.g. 1)
   * @param[in] strideInBytes
   * Stride in bytes, 0 is only valid in case there's just a single query
   * @param[in] queryResultFlags
   * Query control flags (e.g. "rhi::QueryResultFlags::WAIT")
   *
   * @return
   * "true" if all went fine, else "false"
   */
  [[nodiscard]] virtual bool get_query_pool_results(RHIQueryPool &queryPool, core::uint32 numberOfDataBytes, core::uint8 *data, core::uint32 firstQueryIndex = 0, core::uint32 numberOfQueries = 1, core::uint32 strideInBytes = 0, core::uint32 queryResultFlags = 0) = 0;

  //[-------------------------------------------------------]
  //[ Operation                                             ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Dispatch command buffer to RHI
   *
   * @param[in] commandBuffer
   * Command buffer to dispatch
   */
  virtual void dispatch_command_buffer(const RHICommandBuffer &commandBuffer) = 0;

  //[-------------------------------------------------------]
  //[ RHI implementation specific                           ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual void *get_d3d11_device_pointer() const {
    return nullptr;
  }

  [[nodiscard]] virtual void *get_d3d11_immediate_context_pointer() const {
    return nullptr;
  }

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] nameId
   * RHI implementation name as ID
   * @param[in] context
   * RHI context, the RHI context instance must stay valid as long as the RHI instance exists
   */
  inline RHIDevice(NameId nameId)
  : mNameId(nameId) {

  }

  explicit RHIDevice(const RHIDevice &source) = delete;

  RHIDevice &operator=(const RHIDevice &source) = delete;

  // Protected data
protected:
  /** RHI implementation name as ID */
  NameId mNameId;
  /** Context */
  RHIContext *mContext;
  /** Capabilities */
  Capabilities mCapabilities;

#ifdef DEBUG
  // Private data
private:
  /** Statistics */
  RHIStatistics mStatistics;
#endif

};

typedef core::Ptr<RHIDevice> RHIDevicePtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
