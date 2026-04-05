Texture2D quadTexture: register(t0);
SamplerState quadSampler: register(s0);

struct InputPS
{
    float4 position: SV_Position;
    float2 uv: TEXCOORD0;
};

float4 main(InputPS input): SV_Target0
{
    return quadTexture.Sample(quadSampler, input.uv);
}