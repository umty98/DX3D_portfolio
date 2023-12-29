#include "Header.hlsli"

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
    
    matrix projection : PROJECTION;
    matrix invView    : INVVIEW;
};

struct SurfaceData
{
    float  linearDepth;
    float3 color;
    float3 normal;
    float  specInt;
    float  specPow;
};

Texture2D  diffuseTexture : register(t10);
Texture2D specularTexture : register(t11);
Texture2D   normalTexture : register(t12);
Texture2D    depthTexture : register(t13);

float ConvertDepthToLinear(float depth)
{
    float linearDepth = proj._43 / (depth - proj._33);
    
    return linearDepth;
}

SurfaceData UnpackGBuffer(int2 location)
{
    SurfaceData output;
    
    int3 location3 = int3(location, 0);

    float depth = depthTexture.Load(location3).x;
    output.linearDepth = ConvertDepthToLinear(depth);
    output.color = diffuseTexture.Load(location3).rgb;

    float4 specular = specularTexture.Load(location3);
    output.specPow = MIN_SHININESS + specular.x * MAX_SHININESS;
    output.specInt = specular.y;

    output.normal = normalTexture.Load(location3).xyz;
    output.normal = normalize(output.normal * 2.0f - 1.0f);

    return output;
}

float3 CalculateWorldPos(float2 pos, float linearDepth, matrix inverseView, matrix projection)
{
    float4 position;
    
    float2 temp;
    temp.x = 1 / projection._11;
    temp.y = 1 / projection._22;
    
    position.xy = pos.xy * temp * linearDepth;
    position.z  = linearDepth;
    position.w  = 1.0f;
    
    return mul(position, inverseView).xyz;
}

float4 main(VertexOutput input) : SV_TARGET
{
    SurfaceData data = UnpackGBuffer(input.pos.xy);

    LightMaterial material;
    material.normal            = data.normal;
    material.diffuseColor      = float4(data.color, 1.0f);
    material.specularIntensity = data.specInt;
    material.viewPos           = input.invView._41_42_43;
    material.worldPos          = CalculateWorldPos(input.uv, data.linearDepth, input.invView, input.projection);

    return CalculateLights(material);
}