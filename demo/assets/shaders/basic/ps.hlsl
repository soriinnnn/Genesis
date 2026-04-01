cbuffer CameraData: register(b0)
{
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 cameraPosition;
}

cbuffer LightData: register(b2)
{
    float3 lightDirection;
    float3 lightColor;
    float lightIntensity;
}

cbuffer MaterialData: register(b3)
{
    float ka;
    float kd;
    float ks;
    float shininess;
}

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
    
    normal.xyz = normal.xyz * 2.0 - 1.0;
    normal.xyz = normalize(mul(normal.xyz, input.tbn));
    
    float3 reflectedLight = reflect(lightDirection, normal.xyz);
    float3 viewDirection = normalize(cameraPosition - input.worldPosition);
    
    float diffuse = max(dot(-lightDirection, normal.xyz), 0.0f);
    float specular = 0.0f;
    if (dot(-lightDirection, input.tbn[2]) > 0) {
        specular = pow(max(dot(reflectedLight, viewDirection), 0.0f), shininess);
    }
    
    float3 ambientLight = ka * lightColor * color.rgb;
    float3 diffuseLight = kd * diffuse * lightColor * color.rgb;
    float3 specularLight = ks * specular * lightColor;
    
    return float4(ambientLight + diffuseLight + specularLight, color.a);
}