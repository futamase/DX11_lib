#pragma once

#include "../DX/DX11/ViewPort.h"
#include "../dx/dx11/Shader.h"
#include "../dx/dx11/Texture.h"

namespace Graphics
{
//	void SetViewport(const DX11::Viewport& vp);

	void SetScissor(const std::vector<D3D11_RECT>& rectList);

	void SetScissor(const std::initializer_list<D3D11_RECT>& rectList);

	void SetDefaultRenderTarget();

	// TODO: ユーザに公開するので、ラッピングをしたい
	void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);

	void SetVertexShader(const tama::VertexShader& vs);

	void SetGeometryShader(const tama::GeometryShader& gs);

	void SetPixelShader(const tama::PixelShader& ps);

	void BindTexture(const tama::Texture& texture, uint32_t slot);
}