// Template for pixel shaders.

cbuffer SceneData: register(b0)
{
    float deltaTime;
    uint lightCount;
}

cbuffer CameraData: register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    float4 cameraPosition;
}

cbuffer MaterialData: register(b3)
{
   // Put the variables you will use in whatever order.
}

struct LightData
{
    float3 position;
    float3 direction;
    float3 color;
    float radius;
    float intensity;
    int type;
};

StructuredBuffer<LightData> lights: register(t10);

struct InputPS
{
    // Put the variables you will use in order.
};

float4 main(InputPS input): SV_Target0
{
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}