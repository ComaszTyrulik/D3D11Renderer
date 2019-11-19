struct VS_Out
{
    float4 color : COLOR;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float4 position : SV_POSITION;
};

cbuffer ViewProjection
{
    matrix vp;
};

cbuffer Model
{
    matrix modelMatrix;
};

VS_Out main(float3 position : POSITION, float4 color : COLOR, float2 uv : TEXCOORDS, float3 normals : NORMALS)
{
    matrix mvp = mul(vp, modelMatrix);

    VS_Out output;
    output.color = color;
    output.uv = uv;
    output.position = mul(mvp, float4(position, 1.0f));
    output.normals = normalize(mul(mvp, float4(normals, 0.0f)).xyz);

    return output;
}