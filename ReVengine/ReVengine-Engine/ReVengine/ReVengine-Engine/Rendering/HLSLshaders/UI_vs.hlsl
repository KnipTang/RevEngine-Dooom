cbuffer MatrixBuffer : register(b0)
{
    matrix orthoMatrix;
};

struct vs_input
{
    float4 position : POSITION;
    float2 uv : UV;
};

struct vs_output
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

vs_output vs_main(vs_input input)
{
    vs_output output;
    output.position = mul(input.position, orthoMatrix);
    output.uv = input.uv;
    return output;
}