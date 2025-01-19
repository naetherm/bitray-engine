if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per control point of the patch
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 Position;	// Object space control point position of the patch we received from the input assembly (IA) as input
out vec3 vPosition;	// Object space control point position of the patch as output

// Programs
void main()
{
	// Pass through the object space control point position of the patch
	vPosition = Position;
}
)";


//[-------------------------------------------------------]
//[ Tessellation control shader source code               ]
//[-------------------------------------------------------]
// One tessellation control shader invocation per patch control point (with super-vision)
tessellationControlShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 vPosition[];	// Object space control point position of the patch we received from the vertex shader (VS) as input
out vec3 tcPosition[];	// Object space control point position of the patch as output

// Uniforms
layout(std140) uniform UniformBlockDynamicTcs
{
	float TessellationLevelOuter;	// Outer tessellation level
	float TessellationLevelInner;	// Inner tessellation level
};

// Programs
layout(vertices = 3) out;
void main()
{
	// Pass through the object space control point position of the patch
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

	// If this is the first control point of the patch, inform the tessellator about the desired tessellation level
	if (0 == gl_InvocationID)
	{
		gl_TessLevelOuter[0] = TessellationLevelOuter;
		gl_TessLevelOuter[1] = TessellationLevelOuter;
		gl_TessLevelOuter[2] = TessellationLevelOuter;
		gl_TessLevelInner[0] = TessellationLevelInner;
	}
}
)";


//[-------------------------------------------------------]
//[ Tessellation evaluation shader source code            ]
//[-------------------------------------------------------]
// One tessellation evaluation shader invocation per point from tessellator
tessellationEvaluationShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 tcPosition[];		// Object space control point position of the patch we received from the tessellation control shader (TCS) as input
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 tePosition;		// Interpolated object space vertex position inside the patch as output
out vec3 tePatchDistance;	// The barycentric coordinate inside the patch we received from the tessellator as output

// Uniforms
layout(std140) uniform UniformBlockStaticTes
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
layout(triangles, equal_spacing, ccw) in;
void main()
{
	// The barycentric coordinate "gl_TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex object space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three object space control points of the patch
	vec3 p0 = gl_TessCoord.x * tcPosition[0];
	vec3 p1 = gl_TessCoord.y * tcPosition[1];
	vec3 p2 = gl_TessCoord.z * tcPosition[2];
	tePosition = normalize(p0 + p1 + p2);

	// Pass through the barycentric coordinate inside the patch
	tePatchDistance = gl_TessCoord;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(tePosition, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Geometry shader source code                           ]
//[-------------------------------------------------------]
// One geometry shader invocation per primitive
geometryShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in gl_PerVertex
{
	vec4 gl_Position;
} gl_in[3];
in  vec3 tePosition[3];			// Interpolated object space vertex position inside the patch we received from the tessellation evaluation shader (TES) as input
in  vec3 tePatchDistance[3];	// The barycentric coordinate inside the patch from the tessellator we received from the tessellation evaluation shader (TES) as input
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 gFacetNormal;			// Normalized normal of the primitive as output
out vec3 gPatchDistance;		// The barycentric coordinate inside the patch from the tessellator as output
out vec3 gTriDistance;			// Local triangle vertex position as output

// Uniforms
layout(std140) uniform UniformBlockStaticGs
{
	// TODO(naetherm) mat3
	mat4 NormalMatrix;	// Object space to clip space rotation matrix
};

// Programs
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
void main()
{
	vec3 A = tePosition[2] - tePosition[0];
	vec3 B = tePosition[1] - tePosition[0];
	// TODO(naetherm) mat3
	gFacetNormal = (NormalMatrix * vec4(normalize(cross(A, B)), 1.0)).xyz;

	// Emit vertex 0
	gPatchDistance = tePatchDistance[0];
	gTriDistance = vec3(1.0, 0.0, 0.0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	// Emit vertex 1
	gPatchDistance = tePatchDistance[1];
	gTriDistance = vec3(0.0, 1.0, 0.0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	// Emit vertex 2
	gPatchDistance = tePatchDistance[2];
	gTriDistance = vec3(0.0, 0.0, 1.0);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attributes
in vec3 gFacetNormal;	// Normalized normal of the primitive we received from the geometry shader (GS) as input
in vec3 gPatchDistance;	// The barycentric coordinate inside the patch from the tessellator we received from the geometry shader (GS) as input
in vec3 gTriDistance;	// Local triangle vertex position we received from the geometry shader (GS) as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(std140) uniform UniformBlockStaticFs
{
	// TODO(naetherm) vec3
	vec4 LightPosition;
	vec4 DiffuseMaterial;
	vec4 AmbientMaterial;
};

// Programs
float Amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0.0, 1.0);
	d = 1.0 - exp2(-2.0 * d * d);
	return d;
}

void main()
{
	// Simple lighting
	vec3 N = normalize(gFacetNormal);
	vec3 L = LightPosition.xyz;// TODO(naetherm) vec3
	float df = abs(dot(N, L));
	vec3 color = AmbientMaterial.rgb + df * DiffuseMaterial.rgb;// TODO(naetherm) vec3

	// Add wireframe via fragment color manipulation
	// -> Thick (60) black lines for the patch edges
	// -> Thin (40) black lines for the edges of the generated triangles inside the patch
	// -> Determine how far the current fragment is from the nearest edge of the patch and
	//    local triangle by use the given interpolated positions inside the patch and local triangle
	float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
	float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
	color = Amplify(d1, 40.0, -0.5) * Amplify(d2, 60.0, -0.5) * color;

	// Return the calculated color
	OutputColor = vec4(color, 1.0);
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
layout(location = 0) in  vec3 Position;		// Object space control point position of the patch we received from the input assembly (IA) as input
layout(location = 0) out vec3 vPosition;	// Object space control point position of the patch as output

// Programs
void main()
{
	// Pass through the object space control point position of the patch
	vPosition = Position;
}
)";


//[-------------------------------------------------------]
//[ Tessellation control shader source code               ]
//[-------------------------------------------------------]
// One tessellation control shader invocation per patch control point (with super-vision)
tessellationControlShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in  vec3 vPosition[];	// Object space control point position of the patch we received from the vertex shader (VS) as input
layout(location = 0) out vec3 tcPosition[];	// Object space control point position of the patch as output

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBlockDynamicTcs
{
	float TessellationLevelOuter;	// Outer tessellation level
	float TessellationLevelInner;	// Inner tessellation level
};

// Programs
layout(vertices = 3) out;
void main()
{
	// Pass through the object space control point position of the patch
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

	// If this is the first control point of the patch, inform the tessellator about the desired tessellation level
	if (0 == gl_InvocationID)
	{
		gl_TessLevelOuter[0] = TessellationLevelOuter;
		gl_TessLevelOuter[1] = TessellationLevelOuter;
		gl_TessLevelOuter[2] = TessellationLevelOuter;
		gl_TessLevelInner[0] = TessellationLevelInner;
	}
}
)";


//[-------------------------------------------------------]
//[ Tessellation evaluation shader source code            ]
//[-------------------------------------------------------]
// One tessellation evaluation shader invocation per point from tessellator
tessellationEvaluationShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in vec3 tcPosition[];	// Object space control point position of the patch we received from the tessellation control shader (TCS) as input
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 tePosition;		// Interpolated object space vertex position inside the patch as output
layout(location = 2) out vec3 tePatchDistance;	// The barycentric coordinate inside the patch we received from the tessellator as output

// Uniforms
layout(std140, set = 1, binding = 0) uniform UniformBlockStaticTes
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
layout(triangles, equal_spacing, cw) in;
void main()
{
	// The barycentric coordinate "gl_TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex object space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three object space control points of the patch
	vec3 p0 = gl_TessCoord.x * tcPosition[0];
	vec3 p1 = gl_TessCoord.y * tcPosition[1];
	vec3 p2 = gl_TessCoord.z * tcPosition[2];
	tePosition = normalize(p0 + p1 + p2);

	// Pass through the barycentric coordinate inside the patch
	tePatchDistance = gl_TessCoord;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(tePosition, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Geometry shader source code                           ]
//[-------------------------------------------------------]
// One geometry shader invocation per primitive
geometryShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in gl_PerVertex
{
	vec4 gl_Position;
} gl_in[3];
layout(location = 1) in vec3 tePosition[3];			// Interpolated object space vertex position inside the patch we received from the tessellation evaluation shader (TES) as input
layout(location = 2) in vec3 tePatchDistance[3];	// The barycentric coordinate inside the patch from the tessellator we received from the tessellation evaluation shader (TES) as input
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 gFacetNormal;			// Normalized normal of the primitive as output
layout(location = 2) out vec3 gPatchDistance;		// The barycentric coordinate inside the patch from the tessellator as output
layout(location = 3) out vec3 gTriDistance;			// Local triangle vertex position as output

// Uniforms
layout(std140, set = 2, binding = 0) uniform UniformBlockStaticGs
{
	// TODO(naetherm) mat3
	mat4 NormalMatrix;	// Object space to clip space rotation matrix
};

// Programs
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
void main()
{
	vec3 A = tePosition[2] - tePosition[0];
	vec3 B = tePosition[1] - tePosition[0];
	// TODO(naetherm) mat3
	gFacetNormal = (NormalMatrix * vec4(normalize(cross(A, B)), 1.0)).xyz;

	// Emit vertex 0
	gPatchDistance = tePatchDistance[0];
	gTriDistance = vec3(1.0, 0.0, 0.0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	// Emit vertex 1
	gPatchDistance = tePatchDistance[1];
	gTriDistance = vec3(0.0, 1.0, 0.0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	// Emit vertex 2
	gPatchDistance = tePatchDistance[2];
	gTriDistance = vec3(0.0, 0.0, 1.0);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attributes
layout(location = 1) in  vec3 gFacetNormal;		// Normalized normal of the primitive we received from the geometry shader (GS) as input
layout(location = 2) in  vec3 gPatchDistance;	// The barycentric coordinate inside the patch from the tessellator we received from the geometry shader (GS) as input
layout(location = 3) in  vec3 gTriDistance;		// Local triangle vertex position we received from the geometry shader (GS) as input
layout(location = 0) out vec4 OutputColor;		// Output variable for fragment color

// Uniforms
layout(std140, set = 3, binding = 0) uniform UniformBlockStaticFs
{
	// TODO(naetherm) vec3
	vec4 LightPosition;
	vec4 DiffuseMaterial;
	vec4 AmbientMaterial;
};

// Programs
float Amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0.0, 1.0);
	d = 1.0 - exp2(-2.0 * d * d);
	return d;
}

void main()
{
	// Simple lighting
	vec3 N = normalize(gFacetNormal);
	vec3 L = LightPosition.xyz;// TODO(naetherm) vec3
	float df = abs(dot(N, L));
	vec3 color = AmbientMaterial.rgb + df * DiffuseMaterial.rgb;// TODO(naetherm) vec3

	// Add wireframe via fragment color manipulation
	// -> Thick (60) black lines for the patch edges
	// -> Thin (40) black lines for the edges of the generated triangles inside the patch
	// -> Determine how far the current fragment is from the nearest edge of the patch and
	//    local triangle by use the given interpolated positions inside the patch and local triangle
	float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
	float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
	color = Amplify(d1, 40.0, -0.5) * Amplify(d2, 60.0, -0.5) * color;

	// Return the calculated color
	OutputColor = vec4(color, 1.0);
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
	float3 Position : POSITION;	// Object space control point position of the patch as output
};

// Programs
VS_OUTPUT main(float3 Position : POSITION)	// Object space control point position of the patch we received from the input assembly (IA) as input
{
	// Pass through the object space control point position of the patch
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
	float3 Position : POSITION;	// Object space control point position of the patch we received from the vertex shader (VS) as input
};
struct HS_CONSTANT_DATA_OUTPUT
{
	float TessLevelInner[1] : SV_INSIDETESSFACTOR;	// Inner tessellation level
	float TessLevelOuter[3] : SV_TESSFACTOR;		// Outer tessellation level
};
struct HS_OUTPUT
{
	float3 Position : POSITION;	// Object space control point position of the patch as output
};

// Uniforms
cbuffer UniformBlockDynamicTcs : register(b0)
{
	float TessellationLevelOuter;	// Outer tessellation level
	float TessellationLevelInner;	// Inner tessellation level
}

// Program invocation per patch
HS_CONSTANT_DATA_OUTPUT ConstantHS(InputPatch<VS_OUTPUT, 3> input, uint PatchID : SV_PRIMITIVEID)
{
	// Inform the tessellator about the desired tessellation level
	HS_CONSTANT_DATA_OUTPUT output;
	output.TessLevelOuter[0] = TessellationLevelOuter;
	output.TessLevelOuter[1] = TessellationLevelOuter;
	output.TessLevelOuter[2] = TessellationLevelOuter;
	output.TessLevelInner[0] = TessellationLevelInner;
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
	// Pass through the object space control point position of the patch
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
	float TessLevelInner[1] : SV_INSIDETESSFACTOR;	// Inner tessellation level
	float TessLevelOuter[3] : SV_TESSFACTOR;		// Outer tessellation level
};
struct HS_OUTPUT
{
	float3 Position : POSITION;	// Object space control point position of the patch we received from the tessellation control shader (TCS) as input
};
struct DS_OUTPUT
{
	float3 Position			 : POSITION;	// Interpolated object space vertex position inside the patch as output
	float3 PatchDistance	 : TEXCOORD0;	// The barycentric coordinate inside the patch we received from the tessellator as output
	float4 ClipSpacePosition : TEXCOORD1;	// Clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1) as output
};

// Uniforms
cbuffer UniformBlockStaticTes : register(b0)
{
	float4x4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
}

// Programs
[domain("tri")]
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT inputTess, const OutputPatch<HS_OUTPUT, 3> input, float3 TessCoord : SV_DOMAINLOCATION)
{
	DS_OUTPUT output;

	// The barycentric coordinate "TessCoord" we received from the tessellator defines a location
	// inside a triangle as a combination of the weight of the three control points of the patch

	// Calculate the vertex object space position inside the patch by using the barycentric coordinate
	// we received from the tessellator and the three object space control points of the patch
	float3 p0 = TessCoord.x * input[0].Position;
	float3 p1 = TessCoord.y * input[1].Position;
	float3 p2 = TessCoord.z * input[2].Position;
	output.Position = normalize(p0 + p1 + p2);

	// Pass through the barycentric coordinate inside the patch
	output.PatchDistance = TessCoord;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.ClipSpacePosition = mul(ObjectSpaceToClipSpaceMatrix, float4(output.Position, 1.0));

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Geometry shader source code                           ]
//[-------------------------------------------------------]
// One geometry shader invocation per primitive
geometryShaderSourceCode = R"(
// Attribute input/output
struct DS_OUTPUT
{
	float3 Position			 : POSITION;	// Interpolated object space vertex position inside the patch we received from the tessellation evaluation shader (TES) as input
	float3 PatchDistance	 : TEXCOORD0;	// The barycentric coordinate inside the patch from the tessellator we received from the tessellation evaluation shader (TES) as input
	float4 ClipSpacePosition : TEXCOORD1;	// Clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1) we received from the tessellation evaluation shader (TES) as input
};
struct GS_OUTPUT
{
	float4 Position      : SV_POSITION;	// The clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1), as output
	float3 FacetNormal   : TEXCOORD0;	// Normalized normal of the primitive as output
	float3 PatchDistance : TEXCOORD1;	// The barycentric coordinate inside the patch from the tessellator as output
	float3 TriDistance   : TEXCOORD2;	// Triangle vertex as output
};

// Uniforms
cbuffer UniformBlockStaticGs : register(b0)
{
	// TODO(naetherm) float3x3
	float4x4 NormalMatrix;	// Object space to clip space rotation matrix
}

[maxvertexcount(3)]
void main(triangle DS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> OutputStream)
{
	GS_OUTPUT output;

	float3 A = input[2].Position - input[0].Position;
	float3 B = input[1].Position - input[0].Position;
	// TODO(naetherm) float3x3
	output.FacetNormal = mul(NormalMatrix, float4(normalize(cross(A, B)), 1.0f)).xyz;

	// Emit vertex 0
	output.PatchDistance = input[0].PatchDistance;
	output.TriDistance = float3(1.0f, 0.0f, 0.0f);
	output.Position = input[0].ClipSpacePosition;
	OutputStream.Append(output);

	// Emit vertex 1
	output.PatchDistance = input[1].PatchDistance;
	output.TriDistance = float3(0.0f, 1.0f, 0.0f);
	output.Position = input[1].ClipSpacePosition;
	OutputStream.Append(output);

	// Emit vertex 2
	output.PatchDistance = input[2].PatchDistance;
	output.TriDistance = float3(0.0f, 0.0f, 1.0f);
	output.Position = input[2].ClipSpacePosition;
	OutputStream.Append(output);

	// Done
	OutputStream.RestartStrip();
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode = R"(
// Attribute input/output
struct GS_OUTPUT
{
	float4 Position      : SV_POSITION;	// The clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1), we received from the geometry shader (GS) as input
	float3 FacetNormal   : TEXCOORD0;	// Normalized normal of the primitive we received from the geometry shader (GS) as input
	float3 PatchDistance : TEXCOORD1;	// The barycentric coordinate inside the patch from the tessellator we received from the geometry shader (GS) as input
	float3 TriDistance   : TEXCOORD2;	// Triangle vertex we received from the geometry shader (GS) as input
};

// Uniforms
cbuffer UniformBlockStaticFs : register(b0)
{
	// TODO(naetherm) float3
	float4 LightPosition;
	float4 DiffuseMaterial;
	float4 AmbientMaterial;
}

// Programs
float Amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0.0f, 1.0f);
	d = 1.0f - exp2(-2.0f * d * d);
	return d;
}

float4 main(GS_OUTPUT input) : SV_TARGET
{
	// Simple lighting
	float3 N = normalize(input.FacetNormal);
	float3 L = LightPosition.xyz;// TODO(naetherm) float3
	float df = abs(dot(N, L));
	float3 color = AmbientMaterial.rgb + df * DiffuseMaterial.rgb;// TODO(naetherm) float3

	// Add wireframe via fragment color manipulation
	// -> Thick (60) black lines for the patch edges
	// -> Thin (40) black lines for the edges of the generated triangles inside the patch
	// -> Determine how far the current fragment is from the nearest edge of the patch and
	//    local triangle by use the given interpolated positions inside the patch and local triangle
	float d1 = min(min(input.TriDistance.x, input.TriDistance.y), input.TriDistance.z);
	float d2 = min(min(input.PatchDistance.x, input.PatchDistance.y), input.PatchDistance.z);
	color = Amplify(d1, 40.0f, -0.5f) * Amplify(d2, 60.0f, -0.5f) * color;

	// Return the calculated color
	return float4(color, 1.0f);
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
