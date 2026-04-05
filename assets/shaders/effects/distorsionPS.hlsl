Texture2D frameTexture: register(t0);
SamplerState frameSampler: register(s0);

struct InputPS
{
    float4 position: SV_Position;
    float2 uv: TEXCOORD0;
};

float2 distort(float2 p)
{
    float theta = atan2(p.x, p.y);
    float radius = length(p);
    
    radius = pow(radius, 0.5);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5f * (p + 1.0f);
}

float4 main(InputPS input): SV_Target0
{
    float2 xy = 2.0f * input.uv.xy - 1.0f;
    float dist = length(xy);
    float2 uv;
    
    if (dist < 1.0f) {
        uv = distort(xy);
    }
    else {
        uv = input.uv;
    }
    
    float4 color = frameTexture.Sample(frameSampler, uv);
    return color;
}