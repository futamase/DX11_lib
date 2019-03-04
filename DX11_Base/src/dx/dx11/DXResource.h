#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

#include <wrl.h>

namespace DX11
{
//	HRESULT CreateDeviceAndSwapChain(HWND hWnd, ID3D11Device** pDevice, ID3D11DeviceContext** pContext, IDXGISwapChain** pSwapChain);

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>
		CreateRenderTargetView(UINT width, UINT height, DXGI_FORMAT format);

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>
		CreateRenderTargetView(ID3D11Texture2D* buffer);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>
		CreateDepthStencilView(UINT width, UINT height, DXGI_FORMAT format);

	Microsoft::WRL::ComPtr<ID3D11Buffer>
		CreateBuffer(const void* data, size_t size, UINT cpuAccessFlag, D3D11_BIND_FLAG BindFlag);

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>
		CreateRasterizerState(D3D11_CULL_MODE cullmode, bool drawWireFrame = false);

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRasterizerState(
		D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		bool frontCounterClockwise,
		bool scissorEnable,
		int32_t depthBias);

	Microsoft::WRL::ComPtr<ID3D11BlendState>
		CreateBlendState(const std::vector<D3D11_RENDER_TARGET_BLEND_DESC>& descList, BOOL AlphaToCoverageEnable);

	Microsoft::WRL::ComPtr<ID3D11BlendState>
		CreateBlendState(bool enable, uint8_t srcBlend, uint8_t destBlend, uint8_t blendOp,
			uint8_t srcBlendAlpha, uint8_t destBlendAlpha, uint8_t blendOpAlpha);

	Microsoft::WRL::ComPtr<ID3D11SamplerState>
		CreateSamplerState(const D3D11_SAMPLER_DESC& desc);

	Microsoft::WRL::ComPtr<ID3D11SamplerState>
		CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>
		CreateDepthStencilState();

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>
		CreteDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc);

	Microsoft::WRL::ComPtr<ID3D11Texture2D>
		CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		CreateShaderResourceView(ID3D11Texture2D* texture);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		CreateShaderResourceView(const std::wstring& filepath);

	Microsoft::WRL::ComPtr<ID3D11DeviceContext>
		GetImmediateContext();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext>
		CreateDeferredContext();

	Microsoft::WRL::ComPtr<ID3D11Texture2D>
		GetBackBuffer(IDXGISwapChain* swapChain);

	DirectX::XMFLOAT2 GetTextureSize(ID3D11ShaderResourceView* srv);
}
