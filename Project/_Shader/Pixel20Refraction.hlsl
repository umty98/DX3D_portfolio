#include "Header.hlsli"

struct VertexOutput
{
    float4 pos        : SV_POSITION;
    float2 uv         : UV;
    float4 refractPos : POSITION;
};

Texture2D refractionMap       : register(t11);
Texture2D refractionNormalMap : register(t12);

cbuffer WaterBuffer : register(b10)
{
    float4 color;
    
    float waveTime;
    float waveSpeed;
    float waveScale;
    float waveShininess;
    
    float fresnel;
}

float4 main(VertexOutput input) : SV_TARGET
{
    float2 uv;
    
    uv.x = +input.refractPos.x / input.refractPos.w * 0.5f + 0.5f;
    uv.y = -input.refractPos.y / input.refractPos.w * 0.5f + 0.5f;
    
    input.uv += waveTime * waveSpeed;
    
    float4 normal = refractionNormalMap.Sample(samp, input.uv) * 2.0f - 1.0f;
    uv += normal.xy * waveScale;
    
    return refractionMap.Sample(samp, uv) * color;
}