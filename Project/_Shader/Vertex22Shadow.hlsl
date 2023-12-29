#include "Header.hlsli"

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    
    float3  viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4  clipPos : POSITION2;
};

cbuffer LightView : register(b11)
{
    matrix lightView;
}

cbuffer LightProj : register(b12)
{
    matrix lightProj;
}

VertexOutput main(VertexTextureNormalTangentBlend input)
{
    VertexOutput output;
    
    matrix transform;
    
    [branch]
    if (hasAnimation)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
    
    output.pos = mul(input.pos, transform);
    
    output.viewPos  = invView._41_42_43;
    output.worldPos = output.pos;
    
    output.clipPos = mul(output.pos,     lightView);
    output.clipPos = mul(output.clipPos, lightProj);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
                    
    output.normal   = normalize(mul(input.normal,  (float3x3)transform));
    output.tangent  = normalize(mul(input.tangent, (float3x3)transform));
    
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}