#include "Common_BasicColour.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;

	// Transform to world space
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);

	// Pass vertex colour to pixel shader
	vOut.Colour = vIn.Colour;

	return vOut;
}