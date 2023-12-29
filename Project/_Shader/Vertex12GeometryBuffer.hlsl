#include "Header.hlsli"

LightVertexOutput main(VertexTextureNormalTangentBlend input)
{   
    LightVertexOutput output;
    
    //////////////////////////////
    
    matrix transform;
    
    [branch]
    if (hasAnimation)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
    
    output.pos = mul(input.pos, transform);
    
    //////////////////////////////
    
    output.viewPos  = invView._41_42_43;
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
                    
    output.normal   = normalize(mul(input.normal,  (float3x3)transform));
    output.tangent  = normalize(mul(input.tangent, (float3x3)transform));
    
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}