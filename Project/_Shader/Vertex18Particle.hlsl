#include "header.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv  : UV;
    
    matrix transform : INSTANCE_TRANSFORM;
    float4 color     : INSTANCE_COLOR;
};

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
    
    float4 color  : COLOR;
};

VertexOutput main(VertexInput input)
{   
    VertexOutput output;
    
    output.pos = mul( input.pos, input.transform);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
    
    output.color = input.color;
    
    return output;
}