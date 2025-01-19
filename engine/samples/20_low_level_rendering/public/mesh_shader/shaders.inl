if (rhi->get_name_id() == rhi::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// Mesh shader
meshShaderSourceCode = R"(#version 450 // OpenGL 4.5

#extension GL_NV_mesh_shader : require

layout(local_size_x = 1) in;
layout(triangles, max_vertices = 3, max_primitives = 1) out;

out gl_MeshPerVertexNV
{
	vec4 gl_Position;
} gl_MeshVerticesNV[];

const vec3 vertices[3] = {vec3(0.0f, 1.0f, 0.5), vec3(1.0f, 0.0f, 0.5), vec3(-0.5f, 0.0f, 0.5)};

// From http://zone.dog/braindump/mesh_shaders/
// If we don't redeclare gl_PerVertex, compilation fails with the following error:
// error C7592: ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
out gl_PerVertex
{
	vec4 gl_Position;
} gl_Why;

void main()
{
	// Vertices position
 	gl_MeshVerticesNV[0].gl_Position = vec4(vertices[0], 1.0);
	gl_MeshVerticesNV[1].gl_Position = vec4(vertices[1], 1.0);
	gl_MeshVerticesNV[2].gl_Position = vec4(vertices[2], 1.0);

	// Triangle indices
	gl_PrimitiveIndicesNV[0] = 0;
	gl_PrimitiveIndicesNV[1] = 1;
	gl_PrimitiveIndicesNV[2] = 2;

	// Number of triangles
	gl_PrimitiveCountNV = 1;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(1.0, 1.0, 1.0, 1.0);
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
// Mesh shader
meshShaderSourceCode = R"(#version 450 // OpenGL 4.5

#extension GL_NV_mesh_shader : require

layout(local_size_x = 1) in;
layout(triangles, max_vertices = 3, max_primitives = 1) out;

out gl_MeshPerVertexNV
{
	vec4 gl_Position;
} gl_MeshVerticesNV[];

const vec3 vertices[3] = {vec3(0.0f, 1.0f, 0.5), vec3(1.0f, 0.0f, 0.5), vec3(-0.5f, 0.0f, 0.5)};

// From http://zone.dog/braindump/mesh_shaders/
// If we don't redeclare gl_PerVertex, compilation fails with the following error:
// error C7592: ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
/*
out gl_PerVertex
{
	vec4 gl_Position;
} gl_Why;
*/
void main()
{
	// Vertices position
 	gl_MeshVerticesNV[0].gl_Position = vec4(vertices[0], 1.0);
	gl_MeshVerticesNV[1].gl_Position = vec4(vertices[1], 1.0);
	gl_MeshVerticesNV[2].gl_Position = vec4(vertices[2], 1.0);

	// Triangle indices
	gl_PrimitiveIndicesNV[0] = 0;
	gl_PrimitiveIndicesNV[1] = 1;
	gl_PrimitiveIndicesNV[2] = 2;

	// Number of triangles
	gl_PrimitiveCountNV = 1;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi->get_name_id() == rhi::NameId::DIRECT3D12)
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
};

// Programs
VS_OUTPUT main(float2 Position : POSITION)	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = float4(Position, 0.5f, 1.0f);
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
