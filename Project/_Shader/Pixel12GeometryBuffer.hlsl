#include "Header.hlsli"

GBufferOutput main(LightVertexOutput input)
{
    LightMaterial material = GetLightMaterial(input);

    return PackGBuffer(material.diffuseColor.rgb, material.normal, material.specularIntensity.r);
}