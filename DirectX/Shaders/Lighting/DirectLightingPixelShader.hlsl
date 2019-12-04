struct Material
{
    float shininess;
};

struct Light
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;

    float3 position;
};

Texture2DArray diffuseMaps : register(t0);
Texture2DArray specularMaps : register(t1);

SamplerState samplerState;

cbuffer NumberOfMaps : register(b0)
{
    uniform int diffuseMapsCount;
    int specularMapsCount;
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

float3 GetDiffuseColor(float2 texCoords)
{
    float4 diffuseColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < diffuseMapsCount; i++)
    {
        diffuseColor += diffuseMaps.Sample(samplerState, float3(texCoords, i));
    }

    return diffuseColor.rgb;
}

float3 GetSpecularColor(float2 texCoords)
{
    float4 specularColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < specularMapsCount; i++)
    {
        specularColor += specularMaps.Sample(samplerState, float3(texCoords, i));
    }

    return specularColor.rgb;
}

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS, float3 normals : NORMALS, float3 fragmentPosition : WORLD_POSITION) : SV_TARGET
{
    float3 lightDirection = normalize(-lightData.position);
    float theta = dot(lightDirection, normals);
    theta = max(0.0f, theta);

    float3 diffuseColor = GetDiffuseColor(uv);
    float3 ambientColor = (lightData.ambientColor.rgb * lightData.ambientColor.a) * diffuseColor;
    diffuseColor = (lightData.diffuseColor.rgb * lightData.diffuseColor.a) * theta * diffuseColor;

    float3 viewDirection = normalize(viewerPosition - fragmentPosition);
    float3 reflectionDirection = reflect(-lightDirection, normals);

    float specularHighligh = pow(max(0.0, dot(viewDirection, reflectionDirection)), materialData.shininess);
    float3 specularColor = GetSpecularColor(uv);

    float3 lightSpecularColor = lightData.specularColor.rgb * lightData.specularColor.a;
    specularColor = specularColor * specularHighligh * lightSpecularColor;

    return float4(ambientColor + diffuseColor + specularColor, 1.0f);
}