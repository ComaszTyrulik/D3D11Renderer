Texture2D simpTexture;
SamplerState samplerState;

static const float3 LIGHT_DIRECTION = float3(-10.0f, 1.0f, -1.0f);
static const float3 LIGHT_COLOR = float3(1.0f, 1.0f, 1.0f);
static const float LIGHT_INTENSITY = 5.0f;

static const float3 AMBIENT_COLOR = float3(0.2f, 0.2f, 0.2f);

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS, float3 normals : NORMALS) : SV_TARGET
{
    float3 lightDirection = normalize(LIGHT_DIRECTION);
    float theta = dot(lightDirection, normals);
    theta = max(0.0f, theta);

    float3 outputColor = color.rgb * LIGHT_COLOR * theta * LIGHT_INTENSITY;

    float4 texelColor = simpTexture.Sample(samplerState, uv * 10.0f);

    return float4(outputColor * AMBIENT_COLOR, color.a) * texelColor;
    //return outputColor * float4(AMBIENT_COLOR.rgb * AMBIENT_COLOR.a, 1.0f);
}