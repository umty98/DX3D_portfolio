#include "Header.hlsli"

struct VertexOutput
{
    float4        pos : SV_POSITION;
    float2         uv : UV;
    float4 reflectPos : POSITION0;
    float4 refractPos : POSITION1;
    float4   worldPos : POSITION2;
};

cbuffer ReflectionBuffer : register(b10)
{
    matrix reflectionView;
}

VertexOutput main(VertexTexture input)
{
    VertexOutput output;
    
    output.pos = mul( input.pos, world);
    
    output.worldPos = output.pos;
    
    output.reflectPos = mul(output.pos, reflectionView);
    output.reflectPos = mul(output.reflectPos, proj);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv = input.uv;
    output.refractPos = output.pos; 
    
    return output;
}
