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
#include "rhi/state/rhiblend_state_types.h"
#include "rhi/state/rhidepth_stencil_state_types.h"
#include "rhi/state/rhipipeline_state_types.h"
#include "rhi/state/rhirasterizer_state_types.h"
#include "rhi/state/rhisampler_state_types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Sampler state
 *
 * @remarks
 * == About mipmapping ==
 * The texture filter mode does not support explicitly disabling mipmapping. In case our texture does not have
 * any mipmaps, set "rhi::SamplerState::maxLod" to zero in order to ensure a correct behaviour across the
 * difference graphics APIs. When not doing this you usually have no issues when using OpenGL, OpenGL ES 3, Direct 10,
 * Direct3D 11 or Direct3D 9 with the "ps_2_0"-profile, but when using Direct3D 9 with the "ps_3_0"-profile you might
 * get into trouble due to another internal graphics API behaviour.
 *
 * @note
 * - This sampler state maps directly to Direct3D 10 & 11, do not change it
 * - If you want to know how the default values were chosen, have a look into the "rhi::ISamplerState::get_default_sampler_state()"-implementation
 *
 * @see
 * - "D3D12_SAMPLER_DESC"-documentation for details
 */
struct SamplerStateDescriptor final
{
  FilterMode		   filter;			///< Default: "rhi::FilterMode::MIN_MAG_MIP_LINEAR"
  TextureAddressMode addressU;		///< (also known as "S"), Default: "rhi::TextureAddressMode::CLAMP"
  TextureAddressMode addressV;		///< (also known as "T"), Default: "rhi::TextureAddressMode::CLAMP"
  TextureAddressMode addressW;		///< (also known as "R"), Default: "rhi::TextureAddressMode::CLAMP"
  float			   mipLodBias;		///< Default: "0.0f"
  core::uint32		   maxAnisotropy;	///< Default: "16"
  ComparisonFunc	   comparisonFunc;	///< Default: "rhi::ComparisonFunc::NEVER"
  float			   borderColor[4];	///< Default: 0.0f, 0.0f, 0.0f, 0.0f
  float			   minLod;			///< Default: -3.402823466e+38f (-FLT_MAX)
  float			   maxLod;			///< Default: 3.402823466e+38f (FLT_MAX)
};

/**
 * @brief
 * Rasterizer state
 *
 * @note
 * - This rasterizer state maps directly to Direct3D 12, do not change it
 * - This also means that "int" is used over "bool" because in Direct3D it's defined this way
 * - If you want to know how the default values were chosen, have a look into the "rhi::RasterizerStateBuilder::getDefaultRasterizerState()"-implementation
 * - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
 * Use the convenience "rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
 *
 * @see
 * - "D3D12_RASTERIZER_DESC"-documentation for details
 */
struct RasterizerStateDescriptor final
{
  FillMode						fillMode;						///< Default: "rhi::FillMode::SOLID"
  CullMode						cullMode;						///< Default: "rhi::CullMode::BACK"
  int								frontCounterClockwise;			///< Select counter-clockwise polygons as front-facing? Boolean value. Default: "false"
  int								depthBias;						///< Default: "0"
  float							depthBiasClamp;					///< Default: "0.0f"
  float							slopeScaledDepthBias;			///< Default: "0.0f"
  int								depthClipEnable;				///< Boolean value. Default: "true"
  int								multisampleEnable;				///< Boolean value. Default: "false"
  int								antialiasedLineEnable;			///< Boolean value. Default: "false"
  unsigned int					forcedSampleCount;				///< Default: "0"
  ConservativeRasterizationMode	conservativeRasterizationMode;	///< Boolean value. >= Direct3D 12 only. Default: "false"
  int								scissorEnable;					///< Boolean value. Not available in Vulkan or Direct3D 12 (scissor testing is always enabled). Default: "false"
};
struct RasterizerStateDescriptorBuilder final
{
  /**
   * @brief
   * Return the default rasterizer state
   *
   * @return
   * The default rasterizer state, see "rhi::RasterizerState" for the default values
   */
  [[nodiscard]] static inline const RasterizerStateDescriptor& getDefaultRasterizerState()
  {
    // As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
    // (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
    // - Direct3D 12 "D3D12_RASTERIZER_DESC structure"-documentation at MSDN: https://msdn.microsoft.com/de-de/library/windows/desktop/dn770387%28v=vs.85%29.aspx
    // - Direct3D 11 "D3D11_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476198%28v=vs.85%29.aspx
    // - Direct3D 10 "D3D10_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172408(v=vs.85).aspx
    // - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
    // - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

    // Return default values
    static constexpr RasterizerStateDescriptor RASTERIZER_STATE =
      {																												//	Direct3D 11	Direct3D 10	Direct3D 9		OpenGL
        FillMode::SOLID,					// fillMode (rhi::FillMode)												"SOLID"			"SOLID"		"SOLID"			"SOLID"
        CullMode::BACK,						// cullMode (rhi::CullMode)												"BACK"			"Back"		"BACK" (CCW)	"BACK"
        false,								// frontCounterClockwise (int)											"false"			"false"		"false" (CCW)	"true"
        0,									// depthBias (int)														"0"				"0"			"0"
        0.0f,								// depthBiasClamp (float)												"0.0f"			"0.0f"		<unsupported>
        0.0f,								// slopeScaledDepthBias (float)											"0.0f"			"0.0f"		"0.0f"
        true,								// depthClipEnable (int)												"true"			"true"		<unsupported>
        false,								// multisampleEnable (int)												"false"			"false"		"true"			"true"
        false,								// antialiasedLineEnable (int)											"false"			"false"		"false"			"false"
        0,									// forcedSampleCount (unsigned int)
        ConservativeRasterizationMode::OFF,	// conservativeRasterizationMode (rhi::ConservativeRasterizationMode)
        false								// scissorEnable (int)													"false"			"false"		"false"			"false"
      };
    return RASTERIZER_STATE;
  }
};

/**
 * @brief
 * Depth stencil state
 *
 * @note
 * - This depth stencil state maps directly to Direct3D 10 & 11 & 12, do not change it
 * - This also means that "int" is used over "bool" because in Direct3D it's defined this way
 * - If you want to know how the default values were chosen, have a look into the "rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
 *
 * @see
 * - "D3D12_DEPTH_STENCIL_DESC"-documentation for details
 */
struct DepthStencilStateDescriptor final
{
  int					depthEnable;		///< Boolean value. Default: "true"
  DepthWriteMask		depthWriteMask;		///< Default: "rhi::DepthWriteMask::ALL"
  ComparisonFunc		depthFunc;			///< Default: "rhi::ComparisonFunc::GREATER" instead of "rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
  int					stencilEnable;		///< Boolean value. Default: "false"
  core::uint8				stencilReadMask;	///< Default: "0xff"
  core::uint8				stencilWriteMask;	///< Default: "0xff"
  DepthStencilOpDesc	frontFace;			///< Default: See "rhi::DepthStencilOpDesc"
  DepthStencilOpDesc	backFace;			///< Default: See "rhi::DepthStencilOpDesc"
};
struct DepthStencilStateDescriptorBuilder final
{
  /**
   * @brief
   * Return the default depth stencil state
   *
   * @return
   * The default depth stencil state, see "rhi::DepthStencilState" for the default values
   */
  [[nodiscard]] static inline const DepthStencilStateDescriptor& getDefaultDepthStencilState()
  {
    // As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
    // (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
    // - Direct3D 11 "D3D11_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476110%28v=vs.85%29.aspx
    // - Direct3D 10 "D3D10_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb205036%28v=vs.85%29.aspx
    // - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
    // - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

    // Return default values
    static constexpr DepthStencilStateDescriptor DEPTH_STENCIL_STATE =
      {																		//	Direct3D 11		Direct3D 10	Direct3D 9				OpenGL
        true,						// depthEnable (int)						"true"			"true"		"true"					true
        DepthWriteMask::ALL,		// depthWriteMask (rhi::DepthWriteMask)		"ALL"			"ALL"		"ALL"					"ALL"
        ComparisonFunc::GREATER,	// depthFunc (rhi::ComparisonFunc)			"LESS"			"LESS"		"LESS_EQUAL"
        false,						// stencilEnable (int)						"false"			"false"		"false"
        0xff,						// stencilReadMask (core::uint8)				"0xff"			"0xff"		"0xffffffff"
        0xff,						// stencilWriteMask (core::uint8)				"0xff"			"0xff"		"0xffffffff"
        { // sFrontFace (rhi::DepthStencilOpDesc)
          StencilOp::KEEP,		// stencilFailOp (rhi::StencilOp			"KEEP"			"KEEP"		"KEEP" (both sides)
          StencilOp::KEEP,		// stencilDepthFailOp (rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)
          StencilOp::KEEP,		// stencilPassOp (rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)
          ComparisonFunc::ALWAYS	// stencilFunc (rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)
        },
        { // sBackFace (rhi::DepthStencilOpDesc)
          StencilOp::KEEP,		// stencilFailOp (rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)
          StencilOp::KEEP,		// stencilDepthFailOp (rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)
          StencilOp::KEEP,		// stencilPassOp (rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)
          ComparisonFunc::ALWAYS	// stencilFunc (rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)
        }
      };
    return DEPTH_STENCIL_STATE;
  }
};

/**
 * @brief
 * Blend state
 *
 * @note
 * - This blend state maps directly to Direct3D 10.1 & 11 & 12, do not change it
 * - This also means that "int" is used over "bool" because in Direct3D it's defined this way
 * - If you want to know how the default values were chosen, have a look into the "rhi::BlendStateBuilder::getDefaultBlendState()"-implementation
 *
 * @see
 * - "D3D12_BLEND_DESC"-documentation for details
 */
struct BlendStateDescriptor final
{
  int					  alphaToCoverageEnable;	///< Boolean value. Default: "false"
  int					  independentBlendEnable;	///< Boolean value. Default: "false"
  RenderTargetBlendDesc renderTarget[8];			///< Default: See "rhi::RenderTargetBlendDesc"
};
struct BlendStateDescriptorBuilder final
{
  /**
   * @brief
   * Return the default blend state
   *
   * @return
   * The default blend state, see "rhi::BlendState" for the default values
   */
  [[nodiscard]] static inline const BlendStateDescriptor& getDefaultBlendState()
  {
    // As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
    // (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
    // - Direct3D 11 "D3D11_BLEND_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476087%28v=vs.85%29.aspx
    // - Direct3D 10 "D3D10_BLEND_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb204893%28v=vs.85%29.aspx
    // - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
    // - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

    // Return default values
    static constexpr BlendStateDescriptor BLEND_STATE =
      {																															//	Direct3D 11	Direct3D 10	Direct3D 9			OpenGL
        false,								// alphaToCoverageEnable (int)															"false"			"false"
        false,								// independentBlendEnable (int)															"false"			"false"

        { // renderTarget[8]
          // renderTarget[0]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[1]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[2]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[3]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[4]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[5]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[6]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          },
          // renderTarget[7]
          {
            false,						// blendEnable (int)																	"false"			"false"
            Blend::ONE,					// srcBlend (rhi::Blend)																"ONE"			"ONE"
            Blend::ZERO,				// destBlend (rhi::Blend)																"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOp (rhi::BlendOp)																"ADD"			"ADD"
            Blend::ONE,					// srcBlendAlpha (rhi::Blend)															"ONE"			"ONE"
            Blend::ZERO,				// destBlendAlpha (rhi::Blend)															"ZERO"			"ZERO"
            BlendOp::ADD,				// blendOpAlpha (rhi::BlendOp)															"ADD"			"ADD"
            ColorWriteEnableFlag::ALL	// renderTargetWriteMask (core::uint8), combination of "rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
          }
        }
      };
    return BLEND_STATE;
  }
};

/**
 * @brief
 * Graphics pipeline state
 */
struct SerializedGraphicsPipelineStateDescriptor
{
  PrimitiveTopology	  primitiveTopology;			///< Input-assembler (IA) stage: Primitive topology used for draw calls
  PrimitiveTopologyType primitiveTopologyType;		///< The primitive topology type specifies how the graphics pipeline interprets geometry or hull shader input primitives
  RasterizerStateDescriptor		  rasterizerState;				///< Rasterizer state
  DepthStencilStateDescriptor	  depthStencilState;			///< Depth stencil state
  BlendStateDescriptor			  blendState;					///< Blend state
  core::uint32			  numberOfRenderTargets;		///< Number of render targets
  TextureFormat::Enum	  renderTargetViewFormats[8];	///< Render target view formats
  TextureFormat::Enum	  depthStencilViewFormat;		///< Depth stencil view formats
};
struct GraphicsPipelineStateDescriptor : public SerializedGraphicsPipelineStateDescriptor
{
  RHIRootSignature*   rootSignature;	///< Root signature (graphics pipeline state instances keep a reference to the root signature), must be valid
  RHIGraphicsProgram* graphicsProgram;	///< Graphics program used by the graphics pipeline state (graphics pipeline state instances keep a reference to the graphics program), must be valid
  VertexAttributes  vertexAttributes;	///< Vertex attributes, can be empty e.g. in case a task and mesh shader is used
  RHIRenderPass*	  renderPass;		///< Render pass, the graphics pipeline state keeps a reference, must be valid
};
struct GraphicsPipelineStateDescriptorBuilder final : public GraphicsPipelineStateDescriptor
{
  inline GraphicsPipelineStateDescriptorBuilder()
  {
    // "GraphicsPipelineState"-part
    rootSignature						= nullptr;
    graphicsProgram						= nullptr;
    vertexAttributes.numberOfAttributes	= 0;
    vertexAttributes.attributes			= nullptr;
    renderPass							= nullptr;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology		   = PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType	   = PrimitiveTopologyType::TRIANGLE;
    rasterizerState			   = RasterizerStateDescriptorBuilder::getDefaultRasterizerState();
    depthStencilState		   = DepthStencilStateDescriptorBuilder::getDefaultDepthStencilState();
    blendState				   = BlendStateDescriptorBuilder::getDefaultBlendState();
    numberOfRenderTargets	   = 1;
    renderTargetViewFormats[0] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7] = TextureFormat::R8G8B8A8;
    depthStencilViewFormat	   = TextureFormat::D32_FLOAT;
  }

  inline GraphicsPipelineStateDescriptorBuilder(RHIRootSignature* _rootSignature, RHIGraphicsProgram* _graphicsProgram, RHIRenderPass& _renderPass)
  {
    // "GraphicsPipelineState"-part which has no vertex attributes, e.g. because the graphics program is using a task and mesh shader
    rootSignature	= _rootSignature;
    graphicsProgram = _graphicsProgram;
    renderPass		= &_renderPass;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
    rasterizerState				= RasterizerStateDescriptorBuilder::getDefaultRasterizerState();
    depthStencilState			= DepthStencilStateDescriptorBuilder::getDefaultDepthStencilState();
    blendState					= BlendStateDescriptorBuilder::getDefaultBlendState();
    numberOfRenderTargets		= 1;
    renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
    depthStencilViewFormat		= TextureFormat::D32_FLOAT;
  }

  inline GraphicsPipelineStateDescriptorBuilder(RHIRootSignature* _rootSignature, RHIGraphicsProgram* _graphicsProgram, const VertexAttributes& _vertexAttributes, RHIRenderPass& _renderPass)
  {
    // "GraphicsPipelineState"-part
    rootSignature	 = _rootSignature;
    graphicsProgram	 = _graphicsProgram;
    vertexAttributes = _vertexAttributes;
    renderPass		 = &_renderPass;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
    rasterizerState				= RasterizerStateDescriptorBuilder::getDefaultRasterizerState();
    depthStencilState			= DepthStencilStateDescriptorBuilder::getDefaultDepthStencilState();
    blendState					= BlendStateDescriptorBuilder::getDefaultBlendState();
    numberOfRenderTargets		= 1;
    renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
    depthStencilViewFormat		= TextureFormat::D32_FLOAT;
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
