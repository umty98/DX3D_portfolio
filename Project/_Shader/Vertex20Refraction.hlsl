#include "Header.hlsli"

struct VertexOutput
{
    float4 pos        : SV_POSITION;
    float2 uv         : UV;
    float4 refractPos : POSITION;
};

VertexOutput main(VertexTexture input)
{
    VertexOutput output;
    
    output.pos = mul( input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv = input.uv;
    
    output.refractPos = output.pos; 
    
    return output;
}
