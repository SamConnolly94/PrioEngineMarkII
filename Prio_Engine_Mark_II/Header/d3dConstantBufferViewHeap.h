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
	/// A wrapper for the constant buffer view heap for DirectX based engines. Will provide functionality for creating the constant buffer view,
	/// and should only require you to provide the required types.
	/// </summary>
	/// <typeparam name="DirectXDevice">The type of device used to create constant buffer views</typeparam>
	/// <typeparam name="HeapType">The type of the heap that will be used to store the constant buffer view</typeparam>
	/// <typeparam name="HeapDescriptorType">The type of the heap descriptor that will be used to create the heap for the constant buffer view</typeparam>
	template <class DirectXDeviceType, class HeapType, class HeapDescriptorType>
	class d3dConstantBufferViewHeap
	{
	public:
		/// <summary>
		/// Constructor which will initialise the constant buffer view
		/// </summary>
		/// <param name="d3dDevice">The pointer to the Direct X device</param>
		/// <param name="engineType">The type of engine (to work out which version of Direct X we should create the CBV for).</param>
		d3dConstantBufferViewHeap(ComPtr<DirectXDeviceType>& d3dDevice, EEngineTypes engineType)
		{
			mEngineType = engineType;
			CreateConstantBufferView(d3dDevice);
		}
	private:
		/// <summary>
		/// The type of engine this constant buffer view is for
		/// </summary>
		EEngineTypes mEngineType;

		/// <summary>
		/// A reference to the constant buffer view 
		/// </summary>
		ComPtr<HeapType> mCbvHeap;

	public:
		/// <summary>
		/// Acquires a pointer to the constant buffer view heap. Marked as const to avoid accidental data modification within getter method.
		/// </summary>
		/// <returns>The pointer to the CBV heap</returns>
		ComPtr<HeapType> GetHeap() const
		{
			return mCbvHeap;
		}
	private:
		/// <summary>
		/// Initialises a constant buffer view descriptor, and creates the constant buffer view based on that descriptor.
		/// </summary>
		/// <param name="d3dDevice">The device that should be used to create the CBV from the descriptor</param>
		void CreateConstantBufferView(ComPtr<DirectXDeviceType>& d3dDevice)
		{
			D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;

			switch (mEngineType)
			{
			case EEngineTypes::DX3D12:
				Logger::GetInstance()->Write("Creating constant buffer view for DirectX 12", ELogVerbosity::Trace);
				cbvHeapDesc.NumDescriptors = 1;
				cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				cbvHeapDesc.NodeMask = 0;
				ThrowIfFailed(d3dDevice->CreateDescriptorHeap(&cbvHeapDesc, 
					IID_PPV_ARGS(&mCbvHeap)));
				break;
			default:
				Logger::GetInstance()->Write("Attempted to create constant buffer view for unsupported graphics API " + EnumUtilities::ToString(mEngineType), ELogVerbosity::Error);
				break;
			}
		};
	};
}