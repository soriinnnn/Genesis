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
Texture2D roughnessTexture: register(t3);
Texture2D metallicTexture: register(t4);
Texture2D emissiveTexture: register(t5);
Texture2D aoTexture: register(t6);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
    float4 worldPosition: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

float3 calcTangentSpaceNormal(SamplerState samplerState, Texture2D textureMap, float2 uv);
float3 calcTotalLighting(InputPS input);

float3 fresnelSchlick(float dp, float metalness, float3 baseColor);
float geometrySmith(float dp, float roughness);
float distributionGGX(float dp, float roughness);
float3 calcSpecularBRDF(float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 lightDirection, float roughness, float metalness);
float3 calcDiffuseBRDF(float3 fLambert, float3 kd);
float3 calcLightingBRDF(float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 lightDirection, float3 lightIntensity, float roughness, float metalness);

float4 main(InputPS input): SV_Target0
{
    float3 finalColor = calcTotalLighting(input);
    
    float3 baseColor = colorTexture.Sample(defaultSampler, input.uv).rgb;
    float ao = aoTexture.Sample(defaultSampler, input.uv).r;
    float3 ambientColor = baseColor * ao * ka;
    float3 emissiveColor = emissiveTexture.Sample(defaultSampler, input.uv).rgb;
    
    finalColor += ambientColor;
    finalColor += emissiveColor;
   
    return float4(finalColor, 1.0f);
}

float3 calcTotalLighting(InputPS input)
{
    float3 tangentNormal = calcTangentSpaceNormal(defaultSampler, normalTexture, input.uv);
    
    float4 baseColor = colorTexture.Sample(defaultSampler, input.uv);
    float3 surfaceNormal = normalize(mul(tangentNormal, input.tbn));
    float3 viewDirection = normalize(cameraPosition.xyz - input.worldPosition.xyz);
    
    float roughness = roughnessTexture.Sample(defaultSampler, input.uv).r;
    float metalness = metallicTexture.Sample(defaultSampler, input.uv).r;
    
    float3 totalLighting = float3(0.0f, 0.0f, 0.0f);
    
    for (uint i = 0; i < lightCount; i++) {
        LightData light = lights[i];
        
        float3 lightDirection = float3(0.0f, 0.0f, 0.0f);
        float3 lightIntensity = light.color * light.intensity;
        
        switch (light.type) {
            case 0:{ // directional
                lightDirection = normalize(-light.direction);
                break;
            }
        }
        
        totalLighting += calcLightingBRDF(baseColor.rgb, surfaceNormal, viewDirection, lightDirection, lightIntensity, roughness, metalness);
    }
    
    return totalLighting;
}

float3 calcTangentSpaceNormal(SamplerState samplerState, Texture2D textureMap, float2 uv)
{
    float3 normal = textureMap.Sample(samplerState, uv).xyz;
    normal = normal * 2.0f - 1.0f;
    return normal;
}

static const float PI = 3.14159265359;

float3 calcLightingBRDF(float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 lightDirection, float3 lightIntensity, float roughness, float metalness)
{
    float3 n = surfaceNormal;
    float3 v = viewDirection;
    float3 l = lightDirection;
    float3 h = normalize(viewDirection + lightDirection);
    
    float vDotH = max(dot(v, h), 0.0f);
    float nDotL = max(dot(n, l), 0.0f);
    
    float3 fLambert = lerp(baseColor, float3(0.0f, 0.0f, 0.0f), metalness);
    float3 kd = float3(1.0f, 1.0f, 1.0f) - fresnelSchlick(vDotH, metalness, baseColor);
    
    float3 diffBRDF = calcDiffuseBRDF(fLambert, kd);
    float3 specBRDF = calcSpecularBRDF(baseColor, surfaceNormal, viewDirection, lightDirection, roughness, metalness);
    
    float3 finalColor = (diffBRDF + specBRDF) * lightIntensity * nDotL;
    return finalColor;
}

// kd = 1.0f - ks
float3 calcDiffuseBRDF(float3 fLambert, float3 kd)
{
    return kd * fLambert / PI;
}

float3 calcSpecularBRDF(float3 baseColor, float3 surfaceNormal, float3 viewDirection, float3 lightDirection, float roughness, float metalness)
{
    float3 n = surfaceNormal;
    float3 v = viewDirection;
    float3 l = lightDirection;
    float3 h = normalize(viewDirection + lightDirection);
    
    float nDotH = max(dot(n, h), 0.0f);
    float vDotH = max(dot(v, h), 0.0f);
    float nDotV = max(dot(n, v), 0.0f);
    float nDotL = max(dot(n, l), 0.0f);
    
    float D = distributionGGX(nDotH, roughness);
    float G = geometrySmith(nDotL, roughness) * geometrySmith(nDotV, roughness);
    float3 F = fresnelSchlick(vDotH, metalness, baseColor);
    
    float3 num = D * G * F;
    float den = 4.0f * nDotV * nDotL + 0.0001f;
    
    return num / den;
}

// dp = dot(normal, half)
float distributionGGX(float dp, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float d = dp * dp * (alpha2 - 1.0f) + 1.0f;
    return alpha2 / (PI * d * d);
}

// dp = dot(normal, view) || dot(normal, light)
float geometrySmith(float dp, float roughness)
{
    float alpha = roughness;
    float k = (alpha + 1.0f) * (alpha + 1.0f) / 8.0f;
    float d = dp * (1.0f - k) + k;
    return dp / d;
}

// dp = dot(view, half)
float3 fresnelSchlick(float dp, float metalness, float3 baseColor)
{
    const float baseR = 0.04f; // reflectància base per a materials no metàl·lics
    float3 f0 = lerp(float3(baseR, baseR, baseR), baseColor, metalness);
    return f0 + (1.0f - f0) * pow(clamp(1.0f - dp, 0.0f, 1.0f), 5.0f);
}