struct VSinput
{
    float3 position: POSITION0;
    float4 color: COLOR0;
};

struct VSoutput
{
    float4 position: SV_Position;
    float4 color: COLOR0;
};

cbuffer ConstantData: register(b0)
{
    float scale;
}

VSoutput VSmain(VSinput input)
{
    VSoutput output;
    
    output.position = float4(input.position * scale, 1);
    output.color = input.color;
    
    return output;
}

float4 PSmain(VSoutput input): SV_Target
{
    return input.color;
}