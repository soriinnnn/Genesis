struct input
{
    float4 position: SV_Position;
    float4 color: COLOR0;
    float2 texCoord: TEXCOORD0;
};

Texture2D tex: register(t0);
SamplerState state: register(s0);

float4 main(input invar): SV_Target0
{
    return tex.Sample(state, invar.texCoord);
}