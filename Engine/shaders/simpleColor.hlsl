// Vertex Shader: Simply transforms the position to screen space
void mainVP(
    float4 pos : POSITION,
    out float4 outPos : SV_POSITION,
    uniform float4x4 worldViewProj)
{
    outPos = mul(worldViewProj, pos);
}

// Pixel Shader: Simply returns Red (RGBA: 1, 0, 0, 1)
float4 mainFP() : SV_Target
{
    return float4(0.0, 1.0, 0.0, 1.0);
}