Texture2D skyboxTexture: register(t0);
SamplerState skyboxSampler: register(s0);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
};

float4 main(InputPS input): SV_Target0
{
    return skyboxTexture.Sample(skyboxSampler, input.uv);
}