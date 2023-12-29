#include "Header.hlsli"

cbuffer SpriteBuffer : register(b10)
{
    float2 maxFrame;
    float2 curFrame;
}

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv  : UV;
};

float4 main(GSOutput input) : SV_TARGET
{
    float2 uv = (input.uv + curFrame) / maxFrame;
    
    return diffuseMap.Sample(samp, uv) * mDiffuse;
}