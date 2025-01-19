if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 Position;	// Object space vertex position
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
void main()
{
	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 1.0);
	TexCoord = normalize(Position);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 TexCoord;
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform samplerCube CubeMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMap, TexCoord);
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

// Attribute input/output
layout(location = 0) in  vec3 Position;	// Object space vertex position
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
void main()
{
	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 1.0);
	TexCoord = normalize(Position);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 1) in  vec3 TexCoord;		// Normalized texture coordinate as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(set = 0, binding = 1) uniform samplerCubeArray CubeMapArray;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMapArray, vec4(TexCoord, 1.0f));	// The cube at index zero is black by intent, use index one
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::OPENGLES3)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  highp vec3 Position;	// Object space vertex position
out highp vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
void main()
{
	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 1.0);
	TexCoord = normalize(Position);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec3 TexCoord;		// Normalized texture coordinate as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform mediump samplerCube CubeMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMap, TexCoord);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D9)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float3 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Uniforms
uniform float4x4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix

// Programs
VS_OUTPUT main(float3 Position : POSITION)	// Object space vertex position as input
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = mul(float4(Position, 1.0f), ObjectSpaceToClipSpaceMatrix);
	output.TexCoord = normalize(Position);
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
uniform samplerCUBE CubeMap : register(s0);

// Programs
float4 main(float4 Position : SV_POSITION, float3 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return texCUBE(CubeMap, TexCoord).bgra;	// Direct3D 9 is using BGR, the world is using RGB
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D10 || rhi->get_name_id() == rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float3 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Uniforms
cbuffer UniformBlockDynamicVs : register(b0)
{
	float4x4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
}

// Programs
VS_OUTPUT main(float3 Position : POSITION)	// Object space vertex position as input
{
	// Vertex output
	VS_OUTPUT output;
	output.Position = mul(ObjectSpaceToClipSpaceMatrix, float4(Position, 1.0)); // Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.TexCoord = normalize(Position);
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
SamplerState SamplerPoint : register(s0);
TextureCube CubeMap : register(t0);

// Programs
float4 main(float4 Position : SV_POSITION, float3 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return CubeMap.Sample(SamplerPoint, TexCoord);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D11)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float3 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Uniforms
cbuffer UniformBlockDynamicVs : register(b0)
{
	float4x4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
}

// Programs
VS_OUTPUT main(float3 Position : POSITION)	// Object space vertex position as input
{
	// Vertex output
	VS_OUTPUT output;
	output.Position = mul(ObjectSpaceToClipSpaceMatrix, float4(Position, 1.0)); // Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.TexCoord = normalize(Position);
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
SamplerState SamplerPoint : register(s0);
TextureCubeArray CubeMapArray : register(t0);

// Programs
float4 main(float4 Position : SV_POSITION, float3 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return CubeMapArray.Sample(SamplerPoint, float4(TexCoord, 1.0f));	// The cube at index zero is black by intent, use index one
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::NULL_DUMMY)
{
vertexShaderSourceCode = fragmentShaderSourceCode = "42";
}
else
{
// Error! (unsupported RHI)
BE_LOG(Critical, "Error: Unsupported RHI")
}
