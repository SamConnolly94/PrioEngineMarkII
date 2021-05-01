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
	/// <summary>
	/// A wrapper for the render target view heap for Direct X based engines. Will provide functionality for creating the render target view,
	/// and should only require you to provide the required types.
	/// </summary>
	/// <typeparam name="DirectXDevice"></typeparam>
	/// <typeparam name="HeapType"></typeparam>
	/// <typeparam name="HeapDescriptorType"></typeparam>
	template <class DirectXDeviceType, class HeapType, class HeapDescriptorType>
	class d3dRenderTargetViewHeap
	{
	public:
		/// <summary>
		/// Constructor which will initialise the render target view
		/// </summary>
		/// <param name="d3dDevice">The pointer to the Direct X device</param>
		/// <param name="engineType">The type of engine (to work out which version of Direct X we should create the RTV for.</param>
		/// <param name="swapChainBufferCount">The amount of buffers included in the swap chain (typically 2).</param>
		d3dRenderTargetViewHeap(ComPtr<DirectXDeviceType>& d3dDevice, EEngineTypes engineType, unsigned int swapChainBufferCount)
		{
			mEngineType = engineType;
			mSwapChainBufferCount = swapChainBufferCount;
			CreateRenderTargetView(d3dDevice);
		}
	private:
		/// <summary>
		/// The type of engine this render target view is for
		/// </summary>
		EEngineTypes mEngineType;

		/// <summary>
		/// The count of buffers in the swap chain. If unsure what a swap chain is, check out the link below.
		/// <see cref="https://docs.microsoft.com/en-us/windows/win32/direct3d9/what-is-a-swap-chain-"/>.
		/// </summary>
		unsigned int mSwapChainBufferCount;

		/// <summary>
		/// A reference to the render target view 
		/// </summary>
		ComPtr<HeapType> mRtvHeap;

	public:
		/// <summary>
		/// Acquires a pointer to the render target view heap. Marked as const to avoid accidental data modification within getter method.
		/// </summary>
		/// <returns>The pointer to the RTV heap</returns>
		ComPtr<HeapType> GetHeap() const
		{
			return mRtvHeap;
		}
	private:
		/// <summary>
		/// Initialises a render target view descriptor, and creates the render target view based on that descriptor.
		/// </summary>
		/// <param name="d3dDevice">The device that should be used to create the RTV from the descriptor</param>
		void CreateRenderTargetView(ComPtr<DirectXDeviceType>& d3dDevice)
		{
			HeapDescriptorType rtvHeapDesc;

			switch (mEngineType)
			{
			case EEngineTypes::DX3D12:
				Logger::GetInstance()->Write("Creating render target view for DirectX 12", ELogVerbosity::Trace);
				rtvHeapDesc.NumDescriptors = mSwapChainBufferCount;
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				rtvHeapDesc.NodeMask = 0;
				ThrowIfFailed(d3dDevice->CreateDescriptorHeap(
					&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));
				break;
			default:
				Logger::GetInstance()->Write("Attempted to create render target view for unsupported graphics API " + EnumUtilities::ToString(mEngineType), ELogVerbosity::Error);
				break;
			}
		};
	};
}