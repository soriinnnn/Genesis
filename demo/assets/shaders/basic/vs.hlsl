struct input
{
    float3 position: POSITION0;
    float4 color: COLOR0;
    float2 texCoord: TEXCOORD0;
};

struct output
{
    float4 position: SV_Position;
    float4 color: COLOR0;
    float2 texCoord: TEXCOORD0;
};

cbuffer ConstantData : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
}

output main(input invar)
{
    output outvar;
    
    outvar.position = mul(float4(invar.position, 1), world);
    outvar.position = mul(outvar.position, view);
    outvar.position = mul(outvar.position, projection);
    
    outvar.color = invar.color;
    outvar.texCoord = invar.texCoord;
    
    return outvar;
}