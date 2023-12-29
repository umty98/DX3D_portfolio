#include "Header.hlsli"

struct VertexOutput
{
    float4 pos        : SV_POSITION;
    float4 reflectPos : POSITION;
};

cbuffer ReflectionBuffer : register(b10)
{
    matrix reflectionView;
}

VertexOutput main(VertexTexture input)
{
    VertexOutput output;
    
    output.pos = mul( input.pos, world);
    
    output.reflectPos = mul(output.pos, reflectionView);
    output.reflectPos = mul(output.reflectPos, proj);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    return output;
}
