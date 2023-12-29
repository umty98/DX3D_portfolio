#include "Header.hlsli"

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir  : VIEWDIR;
};

VertexOutput main(VertexInstancing input)
{   
    VertexOutput output;
    
    //////////////////////////////
    
    matrix transform = mul(SkinWorld(input.index, input.indices, input.weights), input.transform);
    
    output.pos = mul(input.pos, transform);
    
    
    //////////////////////////////
    
    float3 cameraPos = invView._41_42_43;
    
    output.viewDir = normalize(output.pos.xyz - cameraPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
                    
    output.normal   = normalize(mul(input.normal,  (float3x3) input.transform));
    output.tangent  = normalize(mul(input.tangent, (float3x3) input.transform));
    
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}