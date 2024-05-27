#include <metal_stdlib>
using namespace metal;

struct VertexOutput
{
    float4 Position     [[position]];
    float4 Color        [[shared]];
    float2 TexCoord     [[shared]];
    float  TexIndex     [[shared]];
    float  TilingFactor [[shared]];
    int    EntityID     [[shared]];
};

struct Vertex
{
    float3 Translation  [[attribute(0)]];
    float3 Rotation     [[attribute(1)]];
    float3 Scale        [[attribute(2)]];
    float4 QuadIndexPos [[attribute(3)]];
    float4 Color        [[attribute(4)]];
    float2 TexCoord     [[attribute(5)]];
    float TexIndex      [[attribute(6)]];
    float TilingFactor  [[attribute(7)]];
    int EntityID        [[attribute(8)]];
};


float4x4 rotationMatrix(float3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return float4x4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                    0.0,                                0.0,                                0.0,                                1.0);
}

float4x4 toScaleMatrix(float3 scale) {
    return float4x4(scale.x, 0,       0,       0,
                    0,       scale.y, 0,       0,
                    0,       0,       scale.z, 0,
                    0,       0,       0,       1);
}

float4x4 toTranslationMatrix(float3 translation) {
    return float4x4(1,              0,              0,              translation.x,
                    0,              1,              0,              translation.y,
                    0,              0,              1,              translation.z,
                    0,              0,              0,              1);
}

vertex VertexOutput vertexMain(Vertex input [[stage_in]], constant float4x4& Camera [[buffer(1)]]) {
    VertexOutput Output = {0};

    Output.Color = input.Color;
	Output.TexCoord = input.TexCoord;
	Output.TexIndex = input.TexIndex;
	Output.TilingFactor = input.TilingFactor;
    Output.EntityID = input.EntityID;
    
	float4x4 scaleAsMat4 = toScaleMatrix(input.Scale);
    
    float4x4 rotAsMat4 = 
            rotationMatrix(float3(1, 0, 0), input.Rotation.x) 
        *   rotationMatrix(float3(0, 1, 0), input.Rotation.y) 
        *   rotationMatrix(float3(0, 0, 1), input.Rotation.z);

    float4x4 transAsMat4 = toTranslationMatrix(input.Translation);
    
    Output.Position = input.QuadIndexPos * scaleAsMat4 * rotAsMat4 * transAsMat4 * Camera;

    return Output;
}

constexpr sampler s = sampler(coord::normalized, address::clamp_to_edge, filter::linear);
fragment float4 fragmentMain(VertexOutput input [[stage_in]]) {
    float4 color = input.Color;
    float2 texCoord = input.TexCoord;
    float texIndex = input.TexIndex;
    float tilingFactor = input.TilingFactor;
    int entityID = input.EntityID;

    // float4 texColor = u_Textures[(size_t)input.TexIndex].sample(s, texCoord * tilingFactor);
    // color *= texColor;

    return color;
}