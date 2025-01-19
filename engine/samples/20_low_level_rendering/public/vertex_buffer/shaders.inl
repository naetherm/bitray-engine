if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
in  vec3 Color;		// RGB color as input
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 ColorVS;	// RGB color as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);

	// Pass through the color
	ColorVS = Color;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 ColorVS;		// RGB color as input
out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(ColorVS, 1.0);
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
layout(location = 0) in vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
layout(location = 1) in vec3 Color;		// RGB color as input
out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 0) out vec3 ColorVS;	// RGB color as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);

	// Pass through the color
	ColorVS = Color;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in  vec3 ColorVS;		// RGB color as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(ColorVS, 1.0);
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
in  highp vec3 Color;		// RGB color as input
out highp vec3 ColorVS;		// RGB color as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);

	// Pass through the color
	ColorVS = Color;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec3 ColorVS;		// RGB color as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(ColorVS, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D9 || rhi->get_name_id() == rhi::NameId::DIRECT3D10 || rhi->get_name_id() == rhi::NameId::DIRECT3D11 || rhi->get_name_id() == rhi::NameId::DIRECT3D12)
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
	float3 Color    : COLOR;		// RGB color as output
};

// Programs
VS_OUTPUT main(float2 Position : POSITION,	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
			   float3 Color    : COLOR)		// RGB color as input
{
	VS_OUTPUT output;

	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(Position, 0.5f, 1.0f);

	// Pass through the color
	output.Color = Color;

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode = R"(
// Programs
float4 main(float4 Position : SV_POSITION, float3 Color : COLOR) : SV_TARGET
{
	// Return white
	return float4(Color, 1.0f);
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
