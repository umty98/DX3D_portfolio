#include "header.hlsli"

struct VertexOutput
{
    float4 pos    : SV_POSITION;
    float2 uv     : UV;
    
    matrix projection : PROJECTION;
    matrix invView    : INVVIEW;
};

static const float2 arrBasePos[4] =
{
    float2(-1.0f, +1.0f),
    float2(+1.0f, +1.0f),
    float2(-1.0f, -1.0f),
    float2(+1.0f, -1.0f),
};

VertexOutput main(uint vertexID : SV_VertexID)
{   
    VertexOutput output;
    
    output.pos = float4(arrBasePos[vertexID].xy, 0.0f, 1.0f);
    output.uv  = output.pos.xy;
    
    output.projection = proj;
    output.invView    = invView;
    
    return output;
}