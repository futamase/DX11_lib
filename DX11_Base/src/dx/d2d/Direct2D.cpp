#include "Direct2D.h"
//#include "D2DString.h"

#include <stdexcept>
//#include "../../../resource.h"
//#include "DWriteCustomFont\ResourceFontFileLoader.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "WindowsCodecs")
#pragma comment(lib, "dwrite")

namespace DX11
{
	ComPtr<ID2D1Bitmap1> CreateRenderTarget(ID2D1DeviceContext* context, IDXGISwapChain* swapchain);
	ComPtr<ID2D1Bitmap1> CreateRenderTarget(ID2D1DeviceContext* context, const ComPtr<ID3D11Texture2D>& texture);

	WinDrawer::WinDrawer(HWND hWnd)
		: m_hWnd(hWnd)
	{}

	void WinDrawer::CreateResource(ID2D1Factory1* factory)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		// Create a Direct2D render target			
		HRESULT hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				m_hWnd,
				D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top)
			),
			&m_rt
		);
	}

	DX11Drawer::DX11Drawer(ID3D11Device* d11device, IDXGISwapChain* swapChain)
		: m_dx11Device(d11device)
		, m_swapchain(swapChain)
	{}

	void DX11Drawer::CreateResource(ID2D1Factory1* factory)
	{
		IDXGIDevice* dxgiDevice;
		if (FAILED(m_dx11Device->QueryInterface(
			IID_PPV_ARGS(&dxgiDevice))))
			throw std::exception("DXGIデバイスの取得に失敗");

		if (FAILED(factory->CreateDevice(dxgiDevice, m_device.GetAddressOf())))
			throw std::runtime_error("Direct2Dデバイスの生成に失敗");

		dxgiDevice->Release();

		if (FAILED(m_device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			m_context.GetAddressOf())))
			throw std::runtime_error("Direct2Dデバイスコンテキストの生成に失敗");

		m_targetBitmap = CreateRenderTarget(m_context.Get(), m_swapchain);

		m_context->SetTarget(m_targetBitmap.Get());

		// すべての Windowsアプリで、グレースケール テキストのアンチエイリアシングをお勧めします。
		m_context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	}

	void D2DManager::CreateIndependenceResources()
	{
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_MULTI_THREADED,
			__uuidof(ID2D1Factory1),
			nullptr,
			reinterpret_cast<void**>(m_factory.GetAddressOf()))))
			throw std::runtime_error("Direct2Dファクトリの生成に失敗");

		if (FAILED(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(m_dwriteFactory.GetAddressOf()))))
			throw std::runtime_error("DirectWriteファクトリの生成に失敗");

		if (FAILED(m_factory->CreateDrawingStateBlock(m_stateBlock.GetAddressOf())))
			throw std::runtime_error("ドローイングステートブロックの生成に失敗");

		// Windows Imaging Component (WIC) ファクトリを初期化します。
//		if (FAILED(CoCreateInstance(
//			CLSID_WICImagingFactory,
//			NULL,
//			CLSCTX_INPROC_SERVER,
//			__uuidof(IWICImagingFactory),
//			reinterpret_cast<void**>(m_wicFactory.GetAddressOf())
//		)))
//			throw std::runtime_error("WICファクトリの生成に失敗");
	}

	ComPtr<ID2D1Bitmap> D2DManager::CreateTexture()
	{
		// PNGの読込み
		// ファイルからデコーダを作成
		ComPtr<IWICBitmapDecoder> dec;
		HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
			L"noise.png",
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&dec);

		// フレーム取得
		ComPtr<IWICBitmapFrameDecode> frame;
		hr = dec->GetFrame(0, &frame);

		// コンバータでDirect2D用フォーマットに変換
		ComPtr<IWICFormatConverter> converter;
		hr = m_wicFactory->CreateFormatConverter(&converter);

		hr = converter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut);

		// Direct2D用ビットマップを作成
		ComPtr<ID2D1Bitmap> bitmap;
		hr = m_drawer->GetRenderer()->CreateBitmapFromWicBitmap(converter.Get(), nullptr, &bitmap);

		return bitmap;
	}

	void D2DManager::SetDrawer(const std::shared_ptr<ID2DDrawer>& drawer)
	{
		m_drawer = drawer;
		m_drawer->CreateResource(m_factory.Get());

		m_drawer->GetRenderer()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_colorBrush);
	}

	//	void D2DManager::CreateResources(ID3D11Device* d11device, IDXGISwapChain* swapChain)
	//	{
			//		m_fontContext.SetDwriteFactory(m_dwriteFactory.Get());
			//		m_fontContext.Initialize();

			//		uint32_t key[] = { 101 };
			//		IDWriteFontCollection* col;
			//		m_fontContext.CreateFontCollection(key, sizeof(key), &col);
			//		g = std::shared_ptr<IDWriteFontCollection>(col, [](IDWriteFontCollection* p)
			//		{
			//			p->Release(); 
			//		});

					// #########################################
			//		ComPtr<IDWriteFontCollection> collection;
			//		m_dwriteFactory->GetSystemFontCollection(&collection);
			//		size_t familyCount;
			//		familyCount = collection->GetFontFamilyCount();

			//		// コレクション内のすべてのフォント ファミリを繰り返し処理します。
			//		for (size_t familyIndex = 0; familyIndex < familyCount; ++familyIndex)
			//		{
			//			// フォント ファミリ オブジェクトを取得します。
			//			ComPtr<IDWriteFontFamily> fontFamily;
			//			HRESULT	hr = collection->GetFontFamily(familyIndex, &fontFamily);
			//			if (FAILED(hr))
			//				throw std::exception("hoge");

			//			// フォント ファミリ名を取得します。
			//			ComPtr<IDWriteLocalizedStrings> familyNames;
			//			hr = fontFamily->GetFamilyNames(&familyNames);

			//			// 現在のロケールの名前を取得します。
			//	//		std::wstring familyName = GetDisplayName(familyNames);
			//		}
					//#########################################
	//	}

	void D2DManager::CreateTextFormat(float size, const std::wstring& font, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, _In_ IDWriteFontCollection* fontCollection)
	{
		ComPtr<IDWriteTextFormat> format;
		if (FAILED(m_dwriteFactory->CreateTextFormat(
			font.c_str(),// フォント（ＭＳ Ｐ明朝/ＭＳ Ｐゴシックとか）
			fontCollection, // フォントコレクション（NULL=システムフォント）
			weight, //太さ
			style, //スタイル(傾き)
			stretch,//幅
			size,//文字の大きさ
			L"",//ロケール(地域)/ja-jp/en-us
			format.GetAddressOf()
		)))
			throw std::runtime_error("テキストフォーマットの生成に失敗");

		if (FAILED(format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)))
			throw std::runtime_error("アライメントの設定に失敗");

		//!TODO enum DWRITE_TEXT_ALIGNMENT で左寄り、中央、右寄りの設定ができる。外部から変更できるようにしたい
		if (FAILED(format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)))
			throw std::runtime_error("テキストアライメントの設定に失敗");

		auto hash = std::hash<float>()(size) ^ std::hash<std::wstring>()(font);

		m_formatContainer.emplace(hash, format);

		ComPtr<IDWriteTextLayout> layout;
		if (FAILED(m_dwriteFactory->CreateTextLayout(
			L"hoge",
			4,
			m_formatContainer.at(hash).Get(),
			100, // 入力テキストの最大幅。
			10, // 入力テキストの最大高さ。
			&layout
		)))
			throw std::runtime_error("レイアウトの生成に失敗");

		m_layoutContainer.emplace(hash, layout);
	}
	void D2DManager::CreateTextLayout(const std::wstring& text, const std::wstring& font, _Out_ IDWriteTextLayout** ppLayout)
	{
		auto hash = std::hash<std::wstring>()(font);

		if (FAILED(m_dwriteFactory->CreateTextLayout(
			text.c_str(),
			(UINT32)text.size(),
			m_formatContainer.at(hash).Get(),
			800, // 入力テキストの最大幅。
			600, // 入力テキストの最大高さ。
			ppLayout
		)))
			throw std::runtime_error("レイアウトの生成に失敗");
	}

	void D2DManager::DrawString(
		size_t formatKey,
		const std::wstring& text,
		float x, float y, float width, float height, const D2D_COLOR_F& color)
	{
		ComPtr<IDWriteTextLayout> layout;
		if (FAILED(m_dwriteFactory->CreateTextLayout(
			text.c_str(),
			(UINT32)text.size(),
			m_formatContainer.at(formatKey).Get(),
			width, // 入力テキストの最大幅。
			height, // 入力テキストの最大高さ。
			&layout
		)))
			throw std::runtime_error("レイアウトの生成に失敗");

		//		layout = m_layoutContainer.at(formatKey).Get();

		m_colorBrush->SetColor(color);

		m_drawer->GetRenderer()->SaveDrawingState(m_stateBlock.Get());
		m_drawer->GetRenderer()->BeginDraw();

		//		m_context->DrawTextW(L"hoge", 4, m_formatContainer.at(formatKey).Get(),
		//		{ 100, 100, 200, 200 }, m_colorBrush.Get());
		m_drawer->GetRenderer()->DrawTextLayout(
			D2D1::Point2F(x, y),
			layout.Get(),
			m_colorBrush.Get()
		);

		//D2DERR_RECREATE_TARGET をここで無視します。このエラーは、デバイスが失われたことを示します。
		// これは、Present に対する次回の呼び出し中に処理されます。
		HRESULT hr = m_drawer->GetRenderer()->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)) throw std::runtime_error("何か失敗");
		}

		m_drawer->GetRenderer()->RestoreDrawingState(m_stateBlock.Get());
	}
	void D2DManager::DrawString(_In_ IDWriteTextLayout* layout, const D2D_COLOR_F& color)
	{

	}

	void D2DManager::DrawLine(float beginX, float beginY, float endX, float endY, float thickness, const D2D_COLOR_F& color)
	{
		m_colorBrush->SetColor(color);

		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->DrawLine(
			D2D1::Point2F(beginX, beginY),
			D2D1::Point2F(endX, endY),
			m_colorBrush.Get(),
			thickness);
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::DrawRect(float x, float y, float w, float h, float thickness, const D2D_COLOR_F& color)
	{
		m_colorBrush->SetColor(color);

		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->DrawRectangle(D2D1::RectF(x, y, x + w, y + h),
			m_colorBrush.Get(), thickness);
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::FillRect(float x, float y, float w, float h, const D2D_COLOR_F& color)
	{
		m_colorBrush->SetColor(color);
		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->FillRectangle(D2D1::RectF(x, y, x + w, y + h), m_colorBrush.Get());
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::DrawEllipse(float centerX, float centerY, float radiusX, float radiusY, float thickness, const D2D_COLOR_F& color)
	{
		m_colorBrush->SetColor(color);

		m_drawer->GetRenderer()->BeginDraw();
		D2D1_ELLIPSE e;
		e.point.x = centerX;
		e.point.y = centerY;
		e.radiusX = radiusX;
		e.radiusY = radiusY;
		m_drawer->GetRenderer()->DrawEllipse(e, m_colorBrush.Get(), thickness);
		m_drawer->GetRenderer()->EndDraw();
	}
	void D2DManager::FillEllipase(float centerX, float centerY, float radiusX, float radiusY, const D2D_COLOR_F& color)
	{
		m_colorBrush->SetColor(color);

		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->FillEllipse(D2D1::Ellipse(D2D1::Point2(centerX, centerY), radiusX, radiusY),
			m_colorBrush.Get());
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::DrawPolygon(DirectX::XMFLOAT2 points[4], const D2D_COLOR_F& color)
	{
		static ID2D1PathGeometry* geometry;

		HRESULT hr = m_factory->CreatePathGeometry(&geometry);

		if (SUCCEEDED(hr))
		{
			ComPtr<ID2D1GeometrySink> pSink;

			hr = geometry->Open(&pSink);
			if (SUCCEEDED(hr))
			{
				pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

				pSink->BeginFigure(
					D2D1::Point2F(points[0].x, points[0].y),
					D2D1_FIGURE_BEGIN_FILLED
				);
				D2D1_POINT_2F p[] = {
				   D2D1::Point2F(points[1].x, points[1].y),
				   D2D1::Point2F(points[2].x, points[2].y),
				   D2D1::Point2F(points[3].x, points[3].y),
				   D2D1::Point2F(points[0].x, points[0].y),
				};
				pSink->AddLines(p, ARRAYSIZE(p));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			}
			hr = pSink->Close();
		}

		m_colorBrush->SetColor(color);
		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->DrawGeometry(geometry, m_colorBrush.Get());
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::FillPolygon(DirectX::XMFLOAT2 points[4], const D2D_COLOR_F& color)
	{
		static ID2D1PathGeometry* geometry;

		HRESULT hr = m_factory->CreatePathGeometry(&geometry);

		if (SUCCEEDED(hr))
		{
			ComPtr<ID2D1GeometrySink> pSink;

			hr = geometry->Open(&pSink);
			if (SUCCEEDED(hr))
			{
				pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

				pSink->BeginFigure(
					D2D1::Point2F(points[0].x, points[0].y),
					D2D1_FIGURE_BEGIN_FILLED
				);
				D2D1_POINT_2F p[] = {
				   D2D1::Point2F(points[1].x, points[1].y),
				   D2D1::Point2F(points[2].x, points[2].y),
				   D2D1::Point2F(points[3].x, points[3].y),
				   D2D1::Point2F(points[0].x, points[0].y),
				};
				pSink->AddLines(p, ARRAYSIZE(p));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			}
			hr = pSink->Close();
		}

		m_colorBrush->SetColor(color);
		m_drawer->GetRenderer()->BeginDraw();
		m_drawer->GetRenderer()->FillGeometry(geometry, m_colorBrush.Get());
		m_drawer->GetRenderer()->EndDraw();
	}


	void D2DManager::DrawTexture(ID2D1Bitmap* bitmap)
	{
		m_colorBrush->SetColor(D2D1::ColorF(1, 0, 0, 1));

		const auto rc = D2D1::RectF(0, 0, 100, 100);
		m_drawer->GetRenderer()->BeginDraw();

		m_drawer->GetRenderer()->SetTransform(D2D1::Matrix3x2F::Translation(100, 100));

		m_drawer->GetRenderer()->DrawBitmap(
			bitmap,
			nullptr,
			0.5f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			nullptr);

		m_drawer->GetRenderer()->SetTransform(D2D1::Matrix3x2F::Identity());
		m_drawer->GetRenderer()->EndDraw();
	}

	void D2DManager::ResizeBuffer(IDXGISwapChain* swapchain)
	{
		//		m_targetBitmap = CreateRenderTarget(m_rt.Get(), swapchain);

		//		m_context->SetTarget(m_targetBitmap.Get());
	}

	void D2DManager::SetRenderTarget(const ComPtr<ID2D1Bitmap1>& texture)
	{
		auto renderer = dynamic_cast<DX11Drawer*>(m_drawer.get());

		renderer->GetContext()->SetTarget(texture.Get());
	}

	void D2DManager::SetDefaultRenderTarget()
	{
		auto renderer = dynamic_cast<DX11Drawer*>(m_drawer.get());

		if (!renderer)
			return;

		renderer->SetRenderTarget();
	}

	ComPtr<ID2D1Bitmap1> CreateRenderTarget(ID2D1DeviceContext* context, IDXGISwapChain* swapchain)
	{
		// スワップ チェーン バック バッファーに関連付けられた Direct2D ターゲット ビットマップを作成し、
		// それを現在のターゲットとして設定します。
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);

		ComPtr<IDXGISurface> dxgiBackBuffer;
		if (FAILED(swapchain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))))
			throw std::runtime_error("バックバッファの取得に失敗");

		ComPtr<ID2D1Bitmap1> bitmap;
		if (FAILED(context->CreateBitmapFromDxgiSurface(
			dxgiBackBuffer.Get(),
			bitmapProperties,
			bitmap.GetAddressOf()
		)))
			throw std::runtime_error("Direct2Dビットマップの生成に失敗");

		return bitmap;
	}

	ComPtr<ID2D1Bitmap1> D2DManager::CreateRenderTarget(const ComPtr<ID3D11Texture2D>& texture)
	{
		const float dxgiDpi = 96.0f;
		auto context = dynamic_cast<DX11Drawer*>(m_drawer.get())->GetContext();

		context->SetDpi(dxgiDpi, dxgiDpi);

		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dxgiDpi, dxgiDpi
			);

		ComPtr<ID2D1Bitmap1> cubeTextureTarget;
		ComPtr<IDXGISurface> cubeTextureSurface;
		if (FAILED(texture.As(&cubeTextureSurface)))
			throw std::runtime_error("ID2D1Bitmap1への変換に失敗");

		if (FAILED(context->CreateBitmapFromDxgiSurface(
			cubeTextureSurface.Get(),
			&bitmapProperties,
			&cubeTextureTarget)))
			throw std::runtime_error("ビットマップの生成に失敗");

		return cubeTextureTarget;
	}
}

#if 0
Direct2D::Direct2D() = default;

void Direct2D::CreateResources()
{
	ID2D1Factory1* d2dfactory;
	if (FAILED(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		__uuidof(ID2D1Factory1),
		nullptr,
		reinterpret_cast<void**>(&d2dfactory)
	)
	))
		throw std::runtime_error("Direct2Dファクトリの生成に失敗");
	m_d2dFactory = std::shared_ptr<ID2D1Factory1>(d2dfactory, [](IUnknown*p) { p->Release(); });

	IDWriteFactory* dwfactory;
	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&dwfactory)
	)
	))
		throw std::runtime_error("DirectWriteファクトリの生成に失敗");
	m_dwriteFactory = std::shared_ptr<IDWriteFactory>(dwfactory, [](IUnknown*p) { p->Release(); });

	// Windows Imaging Component (WIC) ファクトリを初期化します。
	//IWICImagingFactory* wicfactory;
	//if (FAILED(CoCreateInstance(
	//	CLSID_WICImagingFactory,
	//	nullptr,
	//	CLSCTX_INPROC_SERVER,
	//	IID_PPV_ARGS(&wicfactory)
	//	)
	//	))
	//	throw std::runtime_error("WICファクトリの生成に失敗");
	//m_wicFactory = std::shared_ptr<IWICImagingFactory>(wicfactory, [](IWICImagingFactory*p) { p->Release(); });

	IDWriteTextFormat* format;
	if (FAILED(m_dwriteFactory->CreateTextFormat(
		L"Gabriola", // フォント（ＭＳ Ｐ明朝/ＭＳ Ｐゴシックとか）
		nullptr, // フォントコレクション（NULL=システムフォント）
		DWRITE_FONT_WEIGHT_NORMAL, //太さ
		DWRITE_FONT_STYLE_NORMAL, //スタイル(傾き)
		DWRITE_FONT_STRETCH_NORMAL,//幅
		30.0f,//文字の大きさ
		L"en-us",//ロケール(地域)/ja-jp/en-us
		&format
	)))
		throw std::runtime_error("テキストフォーマットの生成に失敗");
	m_textFormat = std::shared_ptr<IDWriteTextFormat>(format, [](IUnknown*p) { p->Release(); });

	if (FAILED(m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)))
		throw std::runtime_error("アライメントの設定に失敗");

	ID2D1DrawingStateBlock* stateBlock;
	if (FAILED(m_d2dFactory->CreateDrawingStateBlock(&stateBlock)))
		throw std::runtime_error("ドローイングステートブロックの生成に失敗");
	m_stateBlock = std::shared_ptr<ID2D1DrawingStateBlock>(stateBlock, [](IUnknown*p) { p->Release(); });

}


void Direct2D::CreateDependentResource(ID3D11Device* d11device, IDXGISwapChain* swapChain)
{
	IDXGIDevice* dxgiDevice;
	d11device->QueryInterface(
		__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(&dxgiDevice));
	if (!dxgiDevice) throw std::exception("DXGIデバイスの取得に失敗");

	ID2D1Device* d2ddevice;
	if (FAILED(m_d2dFactory->CreateDevice(dxgiDevice, &d2ddevice)))
		throw std::runtime_error("Direct2Dデバイスの生成に失敗");
	m_d2dDevice = std::shared_ptr<ID2D1Device>(d2ddevice, [](IUnknown*p) { p->Release(); });

	dxgiDevice->Release();

	ID2D1DeviceContext* d2dcontext;
	if (FAILED(m_d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&d2dcontext)))
		throw std::runtime_error("Direct2Dデバイスコンテキストの生成に失敗");
	m_d2dContext = std::shared_ptr<ID2D1DeviceContext>(d2dcontext, [](IUnknown*p) { p->Release(); });

	// スワップ チェーン バック バッファーに関連付けられた Direct2D ターゲット ビットマップを作成し、
	// それを現在のターゲットとして設定します。
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	IDXGISurface* dxgiBackBuffer;
	if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))))
		throw std::runtime_error("バックバッファの取得に失敗");

	ID2D1Bitmap1* bitmap;
	if (FAILED(m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer,
		bitmapProperties,
		&bitmap
	)))
		throw std::runtime_error("Direct2Dビットマップの生成に失敗");
	m_d2dTargetBitmap = std::shared_ptr<ID2D1Bitmap1>(bitmap, [](IUnknown*p) { p->Release(); });

	m_d2dContext->SetTarget(m_d2dTargetBitmap.get());

	// すべての Windowsアプリで、グレースケール テキストのアンチエイリアシングをお勧めします。
	m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	dxgiBackBuffer->Release();

	ID2D1SolidColorBrush* brush;
	m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	m_colorBrush = std::shared_ptr<ID2D1SolidColorBrush>(brush, [](IUnknown* p) { p->Release(); });
}

void Direct2D::DrawRect(float x, float y, float w, float h)
{
	m_d2dContext->BeginDraw();
	m_d2dContext->DrawRectangle(D2D1::RectF(x, y, x + w, y + h),
		m_colorBrush.get(), 10);
	//		m_d2dContext->FillRectangle()
	m_d2dContext->EndDraw();
}

void Direct2D::DrawLine(float beginX, float beginY, float endX, float endY,
	const D2D_COLOR_F& color, float stroke)
{
	m_colorBrush->SetColor(color);

	m_d2dContext->BeginDraw();
	m_d2dContext->DrawLine(
		D2D1::Point2F(beginX, beginY),
		D2D1::Point2F(endX, endY),
		m_colorBrush.get(),
		stroke);
	m_d2dContext->EndDraw();
}

void Direct2D::DrawString(const std::wstring& text, float x, float y, const D2D_COLOR_F& color)
{
	IDWriteTextLayout* layout;
	if (FAILED(m_dwriteFactory->CreateTextLayout(
		text.c_str(),
		(uint32_t)text.size(),
		m_textFormat.get(),
		240.0f, // 入力テキストの最大幅。
		50.0f, // 入力テキストの最大高さ。
		&layout
	)))
		throw std::runtime_error("レイアウトの生成に失敗");

	m_colorBrush->SetColor(color);

	m_d2dContext->SaveDrawingState(m_stateBlock.get());
	m_d2dContext->BeginDraw();

	if (FAILED(m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)))
		throw std::runtime_error("テキストアライメントの設定に失敗");

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(x, y),
		layout,
		m_colorBrush.get()
	);

	//D2DERR_RECREATE_TARGET をここで無視します。このエラーは、デバイスが失われたことを示します。
	// これは、Present に対する次回の呼び出し中に処理されます。
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		if (FAILED(hr)) throw std::runtime_error("何か失敗");
	}

	m_d2dContext->RestoreDrawingState(m_stateBlock.get());
}

//要修正
void Direct2D::DrawString(const D2D::String& text)
{
	//		this->DrawString(text.text, text.pos.x, text.pos.y, text.color);
}

void Direct2D::DrawString(const D2D::String& text, float x, float y)
{
	//		this->DrawString(text.text, x, y, text.color);
}

void Direct2D::CreateRenderTargetTexture(ID3D11Texture2D* d11texture)
{
	IDXGISurface* dxgiSurface;
	if (FAILED(d11texture->QueryInterface(
		__uuidof(IDXGISurface),
		reinterpret_cast<void**>(&dxgiSurface))))
		throw std::runtime_error("DXGIサーフェスの生成に失敗");

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	ID2D1Bitmap1* bitmap;
	if (FAILED(m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiSurface,
		bitmapProperties,
		&bitmap)))
		throw std::runtime_error("ビットマップの生成に失敗");

	//描画対象を変更する関数を作成する必要あり
	m_d2dContext->SetTarget(bitmap);
	}
#endif	
