#include "header.hlsli"

cbuffer ValueBuffer : register(b10)
{
    float value;
}

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
};

Texture2D backImage : register(t10);

float4 main(VertexOutput input) : SV_TARGET
{
    if (input.uv.x < value)
        return diffuseMap.Sample(samp, input.uv);
    
    return backImage.Sample(samp, input.uv);
}