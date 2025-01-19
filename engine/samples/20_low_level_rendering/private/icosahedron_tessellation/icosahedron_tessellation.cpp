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
#include "icosahedron_tessellation/icosahedron_tessellation.h"
#include <core/color/color4.h>
#include <core/math/mat44.h>
#include <core/math/vec3.h>
#include <float.h> // For FLT_MAX
#include <stdlib.h> // For rand()




//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void IcosahedronTessellation::on_initialization()
{
	// get and check the RHI instance
	// -> Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> Geometry shaders supported?
	// -> Tessellation control and tessellation evaluation shaders supported?
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi && rhi->get_capabilities().maximumUniformBufferSize > 0 &&
    rhi->get_capabilities().maximumNumberOfGsOutputVertices > 0 &&
    rhi->get_capabilities().maximumNumberOfPatchVertices > 0)
	{
		// Create the buffer manager
		mBufferManager = rhi->create_buffer_manager();

		{ // Create the root signature
			// Setup
			rhi::DescriptorRangeBuilder ranges[4];
			ranges[0].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicTcs", rhi::ShaderVisibility::TESSELLATION_CONTROL);
			ranges[1].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticTes",  rhi::ShaderVisibility::TESSELLATION_EVALUATION);
			ranges[2].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticGs",   rhi::ShaderVisibility::GEOMETRY);
			ranges[3].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticFs",   rhi::ShaderVisibility::FRAGMENT);

			rhi::RootParameterBuilder rootParameters[4];
			rootParameters[0].initializeAsDescriptorTable(1, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[1]);
			rootParameters[2].initializeAsDescriptorTable(1, &ranges[2]);
			rootParameters[3].initializeAsDescriptorTable(1, &ranges[3]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(4, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->create_root_signature(rootSignatureBuilder);
		}

		// Vertex input layout
		static constexpr rhi::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				0,										// alignedByteOffset (core::uint32)
				sizeof(float) * 3,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			}
		};
		const rhi::VertexAttributes vertexAttributes(1, vertexAttributesLayout);

		{ // Create vertex array object (VAO)
			// Create the vertex buffer object (VBO)
			// -> Geometry is from: http://prideout.net/blog/?p=48 (Philip Rideout, "The Little Grasshopper - Graphics Programming Tips")
			static constexpr float VERTEX_POSITION[] =
			{								// Vertex ID
				 0.000f,  0.000f,  1.000f,	// 0
				 0.894f,  0.000f,  0.447f,	// 1
				 0.276f,  0.851f,  0.447f,	// 2
				-0.724f,  0.526f,  0.447f,	// 3
				-0.724f, -0.526f,  0.447f,	// 4
				 0.276f, -0.851f,  0.447f,	// 5
				 0.724f,  0.526f, -0.447f,	// 6
				-0.276f,  0.851f, -0.447f,	// 7
				-0.894f,  0.000f, -0.447f,	// 8
				-0.276f, -0.851f, -0.447f,	// 9
				 0.724f, -0.526f, -0.447f,	// 10
				 0.000f,  0.000f, -1.000f	// 11
			};
			rhi::RHIVertexBufferPtr vertexBuffer(
        mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION}));

			// Create the index buffer object (IBO)
			// -> Geometry is from: http://prideout.net/blog/?p=48 (Philip Rideout, "The Little Grasshopper - Graphics Programming Tips")
			static constexpr core::uint16 INDICES[] =
			{				// Triangle ID
				0,  1,  2,	// 0
				0,  2,  3,	// 1
				0,  3,  4,	// 2
				0,  4,  5,	// 3
				0,  5,  1,	// 4
				7,  6,  11,	// 5
				8,  7,  11,	// 6
				9,  8,  11,	// 7
				10,  9, 11,	// 8
				6, 10,  11,	// 9
				6,  2,  1,	// 10
				7,  3,  2,	// 11
				8,  4,  3,	// 12
				9,  5,  4,	// 13
				10,  1, 5,	// 14
				6,  7,  2,	// 15
				7,  8,  3,	// 16
				8,  9,  4,	// 17
				9, 10,  5,	// 18
				10,  6, 1	// 19
			};
			rhi::RHIIndexBufferPtr indexBuffer(mBufferManager->create_index_buffer({sizeof(INDICES), INDICES}));

			// Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
			mVertexArray = mBufferManager->create_vertex_array({vertexAttributes, 1, vertexArrayVertexBuffers, indexBuffer});
		}

		{ // Create the uniform buffer group with tessellation control shader visibility
			mUniformBufferDynamicTcs = mBufferManager->create_uniform_buffer(
				{sizeof(float) * 2, nullptr, rhi::BufferUsage::DYNAMIC_DRAW});
			rhi::RHIResource* resources[1] = { mUniformBufferDynamicTcs };
			mUniformBufferGroupTcs = mRootSignature->create_resource_group(0, 1, resources);
		}

		{ // Create the uniform buffer group with tessellation evaluation shader visibility: "ObjectSpaceToClipSpaceMatrix"
      const core::Mat44f worldSpaceToViewSpaceMatrix = core::Mat44f::create_translation(core::Vec3f(0.0f, 0.0f, 3.0f));			// Also known as "view matrix"
      const core::Mat44f viewSpaceToClipSpaceMatrix = core::Mat44f::create_perspective(core::Math::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);	// Also known as "projection matrix", near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
      const core::Mat44f objectSpaceToClipSpaceMatrix = viewSpaceToClipSpaceMatrix * worldSpaceToViewSpaceMatrix;			// Also known as "model view projection matrix"
			rhi::RHIResource* resources[1] = {
        mBufferManager->create_uniform_buffer({sizeof(float) * 4 * 4, (objectSpaceToClipSpaceMatrix)}) };
			mUniformBufferGroupTes = mRootSignature->create_resource_group(1, 1, resources);
		}

		{ // Create the uniform buffer group with geometry visibility: "NormalMatrix"
      const core::Mat44f worldSpaceToViewSpaceMatrix = core::Mat44f::create_translation(core::Vec3f(0.0f, 0.0f, 0.0f));
      const core::Mat44f viewSpaceToClipSpaceMatrix = core::Mat44f::create_perspective(core::Math::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
      const core::Mat44f objectSpaceToClipSpaceMatrix = viewSpaceToClipSpaceMatrix * worldSpaceToViewSpaceMatrix;
      const core::Mat44f tMVP(objectSpaceToClipSpaceMatrix);
			rhi::RHIResource* resources[1] = {
        mBufferManager->create_uniform_buffer({sizeof(float) * 4 * 4, (tMVP)}) };
			mUniformBufferGroupGs = mRootSignature->create_resource_group(2,
                                                                    1,
                                                                    resources);
		}

		{ // Create the uniform buffer group with fragment shader visibility: Light and material
			static constexpr float LIGHT_AND_MATERIAL[] =
			{
				0.25f, 0.25f, 1.0f,  1.0,	// "LightPosition"
				 0.0f, 0.75f, 0.75f, 1.0, 	// "DiffuseMaterial"
				0.04f, 0.04f, 0.04f, 1.0,	// "AmbientMaterial"
			};
			rhi::RHIResource* resources[1] = {
        mBufferManager->create_uniform_buffer({sizeof(LIGHT_AND_MATERIAL), LIGHT_AND_MATERIAL}) };
			mUniformBufferGroupFs = mRootSignature->create_resource_group(3,
                                                                    1,
                                                                    resources);
		}

		{
			// Create the graphics program
			rhi::RHIGraphicsProgramPtr graphicsProgram;
			{
				// get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* tessellationControlShaderSourceCode = nullptr;
				const char* tessellationEvaluationShaderSourceCode = nullptr;
				const char* geometryShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "icosahedron_tessellation/shaders.inl"

				// Create the graphics program
				rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributes,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
          shaderLanguage.create_tessellation_control_shader_from_source_code(tessellationControlShaderSourceCode),
          shaderLanguage.create_tessellation_evaluation_shader_from_source_code(tessellationEvaluationShaderSourceCode),
          shaderLanguage.create_geometry_shader_from_source_code(geometryShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                                                                                     get_main_render_target()->get_render_pass());
				graphicsPipelineState.primitiveTopology = rhi::PrimitiveTopology::PATCH_LIST_3;	// Patch list with 3 vertices per patch (tessellation relevant topology type) - "rhi::PrimitiveTopology::TriangleList" used for tessellation
				graphicsPipelineState.primitiveTopologyType = rhi::PrimitiveTopologyType::PATCH;
				mGraphicsPipelineState = rhi->create_graphics_pipeline_state(graphicsPipelineState);
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void IcosahedronTessellation::on_deinitialization()
{
	// release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mUniformBufferGroupTcs = nullptr;
	mUniformBufferGroupTes = nullptr;
	mUniformBufferGroupGs = nullptr;
	mUniformBufferGroupFs = nullptr;
	mUniformBufferDynamicTcs = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void IcosahedronTessellation::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Update the uniform buffer content
	if (nullptr != mUniformBufferDynamicTcs)
	{
		// Copy data into the uniform buffer
		const float data[] =
		{
			mTessellationLevelOuter,	// "TessellationLevelOuter"
			mTessellationLevelInner		// "TessellationLevelInner"
		};
		rhi::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBufferDynamicTcs, data, sizeof(data));
	}

	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void IcosahedronTessellation::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mUniformBufferDynamicTcs, "Invalid uniform buffer dynamic TCS")
	BE_ASSERT(nullptr != mUniformBufferGroupTcs && nullptr != mUniformBufferGroupTes && nullptr != mUniformBufferGroupGs && nullptr != mUniformBufferGroupFs, "Invalid uniform buffer group")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Set graphics resource groups
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mUniformBufferGroupTcs);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mUniformBufferGroupTes);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 2, mUniformBufferGroupGs);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 3, mUniformBufferGroupFs);

	// Input assembly (IA): Set the used vertex array
	rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on indexing into an array of vertices
	rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, 60);
}
