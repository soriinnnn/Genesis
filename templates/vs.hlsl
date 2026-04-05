// Template for vertex shaders.

cbuffer SceneData: register(b0)
{
    float deltaTime;
    uint lightCount;
}

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
    // Put the variables you will use in order.
};

struct OutputPS
{
    // Put the variables you will use in whatever order.
};

OutputPS main(InputVS input)
{
    OutputPS output;
    return output;
}