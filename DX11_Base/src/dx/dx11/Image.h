#pragma once

#include "DXResource.h"

namespace tama
{
	class Texture;

	// ‘‚«‚İ‰Â”\‚È‰æ‘œ
	class Image
	{
		friend class Texture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
		BYTE* m_pData = nullptr;
	public:
		Image(uint32_t width = 50, uint32_t height = 50)
		{
			m_tex = DX11::CreateTexture2D(&CD3D11_TEXTURE2D_DESC(
				DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
				width, height, 1, 1,
				D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE),
				nullptr);

			auto context = DX11::GetImmediateContext();
			D3D11_MAPPED_SUBRESOURCE resource;
			if (SUCCEEDED(context->Map(m_tex.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
			{
				m_pData = (BYTE*)resource.pData;
				for (unsigned int i = 0; i < width * height; i++)
				{
					m_pData[i * 4] = 0;
					m_pData[i * 4 + 1] = 0;
					m_pData[i * 4 + 2] = 0;
					m_pData[i * 4 + 3] = 255;
				}

				context->Unmap(m_tex.Get(), 0);
			}
		}

		BYTE* GetData() const 
		{
			return m_pData;
		}
	};
}