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
    float ka;
    float kd;
    float ks;
    float shininess;
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

StructuredBuffer<LightData> lights: register(t0);
SamplerState defaultSampler: register(s0);

Texture2D colorTexture: register(t1);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float3 normal: COLOR0;
    float2 uv: TEXCOORD0;
    float4 worldPosition: TEXCOORD1;
};

float3 getLightColor(LightData light, float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 worldPosition);

float4 main(InputPS input): SV_Target0
{
    float4 textureColor = colorTexture.Sample(defaultSampler, input.uv);
    float3 viewDirection = normalize(cameraPosition.xyz - input.worldPosition.xyz);
    float3 surfaceNormal = normalize(input.normal);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    for (uint i = 0; i < lightCount; i++) {
        color += getLightColor(lights[i], textureColor.xyz, surfaceNormal, viewDirection, input.worldPosition.xyz);
    }
    
    return float4(color, textureColor.a);
}

float3 getLightColor(LightData light, float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 worldPosition)
{
    float3 lightDirection = float3(0.0f, 0.0f, 0.0f);
    float attenuation = 0.0f;
    
    switch (light.type) {
        case 0: { // directional...
            lightDirection = normalize(-light.direction);
            attenuation = 1.0f;
            break;
        }
    }
    
    float diff = max(dot(surfaceNormal, lightDirection), 0.0f);
    float spec = 0.0f;
    
    if (diff > 0) {
        float3 reflectedLight = reflect(-lightDirection, surfaceNormal);
        spec = pow(max(dot(viewDirection, reflectedLight), 0.0f), shininess);
    }
    
    float3 ambient = baseColor * light.color * ka;
    float3 diffuse = diff * baseColor * light.color * kd;
    float3 specular = spec * light.color * ks;
    
    return (ambient + diffuse + specular) * light.intensity * attenuation;
}