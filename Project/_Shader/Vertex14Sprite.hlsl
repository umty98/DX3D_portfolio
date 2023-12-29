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

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    output.pos     = input.pos;
    output.size    = input.size;
    output.invView = invView;
    output.view    = view;
    output.proj    = proj;
    
    return output;
}