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

Texture2D colorTexture: register(t1);
Texture2D normalTexture: register(t2);

SamplerState defaultSampler: register(s0);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float4 worldPosition: TEXCOORD0;
    float2 uv: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

float3 getLightColor(uint lightIndex, float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 worldPosition)
{
    LightData light = lights[lightIndex];
    
    float3 lightDirection = float3(0.0f, 0.0f, 0.0f);
    float attenuation = 0.0f;
    
    switch (light.type) {
        // Directional light
        case 0: {
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
    float3 diffuse = baseColor * light.color * diff * kd;
    float3 specular = light.color * spec * ks;
    
    return (ambient + diffuse + specular) * light.intensity * attenuation;
}

float4 getTextureNormal(InputPS input)
{
    float4 normal = normalTexture.Sample(defaultSampler, input.uv);
    normal.xyz = normal.xyz * 2.0f - 1.0f;
    normal.xyz = normalize(mul(normal.xyz, input.tbn));
    return normal;
}

float4 main(InputPS input): SV_Target0
{
    float4 textureColor = colorTexture.Sample(defaultSampler, input.uv);
    float4 textureNormal = getTextureNormal(input);
    float4 viewDirection = normalize(cameraPosition - input.worldPosition);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    for (uint i = 0; i < lightCount; i++) {
        color += getLightColor(i, textureColor.rgb, textureNormal.xyz, viewDirection.xyz, input.worldPosition.xyz);
    }

    return float4(color, textureColor.a);
}