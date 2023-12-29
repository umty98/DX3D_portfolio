#include "Header.hlsli"

struct VertexInput
{
    float4      pos : POSITION;
    float2     size : SIZE;
    float3 velocity : VELOCITY;
};

struct VertexOutput //Geometry Input
{
    float4 pos     : POSITION0;
    float2 size    : SIZE;
    matrix invView : INVVIEW;
    matrix view    : VIEW;
    matrix proj    : PROJECTION;
};

cbuffer SparkBuffer : register(b10)
{
    float time;
    float duration;
}

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    output.pos     = input.pos + float4(input.velocity, 0.0f) * time;
    output.size    = input.size;
    output.invView = invView;
    output.view    = view;
    output.proj    = proj;
    
    return output;
}