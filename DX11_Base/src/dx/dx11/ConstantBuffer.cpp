#include "ConstantBuffer.h"
#include "DXResource.h"

namespace tama
{
	using Microsoft::WRL::ComPtr;

	class ConstantBufferBase::Impl
	{
		size_t m_dataSize;

		ComPtr<ID3D11Buffer> m_buffer;

		ComPtr<ID3D11DeviceContext> m_context;
	public:
		Impl(const void* initData, size_t size) 
		{
			m_dataSize = size;

			m_buffer = DX11::CreateBuffer(initData, size, 
				D3D11_CPU_ACCESS_WRITE, 
				D3D11_BIND_CONSTANT_BUFFER);

			m_context = DX11::GetImmediateContext();
		}

		void Set(const void* data, uint32_t slot, ShaderStage bindStage) const
		{
			D3D11_MAPPED_SUBRESOURCE resource;

			auto hr = m_context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			if (SUCCEEDED(hr)) {
				std::memcpy(resource.pData, data, m_dataSize);
				m_context->Unmap(m_buffer.Get(), 0);
			}

			auto buffer = m_buffer.Get();
			if(static_cast<uint8_t>(bindStage) & static_cast<uint8_t>(ShaderStage::VS))
				m_context->VSSetConstantBuffers(slot, 1, &buffer);
			if(static_cast<uint8_t>(bindStage) & static_cast<uint8_t>(ShaderStage::GS))
				m_context->GSSetConstantBuffers(slot, 1, &buffer);
			if(static_cast<uint8_t>(bindStage) & static_cast<uint8_t>(ShaderStage::PS))
				m_context->PSSetConstantBuffers(slot, 1, &buffer);
		}
	};

	ConstantBufferBase::ConstantBufferBase(const void* data, size_t size)
	{
		m_impl = std::make_unique<Impl>(data, size);
	}

	ConstantBufferBase::~ConstantBufferBase() = default;

	void ConstantBufferBase::Set(const void* data, uint32_t slot, ShaderStage bindStage) const
	{
		m_impl->Set(data, slot, bindStage);
	}


}
