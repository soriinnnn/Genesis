struct OutputVS
{
    float4 clipPosition: SV_Position;
    float2 uv: TEXCOORD0;
};

OutputVS main(uint id: SV_VertexID)
{
    OutputVS output;
    
    output.uv = float2((id << 1) & 2, id & 2);
    output.clipPosition = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return output;
}