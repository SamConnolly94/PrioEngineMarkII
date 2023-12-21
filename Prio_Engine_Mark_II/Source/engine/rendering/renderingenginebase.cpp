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
	UpdateCameraMatrices();
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