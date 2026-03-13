struct InputPS
{
    float4 position: SV_Position;
    float3 normal: COLOR0;
    float2 texCoord: TEXCOORD0;
};

cbuffer ConstantData : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 lightDirection;
    float3 camPosition;
}

Texture2D tex: register(t0);
SamplerState state: register(s0);

float4 main(InputPS input): SV_Target0
{
    float ka = 0.1f;
    float3 ambientColor = float3(1.0f, 1.0f, 1.0f);
    float3 ambientLight = ka * ambientColor;
    
    float kd = 1.0f;
    float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);
    float diffuse = max(dot(-lightDirection, input.normal), 0.0f);
    float3 diffuseLight = kd * diffuse * diffuseColor;
    
    float ks = 1.0f;
    float shininess = 100.0f;
    float3 specularColor = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(lightDirection, input.normal);
    float3 viewDir = normalize(camPosition - input.position.xyz);
    float specular = pow(max(dot(reflectedLight, viewDir), 0.0f), shininess);
    float3 specularLight = ks * specular * specularColor;
    
    //float2 coords = frac(input.texCoord);
    //return tex.Sample(state, coords);
    return float4(ambientLight + diffuseLight + specularLight, 1.0f);
}