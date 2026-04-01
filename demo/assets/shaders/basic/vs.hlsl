cbuffer CameraData: register(b0)
{
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 cameraPosition;
}

cbuffer ObjectData: register(b1)
{
    row_major float4x4 world;
}

struct InputVS
{
    float3 position: POSITION0;
    float3 normal: COLOR0;
    float3 tangent: TANGENT0;
    float3 bitangent: BINORMAL0;
    float2 uv: TEXCOORD0;
};

struct OutputPS
{
    float4 position: SV_Position;
    float2 uv: TEXCOORD0;
    float3 worldPosition: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

OutputPS main(InputVS input)
{
    OutputPS output;
    
    output.position = mul(float4(input.position, 1), world);    
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.uv = input.uv;
   
    output.tbn[0] = normalize(mul(input.tangent, (float3x3) world));
    output.tbn[1] = normalize(mul(input.bitangent, (float3x3) world));
    output.tbn[2] = normalize(mul(input.normal, (float3x3) world));
    
    return output;
}