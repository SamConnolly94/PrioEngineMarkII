#include "Common_BasicColour.hlsli"

float4 PS(VertexOut pIn) : SV_TARGET
{
    return pIn.Color;
}