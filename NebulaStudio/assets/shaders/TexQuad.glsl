// #type vertex
// #version 330 core

// layout(location = 0) in vec3 a_Translation;
// layout(location = 1) in vec3 a_Rotation;
// layout(location = 2) in vec3 a_Scale;
// layout(location = 3) in vec4 a_QuadIndexPos;
// layout(location = 4) in vec4 a_Color;
// layout(location = 5) in vec2 a_TexCoord;
// layout(location = 6) in float a_TexIndex;
// layout(location = 7) in float a_TilingFactor;
// layout(location = 8) in int a_EntityID;
// // layout(location = 8) in mat4 a_ModelMat;

// uniform mat4 u_ViewProjection;

// out vec4 v_Color;
// out vec2 v_TexCoord;
// out float v_TexIndex;
// out float v_TilingFactor;
// flat out int v_EntityID;

// mat4 rotationMatrix(vec3 axis, float angle)
// {
//     axis = normalize(axis);
//     float s = sin(angle);
//     float c = cos(angle);
//     float oc = 1.0 - c;
    
//     return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
//                 oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
//                 oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
//                 0.0,                                0.0,                                0.0,                                1.0);
// }

// mat4 toScaleMatrix(vec3 scale) {
//     return mat4(scale.x, 0,       0,       0,
//                 0,       scale.y, 0,       0,
//                 0,       0,       scale.z, 0,
//                 0,       0,       0,       1);
// }

// mat4 toTranslationMatrix(vec3 translation) {
//     return mat4(1,              0,              0,              translation.x,
//                 0,              1,              0,              translation.y,
//                 0,              0,              1,              translation.z,
//                 0,              0,              0,              1);
// }

// void main()
// {
//     v_Color = a_Color;
//     v_TexCoord = a_TexCoord;
//     v_TexIndex = a_TexIndex;
//     v_TilingFactor = a_TilingFactor;
//     v_EntityID = a_EntityID;
//     mat4 scaleAsMat4 = toScaleMatrix(a_Scale);
//     mat4 rotAsMat4 = rotationMatrix(vec3(1, 0, 0), a_Rotation.x) * rotationMatrix(vec3(0, 1, 0), a_Rotation.y) * rotationMatrix(vec3(0, 0, 1), a_Rotation.z);
//     mat4 transAsMat4 = toTranslationMatrix(a_Translation);
//     gl_Position = a_QuadIndexPos * scaleAsMat4 * rotAsMat4 * transAsMat4 * u_ViewProjection;
// }

// #type fragment
// #version 330 core

// layout(location = 0) out vec4 o_Color;
// layout(location = 1) out int o_EntityID;

// in vec4 v_Color;
// in vec2 v_TexCoord;
// in float v_TexIndex;
// in float v_TilingFactor;
// flat in int v_EntityID;

// uniform sampler2D u_Textures[32];

// void main()
// {
//     vec4 texColor = v_Color;
//     switch(int(v_TexIndex))
//     {
//         case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
//         case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
//         case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
//         case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
//         case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
//         case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
//         case 6: texColor *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;
//         case 7: texColor *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;
//         case 8: texColor *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;
//         case 9: texColor *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;
//         case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
//         case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
//         case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
//         case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
//         case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
//         case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
//         case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
//         case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
//         case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
//         case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
//         case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
//         case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
//         case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
//         case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
//         case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
//         case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
//         case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
//         case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
//         case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
//         case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
//         case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
//         case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
//     }
//     o_Color = texColor;
//     if (texColor.w == 0.0) {
//         o_EntityID = -1;
//     }
//     else {
//         o_EntityID = v_EntityID;
//     }
// }

// // Basic Texture Shader

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

layout (std140, binding = 0) uniform Camera
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