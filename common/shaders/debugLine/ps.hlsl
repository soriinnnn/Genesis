struct InputPS
{
    float4 clipPosition: SV_Position;
    float4 color: COLOR0;
};

float4 main(InputPS input): SV_Target0
{
    return input.color;
}