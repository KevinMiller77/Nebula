// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout (binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout (location = 0) out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 1.0) * u_ViewProjection;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int o_EntityID;

layout (location = 0) in vec4 v_Color;

void main()
{
	color = v_Color;
	o_EntityID = -1;
}
