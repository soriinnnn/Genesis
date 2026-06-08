cbuffer CameraData: register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    float4 cameraPosition;
}

struct InputVS
{
    float3 position: POSITION0;
    float2 uv: TEXCOORD0;
};

struct OutputPS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
};

OutputPS main(InputVS input)
{
    OutputPS output;
    
    float4x4 viewNoTranslation = viewMatrix;
    viewNoTranslation[3].xyz = float3(0.0f, 0.0f, 0.0f);
    
    float4 worldPosition = float4(input.position, 1.0f);
    output.clipPosition = mul(mul(worldPosition, viewNoTranslation), projectionMatrix);
    output.clipPosition.z = output.clipPosition.w;
    
    output.uv = input.uv;
    
    return output;
}