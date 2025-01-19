if (mRhi->get_name_id() == rhi::NameId::OPENGL)
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

	// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
	// -> In OpenGL, the texture origin is left/bottom which maps well to clip space coordinates
	// -> (-1,-1) -> (0,0)
	// -> (1,1) -> (1,1)
	TexCoord = Position.xy * 0.5 + 0.5;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content generation      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode_ContentGeneration = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 TexCoord;		// Normalized texture coordinate as input
out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return the color green
	OutputColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content processing      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode_ContentProcessing = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 TexCoord;		// Normalized texture coordinate as input
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform sampler2D ContentMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	// -> Apply a simple wobble to the texture coordinate so we can see that content processing is up and running
	OutputColor = texture2D(ContentMap, vec2(TexCoord.x + sin(TexCoord.x * 100.0) * 0.01, TexCoord.y + cos(TexCoord.y * 100.0) * 0.01));
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (mRhi->get_name_id() == rhi::NameId::OPENGLES3)
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

	// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
	// -> In OpenGL ES 3, the texture origin is left/bottom which maps well to clip space coordinates
	// -> (-1,-1) -> (0,0)
	// -> (1,1) -> (1,1)
	TexCoord = Position.xy * 0.5 + 0.5;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content generation      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode_ContentGeneration = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec2 TexCoord;		// Normalized texture coordinate as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return the color green
	OutputColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content processing      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode_ContentProcessing = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec2 TexCoord;		// Normalized texture coordinate as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform mediump sampler2D ContentMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	// -> Apply a simple wobble to the texture coordinate so we can see that content processing is up and running
	OutputColor = texture(ContentMap, vec2(TexCoord.x + sin(TexCoord.x * 100.0) * 0.01, TexCoord.y + cos(TexCoord.y * 100.0) * 0.01));
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (mRhi->get_name_id() == rhi::NameId::DIRECT3D9)
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
	VS_OUTPUT output;

	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(Position, 0.5f, 1.0f);

	// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
	// -> Unlike OpenGL or OpenGL ES 3, in Direct3D 9 & 10 & 11 the texture origin is left/top which does not map well to clip space coordinates
	// -> We have to flip the y-axis to map the coordinate system to the Direct3D 9 & 10 & 11 texture coordinate system
	// -> In this example we do this within the fragment shader so have identical wobble across the different graphics APIs
	// -> (-1,-1) -> (0,0)
	// -> (1,1) -> (1,1)
	output.TexCoord = Position.xy * 0.5f + 0.5f;

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content generation      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode_ContentGeneration = R"(
// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Return the color green
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content processing      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode_ContentProcessing = R"(
// Uniforms
uniform sampler2D ContentMap : register(s0);

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	// -> Apply a simple wobble to the texture coordinate so we can see that content processing is up and running
	// -> Unlike OpenGL or OpenGL ES 3, in Direct3D 9 & 10 & 11 the texture origin is left/top which does not map well to clip space coordinates
	// -> We have to flip the y-axis to map the coordinate system to the Direct3D 9 & 10 & 11 texture coordinate system
	// -> (-1,-1) -> (0,1)
	// -> (1,1) -> (1,0)
	return tex2D(ContentMap, float2(TexCoord.x + sin(TexCoord.x * 100.0f) * 0.01f, 1.0f - TexCoord.y - cos(TexCoord.y * 100.0f) * 0.01f));
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (mRhi->get_name_id() == rhi::NameId::DIRECT3D10 || mRhi->get_name_id() == rhi::NameId::DIRECT3D11 || mRhi->get_name_id() == rhi::NameId::DIRECT3D12)
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
	VS_OUTPUT output;

	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(Position, 0.5f, 1.0f);

	// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
	// -> Unlike OpenGL or OpenGL ES 3, in Direct3D 9 & 10 & 11 the texture origin is left/top which does not map well to clip space coordinates
	// -> We have to flip the y-axis to map the coordinate system to the Direct3D 9 & 10 & 11 texture coordinate system
	// -> In this example we do this within the fragment shader so have identical wobble across the different graphics APIs
	// -> (-1,-1) -> (0,0)
	// -> (1,1) -> (1,1)
	output.TexCoord = Position.xy * 0.5f + 0.5f;

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content generation      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode_ContentGeneration = R"(
// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Return the color green
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code - Content processing      ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode_ContentProcessing = R"(
// Uniforms
SamplerState SamplerLinear : register(s0);
Texture2D ContentMap : register(t0);

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	// -> Apply a simple wobble to the texture coordinate so we can see that content processing is up and running
	// -> Unlike OpenGL or OpenGL ES 3, in Direct3D 9 & 10 & 11 the texture origin is left/top which does not map well to clip space coordinates
	// -> We have to flip the y-axis to map the coordinate system to the Direct3D 9 & 10 & 11 texture coordinate system
	// -> (-1,-1) -> (0,1)
	// -> (1,1) -> (1,0)
	return ContentMap.Sample(SamplerLinear, float2(TexCoord.x + sin(TexCoord.x * 100.0f) * 0.01f, 1.0f - TexCoord.y - cos(TexCoord.y * 100.0f) * 0.01f));
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (mRhi->get_name_id() == rhi::NameId::NULL_DUMMY)
{
vertexShaderSourceCode = fragmentShaderSourceCode_ContentGeneration = fragmentShaderSourceCode_ContentProcessing = "42";
}
else
{
// Error! (unsupported RHI)
BE_LOG(Critical, "Error: Unsupported RHI")
}
