Texture2D simpTexture;
SamplerState samplerState;

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS) : SV_TARGET
{
    //float4 texelColor = simpTexture.Sample(samplerState, uv);
    float4 texelColor = 1.0f;

    return texelColor * color;
}