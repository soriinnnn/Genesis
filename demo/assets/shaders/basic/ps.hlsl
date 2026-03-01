struct input
{
    float4 position: SV_Position;
    float4 color: COLOR0;
};

float4 main(input invar): SV_Target
{
    return invar.color;
}