#pragma once

#include <wrl.h>
#include <Windows.h>
#include <d3d11_1.h>
#include <dxgi.h>

namespace DXGI
{
	// i番目のディスプレイを取得
	Microsoft::WRL::ComPtr<IDXGIOutput> AccessDisplay(UINT i);

	// 利用可能な表示モードの取得
	void GetDisplayModes(DXGI_MODE_DESC* pModeDesc, DXGI_FORMAT format, UINT * pNum = nullptr);

	// 利用可能な表示モードの数を取得
	UINT GetNumOfDisplayModes(DXGI_FORMAT format);

	// 適切な解像度のディスプレイモードを取得
	DXGI_MODE_DESC GetOptDisplayMode(int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	// IDXGIAdapter -> IDXGIFactory
	Microsoft::WRL::ComPtr<IDXGIFactory> AccessGIFactory(IDXGIAdapter * adapter);
	// ID3D11Device -> IDXGIAdapter -> IDXGIFactory
	Microsoft::WRL::ComPtr<IDXGIFactory> AccessGIFactory(ID3D11Device * device);

	// ID3D11Device -> IDXGIDevice1
	Microsoft::WRL::ComPtr<IDXGIDevice1> AccessInterface(ID3D11Device * device);
	// ID3D11Device -> IDXGIDevice1 -> IDXGIAdapter
	Microsoft::WRL::ComPtr<IDXGIAdapter> AccessAdapter(ID3D11Device * device);

	// ID3D11Texture2D -> IDXGISurface
	Microsoft::WRL::ComPtr<IDXGISurface> AccessSurface(ID3D11Texture2D * tex);
}
