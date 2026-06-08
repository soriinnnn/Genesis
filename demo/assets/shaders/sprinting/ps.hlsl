cbuffer PostProcessData: register(b0)
{
    float intensity;
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
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 center = float2(0.5f, 0.5f);
    
    // desplaçament màxim del blur
    float maxShift = intensity * 0.15f;
    
    // desplaçament per aberració cromàtica
    float2 rOffset = center * intensity * 0.02f;
    float2 bOffset = center * intensity * -0.02f;
    
    const int samples = 8;
    for (int i = 0; i < samples; i++) {
        float t = (float) i / (samples - 1);
        
        float2 offsetUV = input.uv - center * t * maxShift;
        
        float2 rUV = offsetUV + rOffset * t;
        float2 gUV = offsetUV;
        float2 bUV = offsetUV + bOffset * t;
        
        float r = colorTexture.Sample(textureSampler, rUV).r;
        float g = colorTexture.Sample(textureSampler, gUV).g;
        float b = colorTexture.Sample(textureSampler, bUV).b;
    
        color += float4(r, g, b, 1.0f);
    }
    color /= samples;
    
    float distance = length(center);
    float vignette = 1.0f - distance * intensity * 0.5f;
    vignette = saturate(vignette);
    color.rgb *= vignette;
    
    return color;
}