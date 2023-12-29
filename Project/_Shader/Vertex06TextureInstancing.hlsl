#include "Header.hlsli"

struct VertexInput
{
    float4 pos     : POSITION;
    float2 uv      : UV;
    float3 normal  : NORMAL;
    float3 tangent : TANGENT;
    
    matrix transform : INSTANCE_TRANSFORM;
    float4 color     : INSTANCE_COLOR;
};

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir  : VIEWDIR;
    float4 color    : COLOR;
};

VertexOutput main( VertexInput input )
{   
    VertexOutput output;
    
    output.pos = mul(input.pos, input.transform);
    
    float3 cameraPos = invView._41_42_43;
    
    output.viewDir = normalize(output.pos.xyz - cameraPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
                    
    output.normal   = normalize(mul(input.normal,  (float3x3)input.transform));
    output.tangent  = normalize(mul(input.tangent, (float3x3)input.transform));
    
    output.binormal = cross(output.normal, output.tangent);
    
    output.color = input.color;
    
    return output;
}