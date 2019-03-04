#pragma once
#include "DXResource.h"

namespace DX11
{
	struct MRTResource
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex2D;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRView;
	};

	void CreateMRT(uint32_t width, uint32_t height);

	class HOGE
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dsrv, m_ssrv;

	};
}

