#include "RenderTexture.h"

#include "DXResource.h"
#include "../D2D/Direct2D.h"

namespace tama
{
	struct RenderTexture::Impl
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTex;
	};

	RenderTexture::RenderTexture(uint32_t width, uint32_t height)
		: m_pImpl(std::make_unique<Impl>())
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		m_pImpl->tex = DX11::CreateTexture2D(&desc, nullptr);

		D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
		rtDesc.Format = desc.Format;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Texture2D.MipSlice = 0;

		m_pImpl->RTView = DX11::CreateRenderTargetView(m_pImpl->tex.Get());

		m_srv = DX11::CreateShaderResourceView(m_pImpl->tex.Get());
		this->m_size.x = static_cast<float>(width);
		this->m_size.y = static_cast<float>(height);

		m_pImpl->DSView = DX11::CreateDepthStencilView(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT);

		m_pImpl->context = DX11::GetImmediateContext();

		m_pImpl->d2dTex = DX11::D2DManager::GetInstance()->CreateRenderTarget(m_pImpl->tex);
	}

	void RenderTexture::SetRenderTarget(bool clear) const
	{
		static ID3D11ShaderResourceView* dammy = nullptr;
		m_pImpl->context->PSSetShaderResources(0, 1, &dammy);
		m_pImpl->context->OMSetRenderTargets(1, m_pImpl->RTView.GetAddressOf(), nullptr);

		if (clear)
		{
			const float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_pImpl->context->ClearRenderTargetView(m_pImpl->RTView.Get(), clearColor);
			m_pImpl->context->ClearDepthStencilView(m_pImpl->DSView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
		}

		DX11::D2DManager::GetInstance()->SetRenderTarget(m_pImpl->d2dTex);
	}

	RenderTexture::~RenderTexture(){}
}