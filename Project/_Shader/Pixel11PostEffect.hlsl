#include "header.hlsli"

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
};

cbuffer ValueBuffer : register(b10)
{
    int type;
    
    float  scale;
    
    float2 imageSize;
}

static const float2 edges[8] =
{
    float2(-1, -1),
    float2(+0, -1),
    float2(+1, -1),
    float2(-1, +0),

    float2(+1, +0),
    float2(-1, +1),
    float2(+0, +1),
    float2(+1, +1),
};

float4 main(VertexOutput input) : SV_TARGET
{
    [branch]
    if (type == 0)
    {
        float2 temp = floor(input.uv * scale) / scale;

        return diffuseMap.Sample(samp, temp) * mDiffuse;
    }
    else if (type == 1)
    {
        float4 result = 0;
        
        for (int i = 0; i < scale; i++)
        {
            float2 div = (i + 1) / imageSize;

            [unroll(8)]
            for (int j = 0; j < 8; j++)
            {
                float2 uv = edges[j] * div + input.uv;
                result += diffuseMap.Sample(samp, uv);
            }
        }

        result /= scale * 8;
        
        return result * mDiffuse;
    }
    else 
    {
        return float4(1, 1, 1, 1);
    }

}