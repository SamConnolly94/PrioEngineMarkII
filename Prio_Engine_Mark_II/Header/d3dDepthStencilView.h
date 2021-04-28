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
	/// A wrapper for the depth stencil view for DirectX based engines. Will provide functionality for creating the depth stencil view,
	/// and should only require you to provide the required types.
	/// </summary>
	/// <typeparam name="DirectXDevice">The type of device used to create depth stencil views</typeparam>
	/// <typeparam name="HeapType">The type of the heap that will be used to store the depth stencil view</typeparam>
	/// <typeparam name="HeapDescriptorType">The type of the heap descriptor that will be used to create the heap for the depth stencil view</typeparam>
	template <class DirectXDevice, class HeapType, class HeapDescriptorType>
	class d3dDepthStencilView
	{
	public:
		/// <summary>
		/// Constructor which will initialise the depth stencil view
		/// </summary>
		/// <param name="d3dDevice">The pointer to the Direct X device</param>
		/// <param name="engineType">The type of engine (to work out which version of Direct X we should create the RTV for.</param>
		d3dDepthStencilView(ComPtr<DirectXDevice>& d3dDevice, EEngineTypes engineType)
		{
			mEngineType = engineType;
			CreateDepthStencilView(d3dDevice);
		}
	private:
		/// <summary>
		/// The type of engine this depth stencil view is for
		/// </summary>
		EEngineTypes mEngineType;

		/// <summary>
		/// A reference to the depth stencil view 
		/// </summary>
		ComPtr<HeapType> mDsvHeap;

	public:
		/// <summary>
		/// Acquires a pointer to the depth stencil view heap. Marked as const to avoid accidental data modification within getter method.
		/// </summary>
		/// <returns>The pointer to the RTV heap</returns>
		ComPtr<HeapType> GetHeap() const
		{
			return mDsvHeap;
		}
	private:
		/// <summary>
		/// Initialises a depth stencil view descriptor, and creates the depth stencil view based on that descriptor.
		/// </summary>
		/// <param name="d3dDevice">The device that should be used to create the DSV from the descriptor</param>
		void CreateDepthStencilView(ComPtr<DirectXDevice>& d3dDevice)
		{
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;

			switch (mEngineType)
			{
			case EEngineTypes::DX3D12:
				Logger::GetInstance()->Write("Creating depth stencil view for DirectX 12", ELogVerbosity::Trace);
				dsvHeapDesc.NumDescriptors = 1;
				dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				dsvHeapDesc.NodeMask = 0;
				ThrowIfFailed(d3dDevice->CreateDescriptorHeap(
					&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
				break;
			default:
				Logger::GetInstance()->Write("Attempted to create depth stencil view for unsupported graphics API " + EnumUtilities::ToString(mEngineType), ELogVerbosity::Error);
				break;
			}
		};
	};
}