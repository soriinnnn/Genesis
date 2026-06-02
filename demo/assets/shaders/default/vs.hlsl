cbuffer CameraData: register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    float3 cameraPosition;
}

cbuffer ObjectData: register(b2)
{
    row_major float4x4 worldMatrix;
}

struct InputVS
{
    float3 position: POSITION0;
    float3 normal: COLOR0;
    float2 uv: TEXCOORD0;
};

struct OutputPS
{
    float4 clipPosition: SV_Position;
    float3 normal: COLOR0;
    float2 uv: TEXCOORD0;
    float4 worldPosition: TEXCOORD1;
};

OutputPS main(InputVS input)
{
    OutputPS output;
    float4 worldPosition = mul(float4(input.position, 1.0f), worldMatrix);    
   
    output.clipPosition = mul(mul(worldPosition, viewMatrix), projectionMatrix);
    output.normal = normalize(mul(float4(input.normal, 0.0f), worldMatrix).xyz);
    output.uv = input.uv;
    output.worldPosition = worldPosition;
    
    return output;
}