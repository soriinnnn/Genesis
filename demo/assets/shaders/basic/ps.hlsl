struct InputPS
{
    float4 position: SV_Position;
    float3 normal: COLOR0;
    float2 texCoord: TEXCOORD0;
};

cbuffer CameraData: register(b0)
{
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 camPosition;
    float3 lightDirection;
}

cbuffer MaterialProperties: register(b2)
{
    float ka;
    float kd;
    float ks;
    float shininess;
}

Texture2D tex: register(t0);
SamplerState state: register(s0);

float4 main(InputPS input): SV_Target0
{
    float3 ambientColor = float3(1.0f, 1.0f, 1.0f);
    float3 ambientLight = ka * ambientColor;
    
    float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);
    float diffuse = max(dot(-lightDirection, input.normal), 0.0f);
    float3 diffuseLight = kd * diffuse * diffuseColor;
    
    float3 specularColor = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(lightDirection, input.normal);
    float3 viewDir = normalize(camPosition - input.position.xyz);
    float specular = pow(max(dot(reflectedLight, viewDir), 0.0f), shininess);
    float3 specularLight = ks * specular * specularColor;
    
    //float2 coords = frac(input.texCoord);
    //return tex.Sample(state, coords);
    return float4(ambientLight + diffuseLight + specularLight, 1.0f);
}