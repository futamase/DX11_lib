#include "Graphics.h"
#include "../DX/DX11/ConstantBuffer.h"
#include "../DX/DX11/DXResource.h"
#include "../dx/dxgi/Swapchain.h"
#include "../dx/d2d/Direct2D.h"

#include <vector>

namespace Graphics
{
	namespace 
	{
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()
		{
			static Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
			if (!context)
				context = DX11::GetImmediateContext();

			return context;
		}
	}
//	void SetViewport(const DX11::Viewport& vp)
//	{
//		DX11::GetImmediateContext()->RSSetViewports(vp.size(), vp.data());
//	
//		DX11::ConstantBuffer<DirectX::XMFLOAT4> vpNum;
//		vpNum->x = (float)vp.size();
//		vpNum.Set(4, DX11::ShaderStage::GS);
//	}

	void SetScissor(const std::vector<D3D11_RECT>& rectList)
	{
		DX11::GetImmediateContext()->RSSetScissorRects(rectList.size(), rectList.data());
	}

	void SetScissor(const std::initializer_list<D3D11_RECT>& rectList)
	{
		auto rectVec = std::vector<D3D11_RECT>(rectList.begin(), rectList.end());

		SetScissor(rectVec);
	}

	void SetDefaultRenderTarget()
	{
		auto data = DXGI::Swapchain::GetInstance()->GetBackBuffer()->GetData();

		auto context = GetContext();

		context->OMSetRenderTargets(1, &data.RTV, nullptr);

		DX11::D2DManager::GetInstance()->SetDefaultRenderTarget();

//		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//		context->ClearRenderTargetView(data.RTV, clearColor);
	}

	void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
	{
		if (!rtv)
			throw std::exception("rtv‚Ínull‚Å‚Í‚¢‚¯‚È‚¢");

		DX11::GetImmediateContext()->OMSetRenderTargets(1, &rtv, dsv);
	}

	void SetVertexShader(const tama::VertexShader& vs)
	{
		auto context = GetContext();

		context->VSSetShader(vs.Get(), nullptr, 0);
	}

	void SetGeometryShader(const tama::GeometryShader& gs)
	{
		auto context = GetContext();

		context->GSSetShader(gs.Get(), nullptr, 0);
	}

	void SetPixelShader(const tama::PixelShader& ps)
	{
		auto context = GetContext();

		context->PSSetShader(ps.Get(), nullptr, 0);
	}

	void BindTexture(const tama::Texture& texture, uint32_t slot)
	{
		auto context = GetContext();

		auto srv = texture.GetData();
		context->PSSetShaderResources(slot, 1, &srv);
	}
}