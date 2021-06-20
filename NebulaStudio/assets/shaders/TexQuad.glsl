#type vertex
#version 450 core

layout(location = 0) in vec3 a_Translation;
layout(location = 1) in vec3 a_Rotation;
layout(location = 2) in vec3 a_Scale;
layout(location = 3) in vec4 a_QuadIndexPos;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TexCoord;
layout(location = 6) in float a_TexIndex;
layout(location = 7) in float a_TilingFactor;
layout(location = 8) in int a_EntityID;

layout (binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int f_EntityID;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

mat4 toScaleMatrix(vec3 scale) {
    return mat4(scale.x, 0,       0,       0,
                0,       scale.y, 0,       0,
                0,       0,       scale.z, 0,
                0,       0,       0,       1);
}

mat4 toTranslationMatrix(vec3 translation) {
    return mat4(1,              0,              0,              translation.x,
                0,              1,              0,              translation.y,
                0,              0,              1,              translation.z,
                0,              0,              0,              1);
}

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TexIndex = a_TexIndex;
	Output.TilingFactor = a_TilingFactor;
    f_EntityID = a_EntityID;
	mat4 scaleAsMat4 = toScaleMatrix(a_Scale);
    mat4 rotAsMat4 = rotationMatrix(vec3(1, 0, 0), a_Rotation.x) * rotationMatrix(vec3(0, 1, 0), a_Rotation.y) * rotationMatrix(vec3(0, 0, 1), a_Rotation.z);
    mat4 transAsMat4 = toTranslationMatrix(a_Translation);
    gl_Position = a_QuadIndexPos * scaleAsMat4 * rotAsMat4 * transAsMat4 * u_ViewProjection;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) flat in int f_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color; 
	color = texColor;
    if (texColor.w == 0.0) {
        o_EntityID = -1;
    }
    else {
        o_EntityID = f_EntityID;
    }
}