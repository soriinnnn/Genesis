TextureCube skyboxTexture: register(t0);
SamplerState skyboxSampler: register(s0);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float3 sampleDirection: TEXCOORD0;
};

float4 main(InputPS input): SV_Target0
{
    float3 direction = normalize(input.sampleDirection);
    return skyboxTexture.Sample(skyboxSampler, direction);
}