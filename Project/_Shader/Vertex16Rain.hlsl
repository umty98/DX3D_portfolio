#include "Header.hlsli"

struct VertexOutput //Geometry Input
{
    float4 pos      : POSITION0;
    float2 size     : SIZE;
    matrix invView  : INVVIEW;
    matrix view     : VIEW;
    matrix proj     : PROJECTION;
    
    float  distance : DISTANCE;
    float4 color    : COLOR;
    float3 velocity : VELOCITY;
};

cbuffer WeatherBuffer : register(b10)
{
    float3 velocity;
    float  distance;
    
    float4 color;
    
    float3 origin;
    float  time;
    
    float3 size;
    float  turbulence;
}

VertexOutput main(VertexTexture input)
{
    VertexOutput output;
    
    float3 v = velocity;
    v.xz /= input.uv.y * turbulence;
    
    output.velocity = v;
    
    float3 displace = time * v;
    
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
    
    float alpha = cos(time + (input.pos.x + input.pos.z));
    alpha = saturate(1.5f + alpha / distance * 2.0f);
    
    output.distance = distance;
    output.color    = float4(color.rgb, alpha);
    
    output.pos     = input.pos;
    output.size    = input.uv;
    output.invView = invView;
    output.view    = view;
    output.proj    = proj;
    
    return output;
}