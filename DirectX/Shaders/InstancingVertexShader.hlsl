struct Input
{
    float3 position : position;
    float4 color : COLOR;
    float2 uv : TEXCOORDS;

    matrix modelMatrix : MODEL_MATRIX;
};

struct VS_Out
{
    float4 color : COLOR;
    float2 uv : TEXCOORDS;
    float4 position : SV_POSITION;
};

cbuffer ViewProjection
{
    matrix vp;
};

VS_Out main(Input vertexAttributes)
{
    matrix mvp = mul(vp, vertexAttributes.modelMatrix);

    VS_Out output;
    output.color = vertexAttributes.color;
    output.uv = vertexAttributes.uv;
    output.position = mul(mvp, float4(vertexAttributes.position, 1.0f));

    return output;
}