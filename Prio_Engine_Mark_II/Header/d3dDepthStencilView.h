#pragma once

#include "EEngineTypes.h"
#include "PrioEngine.h"
#include "d3dUtil.h"
#include "Logger.h"
#include "EnumUtilities.h"

#include <Windows.h>
#include <memory>
#include <d3d12.h>
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;
using namespace PrioEngineII;


/**************************************************
*              DEVELOPER NOTE                     *
***************************************************
* If you're considering moving implementations to a CPP
* file, don't. Anything that includes a templated type
* is required to know the full implementation, otherwise
* you'll see symbols not recognised.
***************************************************/

namespace PrioEngineII
{

	template <class DirectXDeviceType, class DirectXResourceType, class CpuDescriptorHandle/*D3D12_CPU_DESCRIPTOR_HANDLE*/>
	class d3dDepthStencilView
	{
	public:

		d3dDepthStencilView(ComPtr<DirectXDeviceType> d3dDevice, 
			CpuDescriptorHandle depthStencilViewDescriptorHandle,
			EEngineTypes engineType,
			const int& width, 
			const int& height, 
			const bool& m4xMsaaState,
			UINT& m4xMsaaQuality)
		{
			mDepthStencilBuffer.Reset();
			CreateDepthStencilViewDesc(d3dDevice, depthStencilViewDescriptorHandle, engineType, width, height, m4xMsaaState, m4xMsaaQuality);
		}
	private:
		ComPtr<DirectXResourceType> mDepthStencilBuffer;

		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	public:
		ComPtr<DirectXResourceType> GetDepthStencilBufferPtr()
		{
			return mDepthStencilBuffer;
		}

	private:
		/// <summary>
		/// Initialises a constant buffer view descriptor, and creates the constant buffer view based on that descriptor.
		/// </summary>
		/// <param name="d3dDevice">The device that should be used to create the CBV from the descriptor</param>
		void CreateDepthStencilViewDesc(ComPtr<DirectXDeviceType> d3dDevice, 
			CpuDescriptorHandle depthStencilViewDescriptorHandle,
			EEngineTypes engineType, 
			const int& width, 
			const int& height, 
			const bool& m4xMsaaState, 
			const UINT m4xMsaaQuality)
		{
			switch (engineType)
			{
			case EEngineTypes::DX3D12:
				Logger::GetInstance()->Write("Creating depth stencil view for DirectX 12", ELogVerbosity::Trace);

				D3D12_RESOURCE_DESC depthStencilDesc;
				// Create the depth/stencil buffer and view.
				depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				depthStencilDesc.Alignment = 0;
				depthStencilDesc.Width = width;
				depthStencilDesc.Height = height;
				depthStencilDesc.DepthOrArraySize = 1;
				depthStencilDesc.MipLevels = 1;


				// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
				// the depth buffer.  Therefore, because we need to create two views to the same resource:
				//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
				//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
				// we need to create the depth buffer resource with a typeless format.  
				depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

				depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
				depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
				depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

				D3D12_CLEAR_VALUE optClear;
				optClear.Format = mDepthStencilFormat;
				optClear.DepthStencil.Depth = 1.0f;
				optClear.DepthStencil.Stencil = 0;
				ThrowIfFailed(d3dDevice->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&depthStencilDesc,
					D3D12_RESOURCE_STATE_COMMON,
					&optClear,
					IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

				// Create descriptor to mip level 0 of entire resource using the format of the resource.
				D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
				dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
				dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Format = mDepthStencilFormat;
				dsvDesc.Texture2D.MipSlice = 0;
				d3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, depthStencilViewDescriptorHandle);

				break;
			default:
				Logger::GetInstance()->Write("Attempted to create deptch stencil view for unsupported graphics API " + EnumUtilities::ToString(engineType), ELogVerbosity::Error);
				break;
			}
		};
	};
}