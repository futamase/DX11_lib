#include "Swapchain.h"
#include "../DX11/DX11device.h"
#include "../DX11/DXResource.h"
#include <stdexcept>
#include <DirectXColors.h>

namespace DXGI
{
	void BackBuffer::Create(IDXGISwapChain* swapchain)
	{
		m_buffer = DX11::GetBackBuffer(swapchain);

		D3D11_TEXTURE2D_DESC desc;
		m_buffer->GetDesc(&desc);
		m_DSV = DX11::CreateDepthStencilView(desc.Width, desc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT);

		m_RTV = DX11::CreateRenderTargetView(m_buffer.Get());
	}

	void BackBuffer::Clear(ID3D11DeviceContext* context)
	{
		auto prtv = m_RTV.Get();
		context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
		context->ClearRenderTargetView(m_RTV.Get(), DirectX::Colors::Gray);
		context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}

	ID3D11RenderTargetView* BackBuffer::GetRTV() const
	{
		return m_RTV.Get(); 
	}

	void BackBuffer::Release()
	{
		m_buffer.Reset();
		m_DSV.Reset();
		m_RTV.Reset();
	}

	BackBuffer::PData BackBuffer::GetData() const
	{
		PData data;
		data.texture = this->m_buffer.Get();
		data.RTV = this->m_RTV.Get();
		data.DSV = this->m_DSV.Get();

		return data;
	}

	Swapchain* Swapchain::GetInstance()
	{
		static Swapchain instance;
		return &instance;
	}

	void Swapchain::Create(HWND hWnd)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		DXGI_SWAP_CHAIN_DESC chain_desc;
		ZeroMemory(&chain_desc, sizeof(chain_desc));
		chain_desc.BufferCount = 1;
		chain_desc.BufferDesc.Width = rc.right;
		chain_desc.BufferDesc.Height = rc.bottom;
		chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		chain_desc.BufferDesc.RefreshRate = { 60, 1 };
		chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		chain_desc.SampleDesc = { 1,0 };
		chain_desc.OutputWindow = hWnd;
		chain_desc.Windowed = TRUE;
		chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swapchain = nullptr;
		auto pFactory = DXGI::AccessGIFactory(DX11::Device::Get());
		auto hr = pFactory->CreateSwapChain(
			DX11::Device::Get(),
			&chain_desc,
			&swapchain);

		pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

		if (FAILED(hr))
			throw std::runtime_error("スワップチェインの作成に失敗");

		m_swapchain = std::shared_ptr<IDXGISwapChain>(swapchain, [](IDXGISwapChain* p)
		{
			BOOL isFullScreen;
			p->GetFullscreenState(&isFullScreen, nullptr);
			if (isFullScreen == TRUE) 
				p->SetFullscreenState(FALSE, nullptr);
			p->Release();
		});

		m_context = DX11::GetImmediateContext();

		m_backbuffer.Create(m_swapchain.get());
	}

	void Swapchain::Begin()
	{

		m_backbuffer.Clear(m_context.Get());
	}

	void Swapchain::End()
	{
		m_swapchain->Present(0, 0);
	}

	bool Swapchain::ChangeDisplayMode(uint32_t width, uint32_t height)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		m_swapchain->GetDesc(&desc);

	    HRESULT hr;
	    DXGI_MODE_DESC ModeDesc;
	    ZeroMemory(&ModeDesc, sizeof(ModeDesc));
	    ModeDesc.Width = width;
	    ModeDesc.Height = height;
	    ModeDesc.RefreshRate.Numerator = 60;
	    ModeDesc.RefreshRate.Denominator = 1;
	    ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	    ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	    hr = m_swapchain->ResizeTarget(&desc.BufferDesc);
	    
		return SUCCEEDED(hr);
	}

	HRESULT Swapchain::ResizeBackBuffer(uint32_t width, uint32_t height)
	{
		// 最初にウィンドウが作成されるときも呼ばれるから必要
		if (!m_context) return S_FALSE;

	    m_context->OMSetRenderTargets( 0, 0, 0 );

		m_backbuffer.Release();

		HRESULT hr = m_swapchain->ResizeBuffers(
			1,
			width,
			height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	    
		m_backbuffer.Create(m_swapchain.get());

	    return hr;
	}

	void Swapchain::SetFullscreen(bool fullscreen)
	{
		if (fullscreen)
			m_swapchain->SetFullscreenState(TRUE, 0);
		else {
			m_swapchain->SetFullscreenState(FALSE, 0);
//			this->ChangeDisplayMode(256,256);
		}
	}
}