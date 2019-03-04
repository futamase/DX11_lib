#include <crtdbg.h>
#include "Window.h"
#include "DebugWindow.h"
#include "../dx/ComInitializer.h"
#include "../dx/d2d/Direct2D.h"
#include "../dx/d2d/D2DString.h"
#include "../dx/dxgi/Swapchain.h"
#include "../dx/dx11/DX11Device.h"
#include "../dx/dx11/DXResource.h"

#include "../system/Input.h"

void Main();

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, INT)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ComInitializer com;
//	tama::DebugWindow dbgWindow;

	auto desc = DXGI::GetOptDisplayMode(1280, 720);
	Win::Window window{ _T("title"), instance, desc.Width, desc.Height };
	Win::Window::CursorEnable(false);

	tama::CustomFont::SetUp();

	DXGI::Swapchain::GetInstance()->Create(window.GetHWnd());

	DX11::D2DManager::GetInstance()->SetDrawer(
		std::make_shared<DX11::DX11Drawer>(
			DX11::Device::Get(),
			DXGI::Swapchain::GetInstance()->Get()));

	// 即時コンテキストを取得
	auto c = DX11::GetImmediateContext();

	// ビューポートを設定
	CD3D11_VIEWPORT vp(0.f, 0.f, window.Width(), window.Height());
	c->RSSetViewports(1, &vp);

	Input::Initialize(window.GetHWnd());
	
//	DXGI::Swapchain::GetInstance()->SetFullscreen(true);

	Main();

	com.Release();

//	DXGI::Swapchain::GetInstance()->SetFullscreen(false);

	return 0;
}
