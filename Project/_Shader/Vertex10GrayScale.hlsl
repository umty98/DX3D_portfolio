#include "header.hlsli"

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
};

VertexOutput main( VertexTexture input )
{   
    VertexOutput output;
    
    output.pos = mul( input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
    
    return output;
}