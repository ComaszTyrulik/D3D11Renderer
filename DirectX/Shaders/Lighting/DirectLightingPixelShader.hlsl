struct Material
{
    float3 ambientColor;
    float3 diffuseColor;
    float3 specularColor;
    float shininess;
};

struct Light
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;

    float3 position;
};

cbuffer LightCBuffer : register(b1)
{
    Light lightData;
};

cbuffer ViewerPosition : register(b2)
{
    float3 viewerPosition;
};

cbuffer MaterialCBuffer : register(b3)
{
    Material materialData;
}

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS, float3 normals : NORMALS, float3 fragmentPosition : WORLD_POSITION) : SV_TARGET
{
    float3 ambientColor = (lightData.ambientColor.rgb * lightData.ambientColor.a) * materialData.ambientColor;

    float3 lightDirection = normalize(-lightData.position);
    float theta = dot(lightDirection, normals);
    theta = max(0.0f, theta);

    float3 diffuseColor = (lightData.diffuseColor.rgb * lightData.diffuseColor.a) * theta;
    diffuseColor *= materialData.diffuseColor;

    float3 viewDirection = normalize(viewerPosition - fragmentPosition);
    float3 reflectionDirection = reflect(-lightDirection, normals);

    float specularHighligh = pow(max(0.0, dot(viewDirection, reflectionDirection)), materialData.shininess);
    float3 specularColor = lightData.specularColor.rgb * lightData.specularColor.a;
    specularColor = materialData.specularColor * specularHighligh * specularColor;

    return float4(ambientColor + diffuseColor + specularColor, 1.0f);
}