struct VS_Out
{
    float4 color : COLOR;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float3 worldPosition : WORLD_POSITION;
    float4 position : SV_POSITION;
};

cbuffer ViewProjection : register(b0)
{
    matrix vp;
};

cbuffer Model : register(b1)
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
    output.worldPosition = mul(modelMatrix, float4(position, 1.0f)).xyz;

    return output;
}