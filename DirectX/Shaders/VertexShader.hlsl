struct VS_Out
{
    float4 color : COLOR;
    float2 uv : TEXCOORDS;
    float4 position : SV_POSITION;
};

cbuffer ModelViewProjection
{
    matrix vp;
};

VS_Out main(float3 position : POSITION, float4 color : COLOR, float2 uv : TEXCOORDS)
{
    VS_Out output;
    output.color = color;
    output.uv = uv;
    output.position = mul(vp, float4(position, 1.0f));

    return output;
}