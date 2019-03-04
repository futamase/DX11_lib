#pragma once

#include <memory>
#include <wrl.h>
#include <string>
#include <sstream>
#include <unordered_map>

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <d3d11.h>
#include <wincodec.h>           // WIC
#include <wincodecsdk.h>        // WIC

#include <DirectXColors.h>

//#include "DWriteCustomFont\ResourceFontContext.h"


//! なぜDX11名前空間なのかというと、DX11のデバイスリソースに依存しているから
namespace DX11
{
	using Microsoft::WRL::ComPtr;

	class ID2DDrawer
	{
	public:
		virtual ID2D1RenderTarget* GetRenderer() = 0;
		virtual void CreateResource(ID2D1Factory1* factory) {};
		virtual ~ID2DDrawer(){}
	};

	class WinDrawer final : public ID2DDrawer
	{
		ComPtr<ID2D1HwndRenderTarget> m_rt;
		HWND m_hWnd;
	public:
		WinDrawer(HWND hWnd);
		void CreateResource(ID2D1Factory1* factory) override;

		ID2D1RenderTarget* GetRenderer() override { return m_rt.Get(); }
	};

	class DX11Drawer final : public ID2DDrawer
	{
		ComPtr<ID2D1DeviceContext> m_context;
		ComPtr<ID2D1Device> m_device;
		ComPtr<ID2D1Bitmap1> m_targetBitmap; // バックバッファから生成したレンダーターゲット

		ID3D11Device* m_dx11Device;
		IDXGISwapChain* m_swapchain;

	public:
		DX11Drawer(ID3D11Device* d11device, IDXGISwapChain* swapChain);
		void CreateResource(ID2D1Factory1* factory) override;
		ID2D1RenderTarget* GetRenderer() override { return m_context.Get(); }
		ID2D1DeviceContext* GetContext() { return m_context.Get(); }
		void SetRenderTarget() { m_context->SetTarget(m_targetBitmap.Get()); }
	};

	class D2DManager final
	{
		// Direct2D 描画コンポーネント
		ComPtr<ID2D1Factory1> m_factory;

		ComPtr<ID2D1HwndRenderTarget> m_rt; // HWNDから生成したレンダーターゲット

		// DirectWrite 描画コンポーネント
		ComPtr<IDWriteFactory>		m_dwriteFactory;
		ComPtr<IWICImagingFactory>	m_wicFactory;

		// リソース
		std::unordered_map<size_t, ComPtr<IDWriteTextFormat>> m_formatContainer;
		std::unordered_map<size_t, ComPtr<IDWriteTextLayout>> m_layoutContainer;
		ComPtr<ID2D1SolidColorBrush>	m_colorBrush;
		ComPtr<ID2D1RadialGradientBrush> m_gradientBrush;
		ComPtr<ID2D1DrawingStateBlock> m_stateBlock;

		std::shared_ptr<ID2DDrawer> m_drawer;

		unsigned int m_width, m_height;

	public:

		// テクスチャ試験導入
		struct Texture
		{
			ComPtr<ID2D1Bitmap> tex;
			Texture()
			{
				tex = D2DManager::GetInstance()->CreateTexture();
			}
		};

		ComPtr<ID2D1Bitmap> CreateTexture();

		void SetDrawer(const std::shared_ptr<ID2DDrawer>& drawer);
		void ResizeBuffer(IDXGISwapChain* swapchain);

		void SetRenderTarget(const ComPtr<ID2D1Bitmap1>& texture);
		void SetDefaultRenderTarget();

		/**
		*@brief テキストフォーマットを作成し、コンテナに格納する*/
		void CreateTextFormat(
			float size, 
			const std::wstring& font, 
			DWRITE_FONT_WEIGHT weight, 
			DWRITE_FONT_STYLE style, 
			DWRITE_FONT_STRETCH stretch,
			_In_ IDWriteFontCollection* fontCollection = nullptr);

		void CreateTextLayout(
			const std::wstring& text,
			const std::wstring& font,
			_Out_ IDWriteTextLayout** ppLayout
			);

		void DrawString(size_t formatKey,
			const std::wstring& text, 
			float x,
			float y,
			float width,
			float height,
			const D2D_COLOR_F& color);

		void DrawString(
			_In_ IDWriteTextLayout* layout,
			const D2D_COLOR_F& color);

		void DrawLine(
			float beginX,
			float beginY,
			float endX,
			float endY,
			float thickness,
			const D2D_COLOR_F& color);

		void DrawRect(
			float x,
			float y,
			float w,
			float h,
			float thickness,
			const D2D_COLOR_F& color);

		void FillRect(
			float x,
			float y,
			float w,
			float h,
			const D2D_COLOR_F& color);

		void DrawEllipse(
			float centerX,
			float centerY,
			float radiusX,
			float radiusY,
			float thickness,
			const D2D_COLOR_F& color);

		void FillEllipase(
			float centerX,
			float centerY,
			float radiusX,
			float radiusY,
			const D2D_COLOR_F& color);

		void DrawPolygon(
			DirectX::XMFLOAT2 points[4],
			const D2D_COLOR_F& color);

		void FillPolygon(
			DirectX::XMFLOAT2 points[4],
			const D2D_COLOR_F& color);

		void DrawTexture(ID2D1Bitmap* bitmap);

		IDWriteFactory* GetDWriteFactory() const { return m_dwriteFactory.Get(); }
		ComPtr<ID2D1Bitmap1> CreateRenderTarget(const ComPtr<ID3D11Texture2D>& texture);

	private:
		void CreateIndependenceResources();
	
		D2DManager()
		{
			this->CreateIndependenceResources();
		}
		D2DManager(const D2DManager&) = delete;
		D2DManager& operator = (const D2DManager&) = delete;

		~D2DManager()
		{
		}

	public:
		static D2DManager* GetInstance()
		{
			static D2DManager instance;
			return &instance;
		}
	};
}
