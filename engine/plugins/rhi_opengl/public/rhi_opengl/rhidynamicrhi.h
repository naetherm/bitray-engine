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
#include <rhi/rhidynamicrhi.h>

extern "C"  RHI_OPENGL_API rhi::RHIDevice* create_rhi_device_instance();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class OpenGLRuntimeLinking;
class RHIOpenGLContext;
class Extensions;
class RHIRootSignature;
class RHIGraphicsPipelineState;
class RHIComputePipelineState;
class RHIVertexArray;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL RHI class
 */
class RHIDevice final : public rhi::RHIDevice
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class RHIGraphicsPipelineState;


  //[-------------------------------------------------------]
  //[ Public data                                           ]
  //[-------------------------------------------------------]
public:
  core::MakeIdUInt16 VertexArrayMakeId;
  core::MakeIdUInt16 GraphicsPipelineStateMakeId;
  core::MakeIdUInt16 ComputePipelineStateMakeId;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] context
   * RHI context, the RHI context instance must stay valid as long as the RHI instance exists
   * 
   * @note
   * - Do never ever use a not properly initialized RHI. Use "rhi::RHIrhi::is_initialized()" to check the initialization state.
   */
  RHIDevice();

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIDevice() override;

  /**
   * @brief
   * Return the OpenGL context instance
   * 
   * @return
   * The OpenGL context instance, do not free the memory the reference is pointing to
   */
  [[nodiscard]] inline const RHIOpenGLContext& get_opengl_context() const
  {
    return *mOpenGLContext;
  }

  /**
   * @brief
   * Return the available extensions
   * 
   * @return
   * The available extensions, do not free the memory the reference is pointing to
   */
  [[nodiscard]] inline const Extensions& get_extensions() const
  {
    return *mExtensions;
  }

  /**
   * @brief
   * Return the available extensions
   * 
   * @return
   * The available extensions, do not free the memory the reference is pointing to
   */
  [[nodiscard]] inline Extensions& get_extensions()
  {
    return *mExtensions;
  }

  void dispatch_command_buffer_internal(const rhi::RHICommandBuffer& commandBuffer);

  //[-------------------------------------------------------]
  //[ Graphics                                              ]
  //[-------------------------------------------------------]
  void set_graphics_root_signature(rhi::RHIRootSignature* rootSignature);
  void set_graphics_pipeline_state(rhi::RHIGraphicsPipelineState* graphicsPipelineState);
  void set_graphics_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup);
  void set_graphics_vertex_array(rhi::RHIVertexArray* vertexArray);															// Input-assembler (IA) stage
  void set_graphics_viewports(core::uint32 numberOfViewports, const rhi::Viewport* viewports);									// Rasterizer (RS) stage
  void set_graphics_scissor_rectangles(core::uint32 numberOfScissorRectangles, const rhi::ScissorRectangle* scissorRectangles);	// Rasterizer (RS) stage
  void set_graphics_render_target(rhi::RHIRenderTarget* renderTarget);															// Output-merger (OM) stage
  void clear_graphics(core::uint32 clearFlags, const float color[4], float z, core::uint32 stencil);
  void draw_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_indexed_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_indexed_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_mesh_tasks(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_mesh_tasks_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  //[-------------------------------------------------------]
  //[ Compute                                               ]
  //[-------------------------------------------------------]
  void set_compute_root_signature(rhi::RHIRootSignature* rootSignature);
  void set_compute_pipeline_state(rhi::RHIComputePipelineState* computePipelineState);
  void set_compute_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup);
  void dispatch_compute(core::uint32 groupCountX, core::uint32 groupCountY, core::uint32 groupCountZ);
  //[-------------------------------------------------------]
  //[ Resource                                              ]
  //[-------------------------------------------------------]
  void resolve_multisample_framebuffer(rhi::RHIRenderTarget& destinationRenderTarget, rhi::RHIFramebuffer& sourceMultisampleFramebuffer);
  void copy_resource(rhi::RHIResource& destinationResource, rhi::RHIResource& sourceResource);
  void generate_mipmaps(rhi::RHIResource& resource);
  //[-------------------------------------------------------]
  //[ Query                                                 ]
  //[-------------------------------------------------------]
  void reset_query_pool(rhi::RHIQueryPool& queryPool, core::uint32 firstQueryIndex, core::uint32 numberOfQueries);
  void begin_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex, core::uint32 queryControlFlags);
  void end_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex);
  void write_timestamp_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex);
  //[-------------------------------------------------------]
  //[ Debug                                                 ]
  //[-------------------------------------------------------]
#ifdef DEBUG
  void set_debug_marker(const char* name);
			void begin_debug_event(const char* name);
			void end_debug_event();
#endif


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIDevice methods                      ]
  //[-------------------------------------------------------]
public:

  void initialize(rhi::RHIContext& rhiContext) override;

  [[nodiscard]] virtual const char* get_name() const override;
  [[nodiscard]] virtual bool is_initialized() const override;
  [[nodiscard]] virtual bool is_debug_enabled() override;
  //[-------------------------------------------------------]
  //[ Shader language                                       ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual core::uint32 get_number_of_shader_languages() const override;
  [[nodiscard]] virtual const char* get_shader_language_name(core::uint32 index) const override;
  [[nodiscard]] virtual rhi::RHIShaderLanguage* get_shader_language(const char* shaderLanguageName = nullptr) override;
  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual rhi::RHIRenderPass* create_render_pass(core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat = rhi::TextureFormat::UNKNOWN, core::uint8 numberOfMultisamples = 1
                                                                 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIQueryPool* create_query_pool(rhi::QueryType queryType, core::uint32 numberOfQueries = 1
                                                               RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHISwapChain* create_swap_chain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData frontendPlatformData, bool useExternalContext = false
                                                               RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIFramebuffer* create_framebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment = nullptr
                                                                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIBufferManager* create_buffer_manager() override;
  [[nodiscard]] virtual rhi::RHITextureManager* create_texture_manager() override;
  [[nodiscard]] virtual rhi::RHIRootSignature* create_root_signature(const rhi::RootSignatureDescriptor& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIGraphicsPipelineState* create_graphics_pipeline_state(const rhi::GraphicsPipelineStateDescriptor& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIComputePipelineState* create_compute_pipeline_state(rhi::RHIRootSignature& rootSignature, rhi::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHISamplerState* create_sampler_state(const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  //[-------------------------------------------------------]
  //[ Resource handling                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual bool map(rhi::RHIResource& resource, core::uint32 subresource, rhi::MapType mapType, core::uint32 mapFlags, rhi::MappedSubresource& mappedSubresource) override;
  virtual void unmap(rhi::RHIResource& resource, core::uint32 subresource) override;
  [[nodiscard]] virtual bool get_query_pool_results(rhi::RHIQueryPool& queryPool, core::uint32 numberOfDataBytes, core::uint8* data, core::uint32 firstQueryIndex = 0, core::uint32 numberOfQueries = 1, core::uint32 strideInBytes = 0, core::uint32 queryResultFlags = 0) override;
  //[-------------------------------------------------------]
  //[ Operation                                             ]
  //[-------------------------------------------------------]
  virtual void dispatch_command_buffer(const rhi::RHICommandBuffer& commandBuffer) override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override
  {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private static methods                                ]
  //[-------------------------------------------------------]
private:
  /**
   * @brief
   * Debug message callback function called by the "GL_ARB_debug_output"-extension
   * 
   * @param[in] source
   * Source of the debug message
   * @param[in] type
   * Type of the debug message
   * @param[in] id
   * ID of the debug message
   * @param[in] severity
   * Severity of the debug message
   * @param[in] length
   * Length of the debug message
   * @param[in] message
   * The debug message
   * @param[in] userParam
   * Additional user parameter of the debug message
   */
  static void CALLBACK debug_message_callback(core::uint32 source, core::uint32 type, core::uint32 id, core::uint32 severity, int length, const char* message, const void* userParam);


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIDevice(const RHIDevice& source) = delete;
  RHIDevice& operator =(const RHIDevice& source) = delete;

  /**
   * @brief
   * Initialize the capabilities
   */
  void initialize_capabilities();

  /**
   * @brief
   * Unset the currently used vertex array
   */
  void unset_graphics_vertex_array();

  /**
   * @brief
   * Set resource group, method used by graphics and compute
   * 
   * @param[in] rootSignature
   * Used root signature
   * @param[in] rootParameterIndex
   * Root parameter index
   * @param[in] resourceGroup
   * Resource group to set, can be a null pointer
   */
  void set_resource_group(const RHIRootSignature& rootSignature, core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup);

  /**
   * @brief
   * Set OpenGL graphics program
   * 
   * @param[in] graphicsProgram
   * Graphics program to set
   */
  void set_opengl_graphics_program(rhi::RHIGraphicsProgram* graphicsProgram);

  /**
   * @brief
   * Set OpenGL compute pipeline state
   * 
   * @param[in] computePipelineState
   * Compute pipeline state to set
   */
  void set_opengl_compute_pipeline_state(RHIComputePipelineState* computePipelineState);

  /**
   * @brief
   * Update "GL_ARB_base_instance" emulation
   * 
   * @param[in] startInstanceLocation
   * start instance location
   */
  void updateGL_ARB_base_instanceEmulation(core::uint32 startInstanceLocation);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  OpenGLRuntimeLinking* mOpenGLRuntimeLinking;			///< OpenGL runtime linking instance, always valid
  RHIOpenGLContext*		  mOpenGLContext;					///< OpenGL context instance, always valid
  Extensions*			  mExtensions;						///< Extensions instance, always valid
  rhi::RHIShaderLanguage* mShaderLanguage;					///< Shader language instance (we keep a reference to it), can be a null pointer
  RHIRootSignature*		  mGraphicsRootSignature;			///< Currently set graphics root signature (we keep a reference to it), can be a null pointer
  RHIRootSignature*		  mComputeRootSignature;			///< Currently set compute root signature (we keep a reference to it), can be a null pointer
  rhi::RHISamplerState*   mDefaultSamplerState;				///< Default rasterizer state (we keep a reference to it), can be a null pointer
  GLuint				  mOpenGLCopyResourceFramebuffer;	///< OpenGL framebuffer ("container" object, not shared between OpenGL contexts) used by "OpenGLRhi::OpenGLRhi::copy_resource()" if the "GL_ARB_copy_image"-extension isn't available, can be zero if no resource is allocated
  GLuint				  mDefaultOpenGLVertexArray;		///< Default OpenGL vertex array ("container" object, not shared between OpenGL contexts) to enable attribute-less rendering, can be zero if no resource is allocated
  // States
  RHIGraphicsPipelineState* mGraphicsPipelineState;	///< Currently set graphics pipeline state (we keep a reference to it), can be a null pointer
  RHIComputePipelineState*  mComputePipelineState;	///< Currently set compute pipeline state (we keep a reference to it), can be a null pointer
  // Input-assembler (IA) stage
  RHIVertexArray* mVertexArray;				///< Currently set vertex array (we keep a reference to it), can be a null pointer
  GLenum		 mOpenGLPrimitiveTopology;	///< OpenGL primitive topology describing the type of primitive to render
  GLint		 mNumberOfVerticesPerPatch;	///< Number of vertices per patch
  // Output-merger (OM) stage
  rhi::RHIRenderTarget* mRenderTarget;	///< Currently set render target (we keep a reference to it), can be a null pointer
  // State cache to avoid making redundant OpenGL calls
  GLenum mOpenGLClipControlOrigin;	///< Currently set OpenGL clip control origin
  GLuint mOpenGLProgramPipeline;		///< Currently set OpenGL program pipeline, can be zero if no resource is set
  GLuint mOpenGLProgram;				///< Currently set OpenGL program, can be zero if no resource is set
  GLuint mOpenGLIndirectBuffer;		///< Currently set OpenGL indirect buffer, can be zero if no resource is set
  // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
  GLuint	 mOpenGLVertexProgram;			///< Currently set OpenGL vertex program, can be zero if no resource is set
  GLint	 mDrawIdUniformLocation;		///< Draw ID uniform location
  core::uint32 mCurrentStartInstanceLocation;	///< Currently set start instance location


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
