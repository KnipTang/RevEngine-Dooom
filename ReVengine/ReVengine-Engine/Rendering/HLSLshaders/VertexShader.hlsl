cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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
    vs_output output = (vs_output) 0;
    
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.uv = input.uv;
    
    return output;
}