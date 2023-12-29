#include "Header.hlsli"

struct VertexInput
{
    float4 pos    : POSITION;
    float2 size   : SIZE;
    float2 random : RANDOM;
};

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

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    float3 displace = time * velocity;
    
    input.pos.x += cos(time - input.random.x) * turbulence;
    input.pos.z += cos(time - input.random.y) * turbulence;
    
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);

    output.velocity = velocity;
    output.distance = distance;
    output.color    = color;
    
    output.pos     = input.pos;
    output.size    = input.size;
    output.invView = invView;
    output.view    = view;
    output.proj    = proj;
    
    return output;
}