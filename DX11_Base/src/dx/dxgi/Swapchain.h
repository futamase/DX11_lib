#pragma once

#include "DXGI.h"
#include <memory>
#include <wrl.h>

namespace DXGI
{
	using Microsoft::WRL::ComPtr;

	class BackBuffer
	{
		ComPtr<ID3D11Texture2D> m_buffer;
		ComPtr<ID3D11DepthStencilView> m_DSV;
		ComPtr<ID3D11RenderTargetView> m_RTV;
	public:
		void Create(IDXGISwapChain* swapchain);
		void Clear(ID3D11DeviceContext* context);
		ID3D11RenderTargetView* GetRTV() const;
		// リサイズ時に作り直す必要がある。
		void Release();

		struct PData
		{
			ID3D11Texture2D* texture;
			ID3D11DepthStencilView* DSV;
			ID3D11RenderTargetView* RTV;
		};
		PData GetData() const;
	};


	class Swapchain
	{
		ComPtr<ID3D11DeviceContext> m_context;
		std::shared_ptr<IDXGISwapChain> m_swapchain;
		BackBuffer m_backbuffer;

	public:
		IDXGISwapChain* Get() const { return m_swapchain.get(); }
		BackBuffer* GetBackBuffer() { return &m_backbuffer; }
		void Create(HWND hWnd);

		void Begin();
		void End();

		/**@return 成否*/
		bool ChangeDisplayMode(uint32_t width, uint32_t height);

		HRESULT ResizeBackBuffer(uint32_t width, uint32_t height);

		void SetFullscreen(bool fullscreen);

	public:
		static Swapchain* GetInstance();
	private:
		Swapchain() = default;
		Swapchain(const Swapchain&) = delete;
		Swapchain& operator= (const Swapchain&) = delete;
	};
}