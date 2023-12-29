#include "header.hlsli"

struct VertexOutput
{
    float4 pos       : SV_POSITION;
    float3 originPos : POSITION;
};

TextureCube cubeMap : register(t10);

float4 main(VertexOutput input) : SV_TARGET
{
    return cubeMap.Sample(samp, input.originPos);
}