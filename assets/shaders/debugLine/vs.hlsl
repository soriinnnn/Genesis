cbuffer CameraData: register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    float4 cameraPosition;
}

struct InputVS
{
    float3 position: POSITION0;
    float4 color: COLOR0;
};

struct OutputPS
{
    float4 position: SV_Position;
    float4 color: COLOR0;
};

OutputPS main(InputVS input)
{
    OutputPS output;
    
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.color = input.color;
    
    return output;
}