if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Attribute input/output
in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(binding = 1) uniform samplerBuffer InputTextureBuffer;
layout(std430, binding = 2) readonly buffer InputStructuredBuffer
{
	Vertex inputStructuredBufferVertex[];
};

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position + texelFetch(InputTextureBuffer, gl_VertexID).xy + inputStructuredBufferVertex[gl_VertexID].position, 0.5f, 1.0f);
	TexCoord = Position.xy;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 430 core	// OpenGL 4.3

// Attribute input/output
in  vec2 TexCoord;		// Normalized texture coordinate as input
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(std140, binding = 0) uniform UniformBuffer
{
	vec4 inputColorUniform;
};
layout(binding = 3) uniform sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture2D(AlbedoMap, TexCoord) * inputColorUniform;
}
)";


//[-------------------------------------------------------]
//[ Compute shader source code                            ]
//[-------------------------------------------------------]
computeShaderSourceCode1 = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
};

// Input
layout(binding = 0) uniform sampler2D InputTexture2D;
layout(std430, binding = 1) readonly buffer InputVertexBuffer
{
	Vertex inputVertices[3];
};
layout(std430, binding = 2) readonly buffer InputIndexBuffer
{
	uint inputIndices[3];
};
layout(std140, binding = 3) uniform InputUniformBuffer
{
	vec4 inputColorUniform;
};

// Output
layout(rgba8, binding = 4) writeonly uniform image2D OutputTexture2D;
layout(std430, binding = 5) writeonly buffer OutputVertexBuffer
{
	Vertex outputVertices[3];
};
layout(std430, binding = 6) writeonly buffer OutputIndexBuffer
{
	uint outputIndices[3];
};

// Programs
layout (local_size_x = 16, local_size_y = 16) in;
void main()
{
	// Fetch input texel
	vec4 color = texelFetch(InputTexture2D, ivec2(gl_GlobalInvocationID.xy), 0) * inputColorUniform;

	// Modify color
	color.g *= 1.0f - (float(gl_GlobalInvocationID.x) / 16.0f);
	color.g *= 1.0f - (float(gl_GlobalInvocationID.y) / 16.0f);

	// Output texel
	imageStore(OutputTexture2D, ivec2(gl_GlobalInvocationID.xy), color);

	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output vertex buffer values
		for (int vertexBufferIndex = 0; vertexBufferIndex < 3; ++vertexBufferIndex)
		{
			outputVertices[vertexBufferIndex] = inputVertices[vertexBufferIndex];
		}

		// Output index buffer values
		for (int indexBufferIndex = 0; indexBufferIndex < 3; ++indexBufferIndex)
		{
			outputIndices[indexBufferIndex] = inputIndices[indexBufferIndex];
		}

		// Output uniform buffer not possible by design
	}
}
)";

computeShaderSourceCode2 = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Same layout as "rhi::DrawIndexedArguments"
struct DrawIndexedArguments
{
	uint indexCountPerInstance;
	uint instanceCount;
	uint startIndexLocation;
	uint baseVertexLocation;
	uint startInstanceLocation;
};

// Input
layout(binding = 0) uniform samplerBuffer InputTextureBuffer;
layout(std430, binding = 1) readonly buffer InputStructuredBuffer
{
	Vertex inputStructuredBufferVertex[];
};
layout(std430, binding = 2) readonly buffer InputIndirectBuffer
{
	DrawIndexedArguments inputDrawIndexedArguments;
};

// Output
layout(rgba32f, binding = 3) writeonly uniform imageBuffer OutputTextureBuffer;
layout(std430, binding = 4) writeonly buffer OutputStructuredBuffer
{
	Vertex outputStructuredBufferVertex[];
};
layout(std430, binding = 5) writeonly buffer OutputIndirectBuffer
{
	DrawIndexedArguments outputDrawIndexedArguments;
};

// Programs
layout (local_size_x = 3, local_size_y = 1) in;
void main()
{
	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output texture buffer values
		for (int textureBufferIndex = 0; textureBufferIndex < 3; ++textureBufferIndex)
		{
			imageStore(OutputTextureBuffer, textureBufferIndex, texelFetch(InputTextureBuffer, textureBufferIndex));
		}

		// Output structured buffer values
		for (int structuredBufferIndex = 0; structuredBufferIndex < 3; ++structuredBufferIndex)
		{
			outputStructuredBufferVertex[structuredBufferIndex] = inputStructuredBufferVertex[structuredBufferIndex];
		}

		// Output indirect buffer values (draw calls)
		// outputDrawIndexedArguments.indexCountPerInstance = inputDrawIndexedArguments.indexCountPerInstance;	- Filled by compute shader via atomics counting
		outputDrawIndexedArguments.instanceCount		 = inputDrawIndexedArguments.instanceCount;
		outputDrawIndexedArguments.startIndexLocation	 = inputDrawIndexedArguments.startIndexLocation;
		outputDrawIndexedArguments.baseVertexLocation	 = inputDrawIndexedArguments.baseVertexLocation;
		outputDrawIndexedArguments.startInstanceLocation = inputDrawIndexedArguments.startInstanceLocation;

		// Output uniform buffer not possible by design
	}

	// Atomics for counting usage example
	// -> Change 'layout (local_size_x = 3, local_size_y = 1) in;' into 'layout (local_size_x = 1, local_size_y = 1) in;' and if the triangle is gone you know the counter reset worked
	if (0 == gl_GlobalInvocationID.x)
	{
		// reset the counter on first invocation
		atomicExchange(outputDrawIndexedArguments.indexCountPerInstance, 0);
	}
	atomicAdd(outputDrawIndexedArguments.indexCountPerInstance, 1);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::VULKAN)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Attribute input/output
layout(location = 0) in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec2 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(set = 0, binding = 1) uniform samplerBuffer InputTextureBuffer;
layout(std430, set = 0, binding = 2) readonly buffer InputStructuredBuffer	// TODO(naetherm) Triggers "Fix NonWritable check when vertexPipelineStoresAndAtomics not enabled #2526" - https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/issues/2526
{
	Vertex inputStructuredBufferVertex[];
};

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position + texelFetch(InputTextureBuffer, gl_VertexIndex).xy + inputStructuredBufferVertex[gl_VertexIndex].position, 0.5f, 1.0f);
	TexCoord = Position.xy;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 1) in  vec2 TexCoord;		// Normalized texture coordinate as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBuffer
{
	vec4 inputColorUniform;
};
layout(set = 0, binding = 3) uniform sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(AlbedoMap, TexCoord) * inputColorUniform;
}
)";


//[-------------------------------------------------------]
//[ Compute shader source code                            ]
//[-------------------------------------------------------]
computeShaderSourceCode1 = R"(#version 450 core	// OpenGL 4.5
struct Vertex
{
	vec2 position;
};

// Input
layout(set = 0, binding = 0) uniform sampler2D InputTexture2D;
layout(std430, set = 0, binding = 1) readonly buffer InputVertexBuffer
{
	Vertex inputVertices[3];
};
layout(std430, set = 0, binding = 2) readonly buffer InputIndexBuffer
{
	uint inputIndices[3];
};
layout(std140, set = 0, binding = 3) uniform InputUniformBuffer
{
	vec4 inputColorUniform;
};

// Output
layout(rgba8, set = 0, binding = 4) writeonly uniform image2D OutputTexture2D;
layout(std430, set = 0, binding = 5) writeonly buffer OutputVertexBuffer
{
	Vertex outputVertices[3];
};
layout(std430, set = 0, binding = 6) writeonly buffer OutputIndexBuffer
{
	uint outputIndices[3];
};

// Programs
layout (local_size_x = 16, local_size_y = 16) in;
void main()
{
	// Fetch input texel
	vec4 color = texelFetch(InputTexture2D, ivec2(gl_GlobalInvocationID.xy), 0) * inputColorUniform;

	// Modify color
	color.g *= 1.0f - (float(gl_GlobalInvocationID.x) / 16.0f);
	color.g *= 1.0f - (float(gl_GlobalInvocationID.y) / 16.0f);

	// Output texel
	imageStore(OutputTexture2D, ivec2(gl_GlobalInvocationID.xy), color);

	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output vertex buffer values
		for (int vertexBufferIndex = 0; vertexBufferIndex < 3; ++vertexBufferIndex)
		{
			outputVertices[vertexBufferIndex] = inputVertices[vertexBufferIndex];
		}

		// Output index buffer values
		for (int indexBufferIndex = 0; indexBufferIndex < 3; ++indexBufferIndex)
		{
			outputIndices[indexBufferIndex] = inputIndices[indexBufferIndex];
		}

		// Output uniform buffer not possible by design
	}
}
)";

computeShaderSourceCode2 = R"(#version 450 core	// OpenGL 4.5
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Same layout as "rhi::DrawIndexedArguments"
struct DrawIndexedArguments
{
	uint indexCountPerInstance;
	uint instanceCount;
	uint startIndexLocation;
	uint baseVertexLocation;
	uint startInstanceLocation;
};

// Input
layout(set = 0, binding = 0) uniform samplerBuffer InputTextureBuffer;
layout(std430, set = 0, binding = 1) readonly buffer InputStructuredBuffer
{
	Vertex inputStructuredBufferVertex[];
};
layout(std430, set = 0, binding = 2) readonly buffer InputIndirectBuffer
{
	DrawIndexedArguments inputDrawIndexedArguments;
};

// Output
layout(rgba32f, set = 0, binding = 3) writeonly uniform imageBuffer OutputTextureBuffer;
layout(std430, set = 0, binding = 4) writeonly buffer OutputStructuredBuffer
{
	Vertex outputStructuredBufferVertex[];
};
layout(std430, set = 0, binding = 5) writeonly buffer OutputIndirectBuffer
{
	DrawIndexedArguments outputDrawIndexedArguments;
};

// Programs
layout (local_size_x = 3, local_size_y = 1) in;
void main()
{
	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output texture buffer values
		for (int textureBufferIndex = 0; textureBufferIndex < 3; ++textureBufferIndex)
		{
			imageStore(OutputTextureBuffer, textureBufferIndex, texelFetch(InputTextureBuffer, textureBufferIndex));
		}

		// Output structured buffer values
		for (int structuredBufferIndex = 0; structuredBufferIndex < 3; ++structuredBufferIndex)
		{
			outputStructuredBufferVertex[structuredBufferIndex] = inputStructuredBufferVertex[structuredBufferIndex];
		}

		// Output indirect buffer values (draw calls)
		// outputDrawIndexedArguments.indexCountPerInstance = inputDrawIndexedArguments.indexCountPerInstance;	- Filled by compute shader via atomics counting
		outputDrawIndexedArguments.instanceCount		 = inputDrawIndexedArguments.instanceCount;
		outputDrawIndexedArguments.startIndexLocation	 = inputDrawIndexedArguments.startIndexLocation;
		outputDrawIndexedArguments.baseVertexLocation	 = inputDrawIndexedArguments.baseVertexLocation;
		outputDrawIndexedArguments.startInstanceLocation = inputDrawIndexedArguments.startInstanceLocation;
	}

	// Atomics for counting usage example
	// -> Change 'layout (local_size_x = 3, local_size_y = 1) in;' into 'layout (local_size_x = 1, local_size_y = 1) in;' and if the triangle is gone you know the counter reset worked
	if (0 == gl_GlobalInvocationID.x)
	{
		// reset the counter on first invocation
		atomicExchange(outputDrawIndexedArguments.indexCountPerInstance, 0);
	}
	atomicAdd(outputDrawIndexedArguments.indexCountPerInstance, 1);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D10 || rhi->get_name_id() == rhi::NameId::DIRECT3D11 || rhi->get_name_id() == rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
struct Vertex
{
	float2 position;
	float2 padding;
};

// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float2 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Uniforms
tbuffer InputTextureBuffer : register(t0)
{
	float4 inputPositionOffset[3];
};
StructuredBuffer<Vertex> InputStructuredBuffer : register(t1);

// Programs
VS_OUTPUT main(float2 Position : POSITION,	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
			   uint   VertexId : SV_VERTEXID)
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = float4(Position + inputPositionOffset[VertexId].xy + InputStructuredBuffer[VertexId].position, 0.5f, 1.0f);
	output.TexCoord = Position.xy;
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode = R"(
// Uniforms
cbuffer InputUniformBuffer : register(b0)
{
	float4 inputColorUniform;
}
SamplerState SamplerLinear : register(s0);
Texture2D AlbedoMap : register(t1);

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return AlbedoMap.Sample(SamplerLinear, TexCoord) * inputColorUniform;
}
)";


//[-------------------------------------------------------]
//[ Compute shader source code                            ]
//[-------------------------------------------------------]
computeShaderSourceCode1 = R"(
// Input
Texture2D<float4>	InputTexture2D		 : register(t0);
ByteAddressBuffer	InputVertexBuffer	 : register(t1);
tbuffer				InputIndexBuffer	 : register(t2)
{
	uint inputIndexBuffer[3];
};
cbuffer				InputUniformBuffer	 : register(b0)
{
	float4 inputColorUniform;
}

// Output
RWTexture2D<float4>	OutputTexture2D		 : register(u0);
RWByteAddressBuffer	OutputVertexBuffer   : register(u1);
RWBuffer<uint>		OutputIndexBuffer    : register(u2);

// Programs
[numthreads(16, 16, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
	// Fetch input texel
	float4 color = InputTexture2D.Load(dispatchThreadId) * inputColorUniform;

	// Modify color
	color.g *= 1.0f - (float(dispatchThreadId.x) / 16.0f);
	color.g *= 1.0f - (float(dispatchThreadId.y) / 16.0f);

	// Output texel
	OutputTexture2D[dispatchThreadId.xy] = color;

	// Output buffer
	if (0 == dispatchThreadId.x && 0 == dispatchThreadId.y && 0 == dispatchThreadId.z)
	{
		// Output vertex buffer values
		// -> Using a structured vertex buffer would be handy inside shader source codes, sadly this isn't possible with Direct3D 11 and will result in the following error:
		//    D3D11 ERROR: ID3D11Device::CreateBuffer: Buffers created with D3D11_RESOURCE_MISC_BUFFER_STRUCTURED cannot specify any of the following listed bind flags.  The following BindFlags bits (0x9) are set: D3D11_BIND_VERTEX_BUFFER (1), D3D11_BIND_INDEX_BUFFER (0), D3D11_BIND_CONSTANT_BUFFER (0), D3D11_BIND_STREAM_OUTPUT (0), D3D11_BIND_RENDER_TARGET (0), or D3D11_BIND_DEPTH_STENCIL (0). [ STATE_CREATION ERROR #68: CREATEBUFFER_INVALIDMISCFLAGS]
		for (int vertexBufferIndex = 0; vertexBufferIndex < 3; ++vertexBufferIndex)
		{
			float2 position = asfloat(InputVertexBuffer.Load2(vertexBufferIndex * 8));
			OutputVertexBuffer.Store2(vertexBufferIndex * 8, asuint(position));
		}

		// Output index buffer values
		for (int indexBufferIndex = 0; indexBufferIndex < 3; ++indexBufferIndex)
		{
			OutputIndexBuffer[indexBufferIndex] = inputIndexBuffer[indexBufferIndex];
		}

		// Output uniform buffer not possible by design
	}
}
)";

computeShaderSourceCode2 = R"(
struct Vertex
{
	float2 position;
	float2 padding;
};

// Input
tbuffer					 InputTextureBuffer    : register(t0)
{
	float4 inputPositionOffset[3];
};
StructuredBuffer<Vertex> InputStructuredBuffer : register(t1);
tbuffer					 InputIndirectBuffer   : register(t2)
{
	uint inputIndirectBuffer[5];
};

// Output
RWBuffer<float4>		   OutputTextureBuffer    : register(u0);
RWStructuredBuffer<Vertex> OutputStructuredBuffer : register(u1);
RWBuffer<uint>			   OutputIndirectBuffer   : register(u2);

// Programs
[numthreads(3, 1, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
	// Output buffer
	if (0 == dispatchThreadId.x && 0 == dispatchThreadId.y && 0 == dispatchThreadId.z)
	{
		// Output texture buffer values
		for (int textureBufferIndex = 0; textureBufferIndex < 3; ++textureBufferIndex)
		{
			OutputTextureBuffer[textureBufferIndex] = inputPositionOffset[textureBufferIndex];
		}

		// Output structured buffer values
		for (int structuredBufferIndex = 0; structuredBufferIndex < 3; ++structuredBufferIndex)
		{
			OutputStructuredBuffer[structuredBufferIndex] = InputStructuredBuffer[structuredBufferIndex];
		}

		// Output indirect buffer values (draw calls)
		// -> Using a structured indirect buffer would be handy inside shader source codes, sadly this isn't possible with Direct3D 11 and will result in the following error:
		//    "D3D11 ERROR: ID3D11Device::CreateBuffer: A resource cannot created with both D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS and D3D11_RESOURCE_MISC_BUFFER_STRUCTURED. [ STATE_CREATION ERROR #68: CREATEBUFFER_INVALIDMISCFLAGS]"
	//	OutputIndirectBuffer[0] = inputIndirectBuffer[0];	// rhi::DrawIndexedArguments::indexCountPerInstance	- Filled by compute shader via atomics counting
		OutputIndirectBuffer[1] = inputIndirectBuffer[1];	// rhi::DrawIndexedArguments::instanceCount
		OutputIndirectBuffer[2] = inputIndirectBuffer[2];	// rhi::DrawIndexedArguments::startIndexLocation
		OutputIndirectBuffer[3] = inputIndirectBuffer[3];	// rhi::DrawIndexedArguments::baseVertexLocation
		OutputIndirectBuffer[4] = inputIndirectBuffer[4];	// rhi::DrawIndexedArguments::startInstanceLocation
	}

	// Atomics for counting usage example
	// -> Change '[numthreads(3, 1, 1)]' into '[numthreads(2, 1, 1)]' and if the triangle is gone you know the counter reset worked
	if (0 == dispatchThreadId.x)
	{
		// Reset the counter on first invocation
		uint originalValue;
		InterlockedExchange(OutputIndirectBuffer[0], 0, originalValue);
	}
	InterlockedAdd(OutputIndirectBuffer[0], 1);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::NULL_DUMMY)
{
vertexShaderSourceCode = fragmentShaderSourceCode = computeShaderSourceCode1 = computeShaderSourceCode2 = "42";
}
else
{
// Error! (unsupported RHI)
BE_LOG(Critical, "Error: Unsupported RHI")
}
