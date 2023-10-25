#pragma once

#include <engine/rendering/helper/d3d12util.h>

template <typename T>
class CUploadBuffer
{
public:
    CUploadBuffer::CUploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : m_IsConstantBuffer(isConstantBuffer)
    {
        m_ElementByteSize = sizeof(T);

        // Constant buffer elements always have to be in multiples of 256 bytes
        // This is because hardware can only view at 256 byte offsets
        if (isConstantBuffer)
        {
            m_ElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));
        }

        PrioEngine::ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * elementCount),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_UploadBuffer)
        ));

        PrioEngine::ThrowIfFailed(&m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
        
        // We can't write to the resource for as long as the resource is mapped. 
    }
    CUploadBuffer(const CUploadBuffer& rhs) = delete;
    CUploadBuffer& operator=(const CUploadBuffer& rhs) = delete;
    ~CUploadBuffer()
    {
        if (m_UploadBuffer)
        {
            m_UploadBuffer->Unmap(0, nullptr);
        }
        m_MappedData = nullptr;
    }

    ID3D12Resource* Resource() const
    {
        return m_UploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;
    bool m_IsConstantBuffer{ false };
    UINT m_ElementByteSize{ 0 };
    BYTE* m_MappedData{ nullptr };
};