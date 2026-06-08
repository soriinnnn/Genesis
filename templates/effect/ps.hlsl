// Template for post-processing effect pixel shaders.

cbuffer PostProcessData: register(b0)
{
    // Put the variables you want in whatever order.
}

Texture2D colorTexture: register(t0);
SamplerState textureSampler: register(s0);

struct InputPS
{
    float4 position: SV_Position;
    float2 uv: TEXCOORD0;
};

float4 main(InputPS input): SV_Target0
{
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}