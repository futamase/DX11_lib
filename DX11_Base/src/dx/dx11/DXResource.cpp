#include "DX11device.h"
#include "DXResource.h"
#include <stdexcept>
#include <cassert>

#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex")

#pragma comment( lib, "dxguid" )
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "d3dcompiler" )  

namespace DX11
{
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;

	ComPtr<ID3D11RenderTargetView>
		CreateRenderTargetView(UINT width, UINT height, DXGI_FORMAT format)
	{
		ComPtr<ID3D11RenderTargetView> rtv;

		auto desc = CD3D11_TEXTURE2D_DESC(
			format,
			width,
			height,
			1, 1,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		ID3D11Texture2D* texture;
		HRESULT hr = Device::Get()->CreateTexture2D(&desc, nullptr, &texture);
		if (FAILED(hr)) throw std::runtime_error("テクスチャの生成に失敗");

		auto vdesc = CD3D11_RENDER_TARGET_VIEW_DESC(texture, D3D11_RTV_DIMENSION_TEXTURE2D);

		hr = Device::Get()->CreateRenderTargetView(texture, &vdesc, &rtv);
		if (FAILED(hr)) throw std::runtime_error("レンダーターゲットビューの作成に失敗");

		return rtv;
	}

	ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Texture2D* buffer)
	{
		ComPtr<ID3D11RenderTargetView> rtv;
		if (FAILED(Device::Get()->CreateRenderTargetView(buffer, nullptr, &rtv)))
			throw std::runtime_error("レンダーターゲットビューの作成に失敗");

		return rtv;
	}

	ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(UINT width, UINT height, DXGI_FORMAT format)
	{
		assert(
			format == DXGI_FORMAT_D16_UNORM ||
			format == DXGI_FORMAT_D24_UNORM_S8_UINT ||
			format == DXGI_FORMAT_D32_FLOAT ||
			format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT
			);

		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> dsv;

		CD3D11_TEXTURE2D_DESC descDepth(format, width, height, 1, 1,
			D3D11_BIND_DEPTH_STENCIL);
		HRESULT hr = Device::Get()->CreateTexture2D(&descDepth, nullptr, &depthStencilBuffer);
		if (FAILED(hr)) throw std::runtime_error("テクスチャの生成に失敗");

		CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(
			D3D11_DSV_DIMENSION_TEXTURE2D,
			descDepth.Format);
		hr = Device::Get()->CreateDepthStencilView(depthStencilBuffer.Get(), &descDSV, &dsv);
		if (FAILED(hr)) throw std::runtime_error("深度ステンシルビューの作成に失敗");

		return dsv;
	}

	ComPtr<ID3D11Buffer> CreateBuffer(
		const void* data, size_t size, UINT cpuAccessFlag, D3D11_BIND_FLAG BindFlag)
	{
		ComPtr<ID3D11Buffer> buffer;

		auto resource = std::make_shared<D3D11_SUBRESOURCE_DATA>();
		resource->pSysMem = data;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;

		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.ByteWidth = size;
		BufferDesc.CPUAccessFlags = cpuAccessFlag;
		BufferDesc.Usage = (cpuAccessFlag == 0) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		BufferDesc.MiscFlags = 0;
		BufferDesc.BindFlags = BindFlag;
		if (BindFlag == D3D11_BIND_STREAM_OUTPUT) BufferDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;

		auto hr = Device::Get()->CreateBuffer(&BufferDesc, (data == nullptr) ? nullptr : resource.get(), &buffer);
		if (FAILED(hr)) throw std::runtime_error("ID3D11Bufferの生成に失敗しました.");

		return buffer;
	}

	ComPtr<ID3D11RasterizerState> CreateRasterizerState(D3D11_CULL_MODE cullmode, bool drawWireFrame)
	{
		ComPtr<ID3D11RasterizerState> rasterizer;

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = drawWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		desc.CullMode = cullmode;
		desc.FrontCounterClockwise = false;

		if (FAILED(Device::Get()->CreateRasterizerState(&desc, &rasterizer)))
			throw std::runtime_error("ラスタライザステートの作成に失敗");

		return rasterizer;
	}

	ComPtr<ID3D11RasterizerState> CreateRasterizerState(
		D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		bool frontCounterClockwise,
		bool scissorEnable,
		int32_t depthBias)
	{
		ComPtr<ID3D11RasterizerState> rasterizer;

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.CullMode = cullMode;
		desc.FillMode = fillMode;
		desc.FrontCounterClockwise = frontCounterClockwise;
		desc.ScissorEnable = scissorEnable;
		desc.DepthBias = depthBias;

		if (FAILED(Device::Get()->CreateRasterizerState(&desc, &rasterizer)))
			throw std::runtime_error("ラスタライザステートの作成に失敗");

		return rasterizer;
	}

	ComPtr<ID3D11BlendState> CreateBlendState(const std::vector<D3D11_RENDER_TARGET_BLEND_DESC>& descList, BOOL AlphaToCoverageEnable)
	{
		ComPtr<ID3D11BlendState> state;
		auto numDesc = descList.size();

		if (numDesc > 8)
			throw std::exception("ブレンドステートの上限を超えている");

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		desc.IndependentBlendEnable = numDesc > 1 ? TRUE : FALSE;

		for (UINT i = 0; i < numDesc; i++)
		{
			std::memcpy(&desc.RenderTarget[i], &descList[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		}
		if (FAILED(Device::Get()->CreateBlendState(&desc, &state)))
			throw std::runtime_error("ブレンドステートの作成に失敗");

		return state;
	}

	ComPtr<ID3D11BlendState> CreateBlendState(
		bool enable, 
		uint8_t srcBlend, uint8_t destBlend, uint8_t blendOp, 
		uint8_t srcBlendAlpha,uint8_t destBlendAlpha, uint8_t blendOpAlpha)
	{
		D3D11_RENDER_TARGET_BLEND_DESC desc;
		desc.BlendEnable = enable;
		desc.SrcBlend = static_cast<D3D11_BLEND>(srcBlend);
		desc.DestBlend = static_cast<D3D11_BLEND>(destBlend);
		desc.BlendOp = static_cast<D3D11_BLEND_OP>(blendOp);
		desc.SrcBlendAlpha = static_cast<D3D11_BLEND>(srcBlendAlpha);
		desc.DestBlendAlpha = static_cast<D3D11_BLEND>(destBlendAlpha);
		desc.BlendOpAlpha = static_cast<D3D11_BLEND_OP>(blendOpAlpha);
		desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return CreateBlendState(std::vector<D3D11_RENDER_TARGET_BLEND_DESC>({ desc }), false);
	}

	ComPtr<ID3D11SamplerState> CreateSamplerState(const D3D11_SAMPLER_DESC& desc)
	{
		ComPtr<ID3D11SamplerState> state;

		if (FAILED(Device::Get()->CreateSamplerState(&desc, &state)))
			throw std::runtime_error("サンプラステートの作成に失敗");

		return state;
	}

	ComPtr<ID3D11SamplerState> CreateSamplerState(
		D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW)
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = filter;
		desc.AddressU = AddressU;
		desc.AddressV = AddressV;
		desc.AddressW = AddressW;
		desc.MaxAnisotropy = 16;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxLOD = FLT_MAX;

		return CreateSamplerState(desc);
	}

	ComPtr<ID3D11DepthStencilState> CreateDepthStencilState()
	{
		ComPtr<ID3D11DepthStencilState> state;

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = FALSE; 
		if (FAILED(Device::Get()->CreateDepthStencilState(&dsDesc, &state)))
			throw std::runtime_error("深度ステンシルステートの作成に失敗");

		return state;
	}

	ComPtr<ID3D11DepthStencilState> CreteDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		ComPtr<ID3D11DepthStencilState> state;
		if (FAILED(Device::Get()->CreateDepthStencilState(&desc, &state)))
			throw std::runtime_error("深度ステンシルステートの作成に失敗");

		return state;
	}

	ComPtr<ID3D11Texture2D> CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData)
	{
		ComPtr<ID3D11Texture2D> texture;
		if (FAILED(Device::Get()->CreateTexture2D(
			desc,
			initData,
			&texture
			)))
			throw std::runtime_error("Texture2Dの作成に失敗");

		return texture;
	}

	ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		ComPtr<ID3D11ShaderResourceView> srv;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;//DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		if (FAILED(Device::Get()->CreateShaderResourceView(texture, &srvDesc, &srv)))
			throw std::runtime_error("シェーダリソースビューの作成に失敗");

		return srv;
	}

	ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(const std::wstring& filepath)
	{

		ComPtr<ID3D11ShaderResourceView> shaderResourceView;

		std::unique_ptr<ScratchImage> m_spTexture = std::make_unique<ScratchImage>();

		WCHAR ext[_MAX_EXT];

		_wsplitpath_s(filepath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

		HRESULT hr;
		TexMetadata m_stInfo;
		if (_wcsicmp(ext, L".dds") == 0)
		{
			hr = LoadFromDDSFile(filepath.c_str(), DDS_FLAGS_NONE, &m_stInfo, *m_spTexture);
		}
		else if (_wcsicmp(ext, L".tga") == 0)
		{
			hr = LoadFromTGAFile(filepath.c_str(), &m_stInfo, *m_spTexture);
		}
		else
		{
			hr = LoadFromWICFile(filepath.c_str(), WIC_FLAGS_NONE, &m_stInfo, *m_spTexture);
		}

		if (FAILED(hr)) throw std::runtime_error("ファイルの読み込みに失敗");

		hr = DirectX::CreateShaderResourceView(
			Device::Get(),
			m_spTexture->GetImages(),
			m_spTexture->GetImageCount(),
			m_stInfo,
			&shaderResourceView);
		if (FAILED(hr))
			throw std::runtime_error("シェーダリソースビューの作成に失敗");

		return shaderResourceView;
	}

	ComPtr<ID3D11DeviceContext> GetImmediateContext()
	{
		ComPtr<ID3D11DeviceContext> context;

		Device::Get()->GetImmediateContext(context.GetAddressOf());
		if (!context) throw std::runtime_error("即時コンテキスト取得に失敗"); //まずありえない

		return context;
	}

	ComPtr<ID3D11DeviceContext>
		CreateDeferredContext()
	{
		ComPtr<ID3D11DeviceContext> context;

		//第一引数は将来の使用に備えて予約されたフラグだそう. 0を指定
		if (FAILED(Device::Get()->CreateDeferredContext(0, &context)))
			throw std::runtime_error("遅延コンテキストの生成に失敗");

		return context;
	}

	ComPtr<ID3D11Texture2D>
		GetBackBuffer(IDXGISwapChain* swapChain)
	{
		ComPtr<ID3D11Texture2D> backbuffer;
		if (FAILED(swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(backbuffer.GetAddressOf())))
			)
			throw std::runtime_error("バックバッファの取得に失敗");

		return backbuffer;
	}

	DirectX::XMFLOAT2 GetTextureSize(ID3D11ShaderResourceView* srv)
	{
		// Convert resource view to underlying resource.
		ID3D11Resource* resource;
		srv->GetResource(&resource);

		// Cast to texture.
		ID3D11Texture2D* texture2D;
		if (FAILED(resource->QueryInterface(
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&texture2D))))
			throw std::exception("シェーダリソースビューからテクスチャの取得に失敗");

		// Query the texture size.
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		resource->Release();
		texture2D->Release();

		return DirectX::XMFLOAT2(
			static_cast<float>(desc.Width), 
			static_cast<float>(desc.Height));
	}
}