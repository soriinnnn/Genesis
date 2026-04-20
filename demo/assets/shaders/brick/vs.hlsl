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
    float3 normal: NORMAL0;
    float3 tangent: TANGENT0;
    float3 bitangent: BINORMAL0;
    float2 uv: TEXCOORD0;
};

struct OutputPS
{
    float4 clipPosition: SV_Position;
    float4 worldPosition: TEXCOORD0;
    float2 uv: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

OutputPS main(InputVS input)
{
    OutputPS output;
    
    output.worldPosition = mul(float4(input.position, 1.0f), worldMatrix);
    
    float4 viewPosition = mul(output.worldPosition, viewMatrix);
    output.clipPosition = mul(viewPosition, projectionMatrix);
    
    output.uv = input.uv;
   
    output.tbn[0] = normalize(mul(input.tangent, (float3x3) worldMatrix));
    output.tbn[1] = normalize(mul(input.bitangent, (float3x3) worldMatrix));
    output.tbn[2] = normalize(mul(input.normal, (float3x3) worldMatrix));
    
    return output;
}