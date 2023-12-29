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

struct GSOutput
{
	float4 pos   : SV_POSITION;
	float2 uv    : UV;
    float4 color : COLOR;
};

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f),
};

[maxvertexcount(4)]
void main( point VertexOutput input[1], inout TriangleStream< GSOutput > output)
{
    float3 forward = input[0].pos.xyz - input[0].invView._41_42_43;
    forward = normalize(forward);
    
    float3 up = normalize(-input[0].velocity);
    
    float3 right = normalize(cross(up, forward));
    
    float2 halfSize = input[0].size * 0.5f;
    
    float4 vertices[4] =
    {
        float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f),  
        float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f),  
        float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f),  
        float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f),
    };
    
    GSOutput element;
    
    element.color = input[0].color;
    
	for (uint i = 0; i < 4; i++)
	{
        element.pos = mul(vertices[i], input[0].view);
        element.pos = mul(element.pos, input[0].proj);
        
        element.uv = TEXCOORD[i];
        
        element.color.a = 0.2f * saturate(1 - element.pos.z / input[0].distance) * input[0].color.a;
        
		output.Append(element);
	}
}