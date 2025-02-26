if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per control point of the patch
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 Position;	// Clip space control point position of the patch as input, left/bottom is (-1,-1) and right/top is (1,1)
out vec2 vPosition;	// Clip space control point position of the patch as output, left/bottom is (-1,-1) and right/top is (1,1)

// Programs
void main()
{
	// Pass through the clip space control point position of the patch, left/bottom is (-1,-1) and right/top is (1,1)
	vPosition = Position;
}
)";


//[-------------------------------------------------------]
//[ Tessellation control shader source code               ]
//[-------------------------------------------------------]
// One tessellation control shader invocation per patch control point (with super-vision)
tessellationControlShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 vPosition[];	// Clip space control point position of the patch we received from the vertex shader (VS) as input
out vec2 tcPosition[];	// Clip space control point position of the patch as output

// Programs
layout(vertices = 3) out;
void main()
{
	// Pass through the clip space control point position of the patch
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

	// If this is the first control point of the patch, inform the tessellator about the desired tessellation level
	if (0 == gl_InvocationID)
	{
		gl_TessLevelOuter[0] = 1.0;
		gl_TessLevelOuter[1] = 2.0;
		gl_TessLevelOuter[2] = 3.0;
		gl_TessLevelInner[0] = 4.0;
	}
}
)";


//[-------------------------------------------------------]
//[ Tessellation evaluation shader source code            ]
//[-------------------------------------------------------]
// One tessellation evaluation shader invocation per point from tessellator
tessellationEvaluationShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in vec2 tcPosition[];	// Clip space control point position of the patch we received from the tessellation control shader (TCS) as input
out gl_PerVertex
{
	vec4 gl_Position;
};

// Programs
layout(triangles, equal_spacing, ccw) in;
void main()
{
	// The barycentric coordinate "gl_TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex clip space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three clip  space control points of the patch
	vec2 p0 = gl_TessCoord.x * tcPosition[0];
	vec2 p1 = gl_TessCoord.y * tcPosition[1];
	vec2 p2 = gl_TessCoord.z * tcPosition[2];

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(p0 + p1 + p2, 0.5f, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
layout(location = 0, index = 0) out vec4 Color0;

// Programs
void main()
{
	// Return white
	Color0 = vec4(1.0, 1.0, 1.0, 1.0);
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
// One vertex shader invocation per control point of the patch
vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in vec2 Position;		// Clip space control point position of the patch as input, left/bottom is (-1,-1) and right/top is (1,1)
layout(location = 0) out vec2 vPosition;	// Clip space control point position of the patch as output, left/bottom is (-1,-1) and right/top is (1,1)

// Programs
void main()
{
	// Pass through the clip space control point position of the patch, left/bottom is (-1,-1) and right/top is (1,1)
	vPosition = Position;
}
)";


//[-------------------------------------------------------]
//[ Tessellation control shader source code               ]
//[-------------------------------------------------------]
// One tessellation control shader invocation per patch control point (with super-vision)
tessellationControlShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in vec2 vPosition[];	// Clip space control point position of the patch we received from the vertex shader (VS) as input
layout(location = 0) out vec2 tcPosition[];	// Clip space control point position of the patch as output

// Programs
layout(vertices = 3) out;
void main()
{
	// Pass through the clip space control point position of the patch
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

	// If this is the first control point of the patch, inform the tessellator about the desired tessellation level
	if (0 == gl_InvocationID)
	{
		gl_TessLevelOuter[0] = 1.0;
		gl_TessLevelOuter[1] = 2.0;
		gl_TessLevelOuter[2] = 3.0;
		gl_TessLevelInner[0] = 4.0;
	}
}
)";


//[-------------------------------------------------------]
//[ Tessellation evaluation shader source code            ]
//[-------------------------------------------------------]
// One tessellation evaluation shader invocation per point from tessellator
tessellationEvaluationShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in vec2 tcPosition[];	// Clip space control point position of the patch we received from the tessellation control shader (TCS) as input
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};

// Programs
layout(triangles, equal_spacing, ccw) in;
void main()
{
	// The barycentric coordinate "gl_TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex clip space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three clip  space control points of the patch
	// -> Compensate for different Vulkan coordinate system
	vec2 p0 = gl_TessCoord.x * tcPosition[2];
	vec2 p1 = gl_TessCoord.y * tcPosition[1];
	vec2 p2 = gl_TessCoord.z * tcPosition[0];

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(p0 + p1 + p2, 0.5f, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0, index = 0) out vec4 Color0;

// Programs
void main()
{
	// Return white
	Color0 = vec4(1.0, 1.0, 1.0, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D11 || rhi->get_name_id() == rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per control point of the patch
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float2 Position : POSITION;	// Clip space control point position of the patch as input, left/bottom is (-1,-1) and right/top is (1,1)
};

// Programs
VS_OUTPUT main(float2 Position : POSITION)
{
	// Pass through the clip space control point position of the patch, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = Position;
	return output;
}
)";


//[-------------------------------------------------------]
//[ Tessellation control shader source code               ]
//[-------------------------------------------------------]
// Under Direct3D 11, the tessellation control shader invocation is slit into per patch and per patch control point
// "hull shader" in Direct3D terminology
tessellationControlShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float2 Position : POSITION;	// Clip space control point position of the patch we received from the vertex shader (VS) as input
};
struct HS_CONSTANT_DATA_OUTPUT
{
	float TessLevelOuter[3] : SV_TESSFACTOR;		// Outer tessellation level
	float TessLevelInner[1] : SV_INSIDETESSFACTOR;	// Inner tessellation level
};
struct HS_OUTPUT
{
	float2 Position : POSITION;	// Clip space control point position of the patch as output
};

// Program invocation per patch
HS_CONSTANT_DATA_OUTPUT ConstantHS(InputPatch<VS_OUTPUT, 3> input)
{
	// Inform the tessellator about the desired tessellation level
	HS_CONSTANT_DATA_OUTPUT output;
	output.TessLevelOuter[0] = 1.0f;
	output.TessLevelOuter[1] = 2.0f;
	output.TessLevelOuter[2] = 3.0f;
	output.TessLevelInner[0] = 4.0f;
	return output;
}

// Program invocation per patch control point
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HS_OUTPUT main(InputPatch<VS_OUTPUT, 3> input, uint InvocationID : SV_OutputControlPointID)
{
	// Pass through the clip space control point position of the patch
	HS_OUTPUT output;
	output.Position = input[InvocationID].Position;
	return output;
}
)";


//[-------------------------------------------------------]
//[ Tessellation evaluation shader source code            ]
//[-------------------------------------------------------]
// One tessellation evaluation shader invocation per point from tessellator
// "domain shader" in Direct3D terminology
tessellationEvaluationShaderSourceCode = R"(
// Attribute input/output
struct HS_CONSTANT_DATA_OUTPUT
{
	float TessLevelOuter[3] : SV_TESSFACTOR;		// Outer tessellation level
	float TessLevelInner[1] : SV_INSIDETESSFACTOR;	// Inner tessellation level
};
struct HS_OUTPUT
{
	float2 Position : POSITION;	// Clip space control point position of the patch we received from the tessellation control shader (TCS) as input
};
struct DS_OUTPUT
{
	float4 Position : SV_POSITION;	// Interpolated clip space control point position inside the patch as output
};

// Programs
[domain("tri")]
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT inputTess, float3 TessCoord : SV_DOMAINLOCATION, const OutputPatch<HS_OUTPUT, 3> input)
{
	DS_OUTPUT output;

	// The barycentric coordinate "TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex clip space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three clip space control points of the patch
	float2 p0 = TessCoord.x * input[0].Position;
	float2 p1 = TessCoord.y * input[1].Position;
	float2 p2 = TessCoord.z * input[2].Position;
	output.Position = float4(p0 + p1 + p2, 0.5f, 1.0f);

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
float4 main(float4 Position : SV_POSITION) : SV_TARGET
{
	// Return white
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
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