Texture2DArray diffuseMaps : register(t0);
Texture2DArray specularMaps : register(t1);

SamplerState samplerState;

cbuffer NumberOfMaps : register(b0)
{
    int diffuseMapsCount;
    int specularMapsCount;
};

cbuffer DirectionalLight : register(b1)
{
    float4 directionalLightDirection;
    float4 directionalLightColor;
};

cbuffer ViewerPosition : register(b2)
{
    float3 viewerPosition;
};

static const float3 AMBIENT_COLOR = float3(0.3f, 0.3f, 0.3f);

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS, float3 normals : NORMALS) : SV_TARGET
{
    float3 lightDirection = normalize(directionalLightDirection.xyz);
    float theta = dot(lightDirection, normals);
    theta = max(0.0f, theta);

    float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < diffuseMapsCount; i++)
    {
        diffuseColor *= diffuseMaps.Sample(samplerState, float3(uv, i));
    }

    float3 outputColor = color.rgb * (directionalLightColor.rgb * directionalLightColor.a) * theta;
    return float4(outputColor * AMBIENT_COLOR, color.a) * diffuseColor;
}