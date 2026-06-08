Texture2D colorTexture: register(t0);
SamplerState textureSampler: register(s0);

struct InputPS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
};

float4 main(InputPS input): SV_Target0
{
    return colorTexture.Sample(textureSampler, input.uv);
}