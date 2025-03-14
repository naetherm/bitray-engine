if (rhi.get_name_id() == rhi::NameId::OPENGL && 0 == mRhi->get_capabilities().maximumUniformBufferSize)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 130	// OpenGL 3.0

// Attribute input - Mesh data
in vec3 Position;	// Object space vertex position input
in vec2 TexCoord;
in vec3 Normal;

// Attribute input - Per-instance data
in vec4 PerInstancePositionTexture;	// Position: xyz=Position, w=Slice of the 2D texture array to use
in vec4 PerInstanceRotationScale;	// Rotation: Rotation quaternion (xyz) and scale (w)
									// -> We don't need to store the w component of the quaternion. It's normalized and storing
									//    three components while recomputing the fourths component is be sufficient.

// Attribute output
out vec3 WorldPositionVs;
out vec2 TexCoordVs;
out vec3 NormalVs;

// Uniforms
uniform mat4 MVP;
uniform vec2 TimerAndGlobalScale;	// x=Timer, y=Global scale

// Programs
void main()
{
	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0 - PerInstanceRotationScale.x * PerInstanceRotationScale.x
					- PerInstanceRotationScale.y * PerInstanceRotationScale.y
					- PerInstanceRotationScale.z * PerInstanceRotationScale.z;
	vec4 r = vec4(PerInstanceRotationScale.xyz, (sqw > 0.0) ? -sqrt(sqw) : 0.0);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		vec4 from = vec4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		vec4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		vec4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (slerp)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	// start with the local space vertex position
	vec4 position = vec4(Position, 1.0);

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = vec3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001);
	position.y += sin(TimerAndGlobalScale.x * 0.0001) * 2.0;
	position.z += cos(TimerAndGlobalScale.x * 0.0001) * 0.5;

	// Apply per instance position
	position.xyz += PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	WorldPositionVs = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = MVP * position;

	// Write out the final vertex data
	gl_Position = position;
	TexCoordVs = vec2(TexCoord.x, TexCoord.y / 8.0 + 1.0 / 8.0 * PerInstancePositionTexture.w);	// Fixed build in number of textures
	NormalVs = Normal;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 130	// OpenGL 3.0

// Attribute input/output
in vec3 WorldPositionVs;
in vec2 TexCoordVs;
in vec3 NormalVs;

// Uniforms
uniform sampler2D AlbedoMap;		// Usage of 'layout(binding = 1)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
uniform vec3      LightPosition;	// World space light position

// Programs
void main()
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(NormalVs, normalize(LightPosition - WorldPositionVs)), 0.0, 0.8);

	// Calculate the final fragment color
	gl_FragColor = (vec4(0.2, 0.2, 0.2, 1.0) + lighting) * texture2D(AlbedoMap, TexCoordVs);
	gl_FragColor.a = 0.8;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi.get_name_id() == rhi::NameId::OPENGL && mRhi->get_capabilities().maximumUniformBufferSize > 0)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 140	// OpenGL 3.1

// Attribute input - Mesh data
in vec3 Position;	// Object space vertex position input
in vec2 TexCoord;
in vec3 Normal;

// Attribute input - Per-instance data
in vec4 PerInstancePositionTexture;	// Position: xyz=Position, w=Slice of the 2D texture array to use
in vec4 PerInstanceRotationScale;	// Rotation: Rotation quaternion (xyz) and scale (w)
									// -> We don't need to store the w component of the quaternion. It's normalized and storing
									//    three components while recomputing the fourths component is be sufficient.

// Attribute output
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 WorldPositionVs;
out vec2 TexCoordVs;
out vec3 NormalVs;

// Uniforms
layout(std140) uniform UniformBlockStaticVs		// Usage of 'layout(binding = 0)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	mat4 MVP;
};
layout(std140) uniform UniformBlockDynamicVs	// Usage of 'layout(binding = 1)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	vec2 TimerAndGlobalScale;	// x=Timer, y=Global scale
};

// Programs
void main()
{
	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0 - PerInstanceRotationScale.x * PerInstanceRotationScale.x
					- PerInstanceRotationScale.y * PerInstanceRotationScale.y
					- PerInstanceRotationScale.z * PerInstanceRotationScale.z;
	vec4 r = vec4(PerInstanceRotationScale.xyz, (sqw > 0.0) ? -sqrt(sqw) : 0.0);

	// start with the local space vertex position
	vec4 position = vec4(Position, 1.0);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		vec4 from = vec4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		vec4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		vec4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (SLERP)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = vec3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001);
	position.y += sin(TimerAndGlobalScale.x * 0.0001) * 2.0;
	position.z += cos(TimerAndGlobalScale.x * 0.0001) * 0.5;

	// Apply per instance position
	position.xyz += PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	WorldPositionVs = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = MVP * position;

	// Write out the final vertex data
	gl_Position = position;
	TexCoordVs = vec2(TexCoord.x, TexCoord.y / 8.0 + 1.0 / 8.0 * PerInstancePositionTexture.w);	// Fixed build in number of textures
	NormalVs = Normal;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 140	// OpenGL 3.1
#extension GL_ARB_explicit_attrib_location : enable	// Required for 'layout(location = 0)' etc.

// Attribute input/output
in vec3 WorldPositionVs;
in vec2 TexCoordVs;
in vec3 NormalVs;
layout(location = 0, index = 0) out vec4 Color0;

// Uniforms
uniform sampler2D AlbedoMap;					// Usage of 'layout(binding = 1)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
layout(std140) uniform UniformBlockDynamicFs	// Usage of 'layout(binding = 0)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	vec3 LightPosition;	// World space light position
};

// Programs
void main()
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(NormalVs, normalize(LightPosition - WorldPositionVs)), 0.0, 0.8);

	// Calculate the final fragment color
	Color0 = (vec4(0.2, 0.2, 0.2, 1.0) + lighting) * texture(AlbedoMap, TexCoordVs);
	Color0.a = 0.8;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi.get_name_id() == rhi::NameId::VULKAN)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input - Mesh data
layout(location = 0) in vec3 Position;	// Object space vertex position input
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Attribute input - Per-instance data
layout(location = 3) in vec4 PerInstancePositionTexture;	// Position: xyz=Position, w=Slice of the 2D texture array to use
layout(location = 4) in vec4 PerInstanceRotationScale;		// Rotation: Rotation quaternion (xyz) and scale (w)
															// -> We don't need to store the w component of the quaternion. It's normalized and storing
															//    three components while recomputing the fourths component is be sufficient.

// Attribute output
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 WorldPositionVs;
layout(location = 2) out vec2 TexCoordVs;
layout(location = 3) out vec3 NormalVs;

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBlockStaticVs
{
	mat4 MVP;
};
layout(std140, set = 0, binding = 1) uniform UniformBlockDynamicVs
{
	vec2 TimerAndGlobalScale;	// x=Timer, y=Global scale
};

// Programs
void main()
{
	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0 - PerInstanceRotationScale.x * PerInstanceRotationScale.x
					- PerInstanceRotationScale.y * PerInstanceRotationScale.y
					- PerInstanceRotationScale.z * PerInstanceRotationScale.z;
	vec4 r = vec4(PerInstanceRotationScale.xyz, (sqw > 0.0) ? -sqrt(sqw) : 0.0);

	// start with the local space vertex position
	vec4 position = vec4(Position, 1.0);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		vec4 from = vec4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		vec4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		vec4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (SLERP)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = vec3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001);
	position.y += sin(TimerAndGlobalScale.x * 0.0001) * 2.0;
	position.z += cos(TimerAndGlobalScale.x * 0.0001) * 0.5;

	// Apply per instance position
	position.xyz += PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	WorldPositionVs = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = MVP * position;

	// Write out the final vertex data
	gl_Position = position;
	TexCoordVs = vec2(TexCoord.x, TexCoord.y / 8.0 + 1.0 / 8.0 * PerInstancePositionTexture.w);	// Fixed build in number of textures
	NormalVs = Normal;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 1) in vec3 WorldPositionVs;
layout(location = 2) in vec2 TexCoordVs;
layout(location = 3) in vec3 NormalVs;
layout(location = 0, index = 0) out vec4 Color0;

// Uniforms
layout(set = 1, binding = 0) uniform sampler2D AlbedoMap;
layout(std140, set = 1, binding = 1) uniform UniformBlockDynamicFs
{
	vec3 LightPosition;	// World space light position
};

// Programs
void main()
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(NormalVs, normalize(LightPosition - WorldPositionVs)), 0.0, 0.8);

	// Calculate the final fragment color
	Color0 = (vec4(0.2, 0.2, 0.2, 1.0) + lighting) * texture(AlbedoMap, TexCoordVs);
	Color0.a = 0.8;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi.get_name_id() == rhi::NameId::OPENGLES3 && mRhi->get_capabilities().maximumUniformBufferSize > 0)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input - Mesh data
in highp vec3 Position;	// Object space vertex position input
in highp vec2 TexCoord;
in highp vec3 Normal;

// Attribute input - Per-instance data
in highp vec4 PerInstancePositionTexture;	// Position: xyz=Position, w=Slice of the 2D texture array to use
in highp vec4 PerInstanceRotationScale;		// Rotation: Rotation quaternion (xyz) and scale (w)
											// -> We don't need to store the w component of the quaternion. It's normalized and storing
											//    three components while recomputing the fourths component is be sufficient.

// Attribute output
out highp vec3 WorldPositionVs;
out highp vec2 TexCoordVs;
out highp vec3 NormalVs;

// Uniforms
layout(std140) uniform UniformBlockStaticVs		// Usage of 'layout(binding = 0)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	mat4 MVP;
};
layout(std140) uniform UniformBlockDynamicVs	// Usage of 'layout(binding = 1)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	vec2 TimerAndGlobalScale;	// x=Timer, y=Global scale
};

// Programs
void main()
{
	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0 - PerInstanceRotationScale.x * PerInstanceRotationScale.x
					- PerInstanceRotationScale.y * PerInstanceRotationScale.y
					- PerInstanceRotationScale.z * PerInstanceRotationScale.z;
	vec4 r = vec4(PerInstanceRotationScale.xyz, (sqw > 0.0) ? -sqrt(sqw) : 0.0);

	// Start with the local space vertex position
	vec4 position = vec4(Position, 1.0);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		vec4 from = vec4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		vec4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		vec4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (SLERP)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = vec3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001);
	position.y += sin(TimerAndGlobalScale.x * 0.0001) * 2.0;
	position.z += cos(TimerAndGlobalScale.x * 0.0001) * 0.5;

	// Apply per instance position
	position.xyz += PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	WorldPositionVs = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = MVP * position;

	// Write out the final vertex data
	gl_Position = position;
	TexCoordVs = vec2(TexCoord.x, TexCoord.y / 8.0 + 1.0 / 8.0 * PerInstancePositionTexture.w);	// Fixed build in number of textures
	NormalVs = Normal;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0
precision highp float;

// Attribute input/output
in vec3 WorldPositionVs;
in vec2 TexCoordVs;
in vec3 NormalVs;
layout(location = 0) out vec4 Color0;	// Index layout needs 'EXT_blend_func_extended' which is OpenGL ES 3.1+ feature but not supported in fragment shader

// Uniforms
uniform highp sampler2D AlbedoMap;				// Usage of 'layout(binding = 1)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
layout(std140) uniform UniformBlockDynamicFs	// Usage of 'layout(binding = 0)' would be nice, but requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
{
	vec3 LightPosition;	// World space light position
};

// Programs
void main()
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(NormalVs, normalize(LightPosition - WorldPositionVs)), 0.0, 0.8);

	// Calculate the final fragment color
	Color0 = (vec4(0.2, 0.2, 0.2, 1.0) + lighting) * texture(AlbedoMap, TexCoordVs);
	Color0.a = 0.8;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi.get_name_id() == rhi::NameId::DIRECT3D9)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_INPUT
{
	// Mesh data
	float3 Position : POSITION;	// Object space vertex position
	float2 TexCoord : TEXCOORD0;
	float3 Normal   : NORMAL;
	// Per-instance data
	float4 PerInstancePositionTexture : TEXCOORD1;	// Position: xyz=Position, w=Slice of the 2D texture array to use
	float4 PerInstanceRotationScale   : TEXCOORD2;	// Rotation: Rotation quaternion (xyz) and scale (w)
													// -> We don't need to store the w component of the quaternion. It's normalized and storing
													//    three components while recomputing the fourths component is be sufficient.
};
struct VS_OUTPUT
{
	float4 Position      : SV_POSITION;
	float2 TexCoord      : TEXCOORD0;
	float3 Normal        : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
};

// Uniforms
uniform float4x4 MVP;
uniform float2 TimerAndGlobalScale;	// x=Timer, y=Global scale

// Programs
VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT output;

	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0f - Input.PerInstanceRotationScale.x * Input.PerInstanceRotationScale.x
					 - Input.PerInstanceRotationScale.y * Input.PerInstanceRotationScale.y
					 - Input.PerInstanceRotationScale.z * Input.PerInstanceRotationScale.z;
	float4 r = float4(Input.PerInstanceRotationScale.xyz, (sqw > 0.0f) ? -sqrt(sqw) : 0.0f);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		float4 from = float4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		float4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		float4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (SLERP)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	// Start with the local space vertex position
	float4 position = float4(Input.Position, 1.0f);

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = float3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							  position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							  position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * Input.PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001f);
	position.y += sin(TimerAndGlobalScale.x * 0.0001f) * 2.0f;
	position.z += cos(TimerAndGlobalScale.x * 0.0001f) * 0.5f;

	// Apply per instance position
	position.xyz += Input.PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	output.WorldPosition = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = mul(position, MVP);

	// Write out the final vertex data
	output.Position = position;
	output.TexCoord = float2(Input.TexCoord.x, Input.TexCoord.y / 8.0f + 1.0f / 8.0f * Input.PerInstancePositionTexture.w);	// Fixed build in number of textures
	output.Normal = Input.Normal;

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
// Attribute input
struct VS_OUTPUT
{
	float4 Position      : SV_POSITION;
	float2 TexCoord      : TEXCOORD0;
	float3 Normal        : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
};

// Uniforms
uniform sampler2D AlbedoMap : register(s0);
uniform float3 LightPosition;	// World space light position

// Programs
float4 main(VS_OUTPUT Input) : SV_TARGET
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(Input.Normal, normalize(LightPosition - Input.WorldPosition)), 0.0f, 0.8f);

	// Calculate the final fragment color
	// -> Direct3D 9 is using BGR, the world is using RGB
	float4 color = (float4(0.2f, 0.2f, 0.2f, 1.0f) + lighting) * tex2D(AlbedoMap, Input.TexCoord).bgra;
	color.a = 0.8f;

	// Done
	return color;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (rhi.get_name_id() == rhi::NameId::DIRECT3D10 || rhi.get_name_id() == rhi::NameId::DIRECT3D11 || rhi.get_name_id() == rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_INPUT
{
	// Mesh data
	float3 Position : POSITION;	// Object space vertex position
	float2 TexCoord : TEXCOORD0;
	float3 Normal   : NORMAL;
	// Per-instance data
	float4 PerInstancePositionTexture : TEXCOORD1;	// Position: xyz=Position, w=Slice of the 2D texture array to use
	float4 PerInstanceRotationScale   : TEXCOORD2;	// Rotation: Rotation quaternion (xyz) and scale (w)
													// -> We don't need to store the w component of the quaternion. It's normalized and storing
													//    three components while recomputing the fourths component is be sufficient.
};
struct VS_OUTPUT
{
	float4 Position      : SV_POSITION;
	float2 TexCoord      : TEXCOORD0;
	float3 Normal        : NORMAL;
	float3 WorldPosition : TEXCOORD1;
};

// Uniforms
cbuffer UniformBlockStaticVs : register(b0)
{
	float4x4 MVP;
};
cbuffer UniformBlockDynamicVs : register(b1)
{
	float2 TimerAndGlobalScale;	// x=Timer, y=Global scale
};

// Programs
VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT output;

	// Compute last component (w) of the quaternion (rotation quaternions are always normalized)
	float sqw = 1.0f - Input.PerInstanceRotationScale.x * Input.PerInstanceRotationScale.x
					 - Input.PerInstanceRotationScale.y * Input.PerInstanceRotationScale.y
					 - Input.PerInstanceRotationScale.z * Input.PerInstanceRotationScale.z;
	float4 r = float4(Input.PerInstanceRotationScale.xyz, (sqw > 0.0f) ? -sqrt(sqw) : 0.0f);

	{ // Cube rotation: SLERP from identity quaternion to rotation quaternion of the current instance
		// From
		float4 from = float4(0.0, 0.0, 0.0, 1.0);	// Identity

		// To
		float4 to = r;

		// Time
		float time = TimerAndGlobalScale.x * 0.001f;

		// Calculate cosine
		float cosom = dot(from, to);

		// Adjust signs (if necessary)
		float4 to1;
		if (cosom < 0.0f)
		{
			cosom  = -cosom;
			to1 = -to;
		}
		else
		{
			to1 = to;
		}

		// Calculate coefficients
		float scale0;
		float scale1;
		if ((1.0f - cosom) > 0.000001f)
		{
			// Standard case (SLERP)
			float omega = acos(cosom);
			float sinom = sin(omega);
			scale0 = sin((1.0f - time) * omega) / sinom;
			scale1 = sin(time * omega) / sinom;
		}
		else
		{
			// "from" and "to" quaternions are very close
			//  ... so we can do a linear interpolation:
			scale0 = 1.0f - time;
			scale1 = time;
		}

		// Calculate final values
		r = scale0 * from + scale1 * to1;
	}

	// Start with the local space vertex position
	float4 position = float4(Input.Position, 1.0f);

	{ // Apply rotation by using the rotation quaternion
		float x2 = r.x * r.x;
		float y2 = r.y * r.y;
		float z2 = r.z * r.z;
		float w2 = r.w * r.w;
		float xa = r.x * position.x;
		float yb = r.y * position.y;
		float zc = r.z * position.z;
		position.xyz = float3(position.x * ( x2 - y2 - z2 + w2) + 2.0 * (r.w * (r.y * position.z - r.z * position.y) + r.x * (yb + zc)),
							  position.y * (-x2 + y2 - z2 + w2) + 2.0 * (r.w * (r.z * position.x - r.x * position.z) + r.y * (xa + zc)),
							  position.z * (-x2 - y2 + z2 + w2) + 2.0 * (r.w * (r.x * position.y - r.y * position.x) + r.z * (xa + yb)));
	}

	// Apply global scale and per instance scale
	position.xyz = position.xyz * TimerAndGlobalScale.y * Input.PerInstanceRotationScale.w;

	// Some movement in general
	position.x += sin(TimerAndGlobalScale.x * 0.0001f);
	position.y += sin(TimerAndGlobalScale.x * 0.0001f) * 2.0f;
	position.z += cos(TimerAndGlobalScale.x * 0.0001f) * 0.5f;

	// Apply per instance position
	position.xyz += Input.PerInstancePositionTexture.xyz;

	// Calculate the world position of the vertex
	output.WorldPosition = position.xyz;

	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	position = mul(MVP, position);

	// Write out the final vertex data
	output.Position = position;
	output.TexCoord = float2(Input.TexCoord.x, Input.TexCoord.y / 8.0f + 1.0f / 8.0f * Input.PerInstancePositionTexture.w);	// Fixed build in number of textures
	output.Normal = Input.Normal;

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
// Attribute input
struct VS_OUTPUT
{
	float4 Position      : SV_POSITION;
	float2 TexCoord      : TEXCOORD0;
	float3 Normal        : NORMAL;
	float3 WorldPosition : TEXCOORD1;
};

// Uniforms
SamplerState SamplerLinear : register(s0);
Texture2D AlbedoMap : register(t0);
cbuffer UniformBlockDynamicFs : register(b0)
{
	float3 LightPosition;	// World space light position
};

// Programs
float4 main(VS_OUTPUT Input) : SV_TARGET
{
	// Simple point light by using Lambert's cosine law
	float lighting = clamp(dot(Input.Normal, normalize(LightPosition - Input.WorldPosition)), 0.0f, 0.8f);

	// Calculate the final fragment color
	float4 color = (float4(0.2f, 0.2f, 0.2f, 1.0f) + lighting) * AlbedoMap.Sample(SamplerLinear, Input.TexCoord);
	color.a = 0.8f;

	// Done
	return color;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else if (mRhi->get_name_id() == rhi::NameId::NULL_DUMMY)
{
vertexShaderSourceCode = fragmentShaderSourceCode = "42";
}
else
{
// Error! (unsupported RHI)
BE_LOG(Critical, "Error: Unsupported RHI")
}
