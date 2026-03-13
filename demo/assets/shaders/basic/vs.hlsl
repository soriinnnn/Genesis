struct InputVS
{
    float3 position: POSITION0;
    float3 normal: COLOR0;
    float2 texCoord: TEXCOORD0;
};

struct OutputPS
{
    float4 position: SV_Position;
    float3 normal: COLOR0;
    float2 texCoord: TEXCOORD0;
};

cbuffer ConstantData: register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 lightDirection;
    float3 camPosition;
}

OutputPS main(InputVS input)
{
    OutputPS output;
    
    output.position = mul(float4(input.position, 1), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    
    return output;
}