if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 TexCoord;	// Normalized texture coordinate as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);
	TexCoord = Position.xy;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 TexCoord;
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform sampler1D GradientMap;
uniform sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(GradientMap, TexCoord.y).r * texture(AlbedoMap, TexCoord);
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
layout(location = 0) in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec2 TexCoord;	// Normalized texture coordinate as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);
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
layout(set = 0, binding = 0) uniform sampler1D GradientMap;
layout(set = 0, binding = 1) uniform sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(GradientMap, TexCoord.y).r * texture(AlbedoMap, TexCoord);
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
in  highp vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
out highp vec2 TexCoord;	// Normalized texture coordinate as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);
	TexCoord = Position.xy;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec2 TexCoord;		// Normalized texture coordinate as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform mediump sampler2D GradientMap;	// OpenGL ES 3 has no 1D textures, just use a 2D texture with a height of one
uniform mediump sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(GradientMap, vec2(TexCoord.y, 0.0f)).r * texture(AlbedoMap, TexCoord);
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
	float2 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Programs
VS_OUTPUT main(float2 Position : POSITION)	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = float4(Position, 0.5f, 1.0f);
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
uniform sampler1D GradientMap : register(s0);
uniform sampler2D AlbedoMap  : register(s1);

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return tex1D(GradientMap, TexCoord.y).r * tex2D(AlbedoMap, TexCoord).bgra;	// Direct3D 9 is using BGR, the world is using RGB
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
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float2 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Programs
VS_OUTPUT main(float2 Position : POSITION)	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = float4(Position, 0.5f, 1.0f);
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
SamplerState SamplerLinear : register(s0);
SamplerState SamplerPoint : register(s1);
Texture1D GradientMap : register(t0);
Texture2D AlbedoMap  : register(t1);

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return GradientMap.Sample(SamplerLinear, TexCoord.y).r * AlbedoMap.Sample(SamplerPoint, TexCoord);
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
