#include "RenderState.h"

#include "DXResource.h"
#include <d3d11_1.h>
#include <wrl.h>

#include <DirectXColors.h>

namespace Graphics
{
	using Microsoft::WRL::ComPtr;

	static std::unordered_map<tama::BlendState, ComPtr<ID3D11BlendState>> s_blendList;
	static std::unordered_map<tama::SamplerState, ComPtr<ID3D11SamplerState>> s_samplerList;
	static std::unordered_map<tama::DepthStencilState, ComPtr<ID3D11DepthStencilState>> s_depthStencilList;
	static std::unordered_map<tama::RasterizerState, ComPtr<ID3D11RasterizerState>> s_rasterizerList;

	void SetBlendState(const tama::BlendState& state)
	{
		if (s_blendList.find(state) == s_blendList.end()) 
		{
			D3D11_RENDER_TARGET_BLEND_DESC desc;
			desc.BlendEnable = state.enable;
			desc.SrcBlend = (D3D11_BLEND)state.src;
			desc.DestBlend = (D3D11_BLEND)state.dest;
			desc.BlendOp = (D3D11_BLEND_OP)state.op;
			desc.SrcBlendAlpha = (D3D11_BLEND)state.srcAlpha;
			desc.DestBlendAlpha = (D3D11_BLEND)state.destAlpha;
			desc.BlendOpAlpha = (D3D11_BLEND_OP)state.opAlpha;
			
			s_blendList.emplace(state,
				DX11::CreateBlendState(state.enable,
				(uint8_t)state.src,
					(uint8_t)state.dest,
					(uint8_t)state.op,
					(uint8_t)state.srcAlpha,
					(uint8_t)state.destAlpha,
					(uint8_t)state.opAlpha));//std::vector<D3D11_RENDER_TARGET_BLEND_DESC>({ desc }), false));
		}

		static const float factor[4] = { 0,0,0,0 };
		DX11::GetImmediateContext()->OMSetBlendState(s_blendList.at(state).Get(), factor, 0xFFFFFF);
	}

	void SetSamplerState(const tama::SamplerState& state, uint32_t slot)
	{
		if (s_samplerList.find(state) == s_samplerList.end())
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(state.addressU);
			desc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(state.addressV);
			desc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(state.addressW);
			desc.Filter = static_cast<D3D11_FILTER>(state.filter);
			desc.MaxAnisotropy = state.maxAnisotropy;

			tama::ColorF colorF(state.borderColor);
			desc.BorderColor[0] = colorF.x;
			desc.BorderColor[1] = colorF.y;
			desc.BorderColor[2] = colorF.z;
			desc.BorderColor[3] = colorF.w;

			s_samplerList.emplace(state, DX11::CreateSamplerState(desc));
		}

		auto sampler = s_samplerList.at(state).Get();
		DX11::GetImmediateContext()->PSSetSamplers(slot, 1, &sampler);
	}

	void SetRasterizerState(const tama::RasterizerState& state)
	{
		if (s_rasterizerList.find(state) == s_rasterizerList.end())
		{
			auto rasterizer = DX11::CreateRasterizerState(
				static_cast<D3D11_CULL_MODE>(state.cullMode),
				static_cast<D3D11_FILL_MODE>(state.fillMode),
				state.frontCounterClockwise,
				state.scissorEnable,
				state.depthBias
			);
			s_rasterizerList.emplace(state, rasterizer);
		}

		DX11::GetImmediateContext()->RSSetState(s_rasterizerList.at(state).Get());
	}

	void SetDepthStencilState(const tama::DepthStencilState& state)
	{
		if (s_depthStencilList.find(state) == s_depthStencilList.end())
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = state.depth.enable;
			desc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(state.depth.func);
			desc.DepthWriteMask = state.depth.writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = state.stencil.enable;
			desc.StencilReadMask = 0xFF;
			desc.StencilWriteMask = 0xFF;

			desc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(state.stencil.func);
			desc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(state.stencil.passOp);
			desc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(state.stencil.failOp);

			desc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(state.stencil.backFunc);
			desc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(state.stencil.backPassOp);
			desc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(state.stencil.backFailOp);
	
			s_depthStencilList.emplace(state, DX11::CreteDepthStencilState(desc));
		}

		DX11::GetImmediateContext()->OMSetDepthStencilState(s_depthStencilList.at(state).Get(), 0);
	}
}
