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
    int pd0;
    float3 direction;
    int pd1;
    float3 color;
    float radius;
    float intensity;
    int type;
};

StructuredBuffer<LightData> lights: register(t10);

Texture2D colorTexture: register(t0);
SamplerState colorSampler: register(s0);

Texture2D normalTexture: register(t1);
SamplerState normalSampler: register(s1);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float4 worldPosition: TEXCOORD0;
    float2 uv: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

float3 getLightColor(uint lightIndex, float3 textureColor, float3 textureNormal, float3 viewDirection, float3 worldPosition)
{
    LightData light = lights[lightIndex];
    
    float3 lightDirection;
    float attenuation;
    
    switch (light.type) {
        // Directional light
        case 0: {
            lightDirection = normalize(-light.direction);
            attenuation = 1.0f;
            break;
        }
        default:
            return float3(0.0f, 0.0f, 0.0f);
    }
    
    float diff = max(dot(textureNormal, lightDirection), 0.0f);
    float spec = 0.0f;
    
    if (diff > 0) {
        float3 reflectedLight = reflect(-lightDirection, textureNormal);
        spec = pow(max(dot(viewDirection, reflectedLight), 0.0f), shininess);
    }

    float3 ambient = textureColor * light.color * ka;
    float3 diffuse = textureColor * light.color * diff * kd;
    float3 specular = light.color * spec * ks;
    
    return (ambient + diffuse + specular) * light.intensity * attenuation;
}

float4 getNormalVector(InputPS input)
{
    float4 normal = normalTexture.Sample(normalSampler, input.uv);
    normal.xyz = normal.xyz * 2.0f - 1.0f;
    normal.xyz = normalize(mul(normal.xyz, input.tbn));
    return normal;
}

float4 main(InputPS input): SV_Target0
{
    float4 textureColor = colorTexture.Sample(colorSampler, input.uv);
    float4 textureNormal = getNormalVector(input);
    float3 viewDirection = normalize(cameraPosition - input.worldPosition.xyz);
    
    float3 finalColor = textureColor.rgb * ka;
    for (uint i = 0; i < lightCount; i++) {
        finalColor += getLightColor(i, textureColor.rgb, textureNormal.xyz, viewDirection, input.worldPosition.xyz);
    }
    
    return float4(finalColor, textureColor.a);
}