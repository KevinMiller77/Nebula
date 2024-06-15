#include <metal_stdlib>
using namespace metal;

struct VertexInput {
    float3 Position [[attribute(0)]];
    float4 Color [[attribute(1)]];
};

struct VertexOutput {
    float4 Position [[position]];
    float4 Color [[shared]];
};

struct FragmentOutput {
    float4 Color [[color(0)]];
    int    EntityID [[color(1)]];
};

vertex VertexOutput vertexMain(const VertexInput in [[stage_in]], constant float4x4& Camera [[buffer(1)]]) {
    VertexOutput out {0};
    out.Position = float4(in.Position, 1.0) * Camera;
    out.Color = in.Color;
 
    return out;
}

fragment FragmentOutput fragmentMain(VertexOutput in [[stage_in]]) {
    return { in.Color, -1 };
}