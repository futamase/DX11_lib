#include "Window.h"
#include <Windows.h>


#include <DirectXTK/Keyboard.h>
#include <DirectXTK/Mouse.h>
#include <DirectXTK/GamePad.h>

#pragma comment(lib, "DirectXTK")

namespace Win
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		switch (msg) {
			case WM_ACTIVATEAPP:
				DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
				DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
				break;
			case WM_MOUSEHOVER: 
			case WM_INPUT:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
				break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				// アプリ終了 
				if (wParam == VK_ESCAPE)
					::DestroyWindow(hWnd);

				DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
				break;

		//! ウィンドウの拡縮の際に呼ばれる
		case WM_SIZE:
		{
			auto width = LOWORD(lParam);
			auto height = HIWORD(lParam);
//			DXGI::Swapchain::GetInstance()->ResizeBackBuffer(width, height);
		}
			break;
			case WM_DESTROY:
				::PostQuitMessage(0);
				break;
			default:
				return ::DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0L;
	}

	Window* Window::s_instance = nullptr;

	Window::Window(const tstring& AppName, HINSTANCE hInstance, UINT width, UINT height)
		: m_hInstance{ hInstance }, m_width{ width }, m_height{ height }, m_appName(AppName)
	{
		if (s_instance)
			throw std::exception("windowが2個作られた");

		s_instance = this;

		// ウィンドウモード 
		DWORD windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU;
		
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_VREDRAW | CS_HREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(DWORD);
		wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = nullptr;
		wc.hIconSm = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = _T("MainWindow");
		wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		wc.hInstance = hInstance;
		// ウィンドウを登録
		::RegisterClassEx(&wc);

		RECT rc;
		::SetRect(&rc, 0, 0, width, height);
		// ウィンドウモードのときタイトルバーを考慮して表示領域を設定するために必要なサイズを取得する 
		::AdjustWindowRect(&rc, windowStyle, FALSE);

		// ウィンドウを作成 
		auto hWnd = ::CreateWindow(
			wc.lpszClassName,
			AppName.c_str(),
			windowStyle,
			0,
			0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			nullptr,
			nullptr,
			hInstance, 
			nullptr
			);

		if (hWnd == nullptr)
			throw(_T("CreateWindowでエラーが発生しました。ウィンドウが作成できません。"));
		
		m_hWnd = hWnd;

		::UpdateWindow(hWnd);
		::ShowWindow(hWnd, SW_SHOW);
	}

	void Window::MoveToCenter()
	{
		if (!m_hWnd) return;

		RECT	cr, wr;
		UINT	w, h;

		GetWindowRect(m_hWnd, &wr);
		GetClientRect(m_hWnd, &cr);
		w = m_width + wr.right - wr.left - cr.right + cr.left;
		h = m_height + wr.bottom - wr.top - cr.bottom + cr.top;
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			(GetSystemMetrics(SM_CXSCREEN) - w) >> 1,
			(GetSystemMetrics(SM_CYSCREEN) - h) >> 1,
			w,
			h,
			SWP_NOSIZE
			);
	}

	Window::~Window()
	{
		::UnregisterClass(_T("MainWindow"), GetModuleHandle(nullptr));
	}

	void Window::SetIcon(const tstring& fileName)
	{
		HICON hIcon = (HICON)::LoadImage(m_hInstance, fileName.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}

	BOOL Window::SetTitle(const tstring& title)
	{
		return ::SetWindowText(m_hWnd, title.c_str());
	}

	float Window::Width()
	{
		RECT rc;
		::GetClientRect(s_instance->GetHWnd(), &rc);
		return static_cast<float>(rc.right);
	}

	float Window::Height()
	{
		RECT rc;
		::GetClientRect(s_instance->GetHWnd(), &rc);
		return static_cast<float>(rc.bottom);
	}

	void Window::CursorEnable(bool enable)
	{
		::ShowCursor(enable);
	}

	/*
	BOOL Resize(UINT width, UINT height)
	{
		BOOL _tagCDirect3D::ChangeDisplayMode( long lX, long lY )
{


    HRESULT hr;
    DXGI_MODE_DESC ModeDesc;
    ZeroMemory( &ModeDesc, sizeof( ModeDesc ) );
    ModeDesc.Width = lX;
    ModeDesc.Height = lY;
    ModeDesc.RefreshRate.Numerator = 60;
    ModeDesc.RefreshRate.Denominator = 1;
    ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    hr = m_lpDXGISwapChain->ResizeTarget( &ModeDesc );
    if( FAILED( hr ) )
        return( FALSE );
    return( TRUE );
}
	}
	*/
}
