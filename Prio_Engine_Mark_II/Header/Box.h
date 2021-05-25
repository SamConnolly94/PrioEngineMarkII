#pragma once

#include <array>
#include <DirectXMath.h>
#include <memory>
#include "Vertex.h"
#include "d3dUtil.h"

namespace PrioEngineII
{
	template <class DirectXDeviceType, class CommandListType>
	class Box
	{
	private:
		std::array<Vertex, 8> vertices;
		std::array<std::uint16_t, 36> indices;
		std::unique_ptr<MeshGeometry> mBoxGeometry;
		std::string mName;
	public:
		Box(ComPtr<DirectXDeviceType> d3dDevice, ComPtr<CommandListType> commandList, std::string& name)
		{
			InitialiseVertices();
			InitialiseIndices();

			mBoxGeometry = std::make_unique<MeshGeometry>();
			mName = name;
			mBoxGeometry->Name = mName;

			// Create vertex buffer
			ThrowIfFailed(D3DCreateBlob(GetVertexBufferByteSize(), &mBoxGeometry->VertexBufferCPU));
			// Copy to appropraite area
			CopyMemory(mBoxGeometry->VertexBufferCPU->GetBufferPointer(), vertices.data(), GetVertexBufferByteSize());

			// Create index buffer
			ThrowIfFailed(D3DCreateBlob(GetIndexBufferByteSize(), &mBoxGeometry->IndexBufferCPU));
			// Copy to appropraite area
			CopyMemory(mBoxGeometry->IndexBufferCPU->GetBufferPointer(), indices.data(), GetIndexBufferByteSize());

			mBoxGeometry->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(d3dDevice.Get(), commandList.Get(), vertices.data(), GetVertexBufferByteSize(), mBoxGeometry->VertexBufferUploader);
			mBoxGeometry->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(d3dDevice.Get(), commandList.Get(), indices.data(), GetIndexBufferByteSize(), mBoxGeometry->IndexBufferUploader);

			mBoxGeometry->VertexByteStride = sizeof(Vertex);
			mBoxGeometry->VertexBufferByteSize = GetVertexBufferByteSize();
			mBoxGeometry->IndexFormat = DXGI_FORMAT_R16_UINT;
			mBoxGeometry->IndexBufferByteSize = GetIndexBufferByteSize();

			SubmeshGeometry submesh;
			submesh.IndexCount = GetIndexCount();
			submesh.StartIndexLocation = 0;
			submesh.BaseVertexLocation = 0;

			mBoxGeometry->DrawArgs["box"] = submesh;
		}


		~Box()
		{
		}

		constexpr UINT GetVertexBufferByteSize() const
		{
			return (UINT)vertices.size() * sizeof(Vertex);
		}

		constexpr UINT GetIndexCount() const
		{
			return (UINT)indices.size();
		}

		constexpr UINT GetIndexBufferByteSize() const
		{
			return GetIndexCount() * sizeof(std::uint16_t);
		}

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const
		{
			return mBoxGeometry->VertexBufferView();
		}

		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const
		{
			return mBoxGeometry->IndexBufferView();
		}
	private:

		void InitialiseIndices()
		{
			indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3,
				4, 3, 7
			};
		}

		void InitialiseVertices()
		{
			vertices =
			{
				Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
				Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
				Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
				Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
				Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
				Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
				Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
				Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
			};
		}
	};
}