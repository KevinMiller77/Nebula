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

vertex VertexOutput vertexMain(const VertexInput in [[stage_in]], constant float4x4& Camera [[buffer(1)]]) {
    VertexOutput out {0};
    out.Position = float4(in.Position, 1.0) * Camera;
    out.Color = in.Color;
 
    return out;
}

fragment float4 fragmentMain(VertexOutput in [[stage_in]]) {
    return in.Color;
}