#include "Header.hlsli"

struct VertexOutput
{
    float4      pos : SV_POSITION;
    float2       uv : UV;
    float3   normal : NORMAL;
    float3  tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3  viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4  clipPos : POSITION2;
};

Texture2D depthMap : register(t10);

float4 main(VertexOutput input) : SV_TARGET
{
    LightVertexOutput data;
    
    data.     pos = input.     pos;
    data.      uv = input.      uv;
    data.  normal = input.  normal;
    data. tangent = input. tangent;
    data.binormal = input.binormal;
    data. viewPos = input. viewPos;
    data.worldPos = input.worldPos;
    
    LightMaterial material = GetLightMaterial(data);
    
    float4 ambient = CalculateAmbient(material);
    float4   color = CalculateLights (material);
    
    float4 result = ambient + color + mEmissive;
    
    
    float2 uv = input.clipPos.xy / input.clipPos.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    //if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
    //    return result;
    
    float  shadowDepth = depthMap.Sample(samp, uv).r;
    
    float currentDepth = input.clipPos.z / input.clipPos.w;
    
    if (currentDepth > shadowDepth)
        return result * 0.5f;
    
    
    return result;
}