cbuffer SceneData: register(b0)
{
    float deltaTime;
    uint lightCount;
}

cbuffer CameraData: register(b1)
{
    row_major float4x4 view;
    row_major float4x4 projection;
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
    int type;
    float3 direction;
    float radius;
    float3 color;
    float intensity;
};

StructuredBuffer<LightData> lights: register(t10);

Texture2D colorTexture: register(t0);
SamplerState colorSampler: register(s0);

Texture2D normalTexture: register(t1);
SamplerState normalSampler: register(s1);

struct InputPS
{
    float4 position: SV_Position;
    float2 uv: TEXCOORD0;
    float3 worldPosition: TEXCOORD1;
    row_major float3x3 tbn: TEXCOORD2;
};

float4 main(InputPS input): SV_Target0
{
    float4 color = colorTexture.Sample(colorSampler, input.uv);
    float4 normal = normalTexture.Sample(normalSampler, input.uv);
    float3 viewDirection = normalize(cameraPosition - input.worldPosition);
    
    normal.xyz = normal.xyz * 2.0 - 1.0;
    normal.xyz = normalize(mul(normal.xyz, input.tbn));
    
    float3 totalLight = ka * color.rgb;
    for (uint i = 0; i < lightCount; i++) {
        LightData light = lights[i];
        
        float3 lightDirection = float3(0.0f, 0.0f, 0.0f);
        float attenuation = 1.0f;

        if (light.type == 0) // Directional light
        {
            lightDirection = normalize(-light.direction);
            attenuation = 1.0f;
        }
        else if (light.type == 1) // Point light
        {
            float3 vectorLightWorld = light.position - input.worldPosition;
            float distance = length(vectorLightWorld);
            
            if (distance > light.radius) {
                continue;
            }
            
            lightDirection = normalize(vectorLightWorld);
            attenuation = 1.0f - (distance / light.radius);
            // attenuation = attenuation * attenuation;
        }
        
        float diffuse = max(dot(normal.xyz, lightDirection), 0.0f);
        float specular = 0.0f;
        
        // Només calculem el reflex si la llum realment toca aquesta cara
        if (diffuse > 0.0f) {
            float3 reflectedLight = reflect(-lightDirection, normal.xyz);
            specular = pow(max(dot(reflectedLight, viewDirection), 0.0f), shininess);
        }
        
        // Combinem els valors utilitzant el color, la intensitat i l'atenuació de la llum actual
        float3 currentLightColor = light.color * light.intensity * attenuation;
        
        float3 diffuseLight = kd * diffuse * currentLightColor * color.rgb;
        float3 specularLight = ks * specular * currentLightColor;
        
        totalLight += (diffuseLight + specularLight);
    }
    
    return float4(totalLight, color.a);
}