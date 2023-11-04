#include <pch.h>
#include <engine/rendering/renderingenginebase.h>

#include <engine/rendering/types/graphicsapi.h>
#include <os/globals.h>
#include <maths/vector4.h>
#include <maths/common/vectorhelper.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

#include <string>
#include <Windows.h>
#include <wrl.h>

using namespace PrioEngine::Globals::OS;
using Microsoft::WRL::ComPtr;
using namespace std;

CRenderingEngineBase::CRenderingEngineBase(const EGraphicsAPI& graphicsApi)
{
	m_GraphicsApi = graphicsApi;
}

EGraphicsAPI CRenderingEngineBase::GetGraphicsAPI() const
{
	return m_GraphicsApi;
}

void CRenderingEngineBase::Update()
{
	using namespace PrioEngine::Math;
	using namespace DirectX;

	float x = m_Radius * sinf(m_Phi) * cosf(m_Theta);
	float z = m_Radius * sinf(m_Phi) * sinf(m_Theta);
	float y = m_Radius * cosf(m_Phi);

	// TODO:
	// Move below
	// DirectX specific stuff going on here which indicates it belongs in the d3d12renderingengine.cpp file instead
	Vector4 pos = { x, y, z, 0.0f };
	Vector4 target = Vector4::GetZero();
	Vector4 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(ToXMVECTOR(pos), ToXMVECTOR(target), ToXMVECTOR(up));
	XMStoreFloat4x4(&m_View, view);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view * proj;

	// Update the constant buffer with the latest worldViewProj matrix.
	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	mObjectCB->CopyData(0, objConstants);
}

std::wstring CRenderingEngineBase::CalculateFrameStats(const float& totalTime)
{
	m_FrameCount++;
	constexpr float kCalculationPeriod = 1.0f;

	// Compute averages over X second period
	if (totalTime - m_TimeElapsed >= kCalculationPeriod)
	{
		float fps = (float)m_FrameCount;
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		// Reset for next average.
		m_FrameCount = 0;
		m_TimeElapsed += 1.0f;

		return L"FPS: " + fpsStr + L"    MSPF: " + mspfStr;
	}

	return L"";
}