#include "Header.hlsli"

cbuffer SparkBuffer : register(b10)
{
    float time;
    float duration;
}

cbuffer StartColorBuffer : register(b11)
{
    float4 startColor;
}

cbuffer EndColorBuffer : register(b12)
{
    float4 endColor;
}

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv  : UV;
};

float4 main(GSOutput input) : SV_TARGET
{
    float4 color = lerp(startColor, endColor, time / duration);
    
    return diffuseMap.Sample(samp, input.uv) * color;
}