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
Texture2D normalTexture: register(t2);
Texture2D ambientOcclusionTexture: register(t3);
Texture2D roughnessTexture: register(t4);
Texture2D metallicTexture: register(t5);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
    float4 worldPosition: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

float3 getTangentSpaceNormal(SamplerState samplerState, Texture2D textureMap, float2 uv);
float3 getLightColor(LightData light, float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 worldPosition, float shininess, float roughness, float metallic);

float4 main(InputPS input): SV_Target0
{
    float4 textureColor = colorTexture.Sample(defaultSampler, input.uv);
    float3 viewDirection = normalize(cameraPosition.xyz - input.worldPosition.xyz);
    float3 tangentSpaceNormal = getTangentSpaceNormal(defaultSampler, normalTexture, input.uv);
    float3 surfaceNormal = normalize(mul(tangentSpaceNormal, input.tbn));
    
    float roughness = roughnessTexture.Sample(defaultSampler, input.uv).r;
    float metallic = metallicTexture.Sample(defaultSampler, input.uv).r;
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    for (uint i = 0; i < lightCount; i++) {
        color += getLightColor(lights[i], textureColor.xyz, surfaceNormal, viewDirection, input.worldPosition.xyz, shininess, roughness, metallic);
    }
    
    float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.uv).r;
    float3 ambientColor = textureColor.rgb * ambientOcclusion * ka;
    
    color += ambientColor;
   
    return float4(color, textureColor.a);
}

float3 getTangentSpaceNormal(SamplerState samplerState, Texture2D textureMap, float2 uv)
{
    float3 normal = textureMap.Sample(samplerState, uv).xyz;
    normal = normal * 2.0f - 1.0f;
    return normal;
}

float3 getLightColor(LightData light, float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 worldPosition, float shininess, float roughness, float metallic)
{
    float3 lightDirection = float3(0.0f, 0.0f, 0.0f);
    float attenuation = 0.0f;
    
    switch (light.type) {
        case 0:{ // directional...
            lightDirection = normalize(-light.direction);
            attenuation = 1.0f;
            break;
        }
    }
    
    float diff = max(dot(surfaceNormal, lightDirection), 0.0f);
    float spec = 0.0f;
         
    if (diff > 0) {
        float3 halfVector = normalize(lightDirection + viewDirection);
        
        // shininess derivat de roughness: materials tous = lluentor gran i difusa
        float adaptedShininess = pow(1.0f - roughness, 2.0f) * 256.0f;
        
        spec = pow(max(dot(surfaceNormal, halfVector), 0.0f), adaptedShininess * 4.0f);
    }
    
    // metalls no tenen llum difusa
    float3 diffuseColor = baseColor * (1.0f - metallic);
    
    // metalls agafen color de la textura, no-metalls blanc
    float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), baseColor, metallic);
    
    float3 diffuse = diff * diffuseColor * light.color * kd;
    float3 specular = spec * specularColor * ks;
    
    return (diffuse + specular) * light.intensity * attenuation;
}