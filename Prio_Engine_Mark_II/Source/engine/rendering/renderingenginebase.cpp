#include <pch.h>
#include <engine/rendering/renderingenginebase.h>

#include <engine/rendering/types/graphicsapi.h>
#include <os/globals.h>

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

std::wstring CRenderingEngineBase::CalculateFrameStats(const float& totalTime)
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	constexpr float kCalculationPeriod = 1.0f;
	// Compute averages over X second period
	if (totalTime - timeElapsed >= kCalculationPeriod)
	{
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		// Reset for next average.
		frameCount = 0;
		timeElapsed += 1.0f;

		return L"FPS: " + fpsStr + L"    MSPF: " + mspfStr;
	}

	return L"";
}