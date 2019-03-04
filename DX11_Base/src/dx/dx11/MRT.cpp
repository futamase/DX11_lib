#include "MRT.h"
#include "DX11Device.h"

#if 0
namespace DX11
{
	using Microsoft::WRL::ComPtr;

	void CreateMRT(uint32_t width, uint32_t height)
	{
		HRESULT hr;

		DXGI_SAMPLE_DESC smpl_desc;
		smpl_desc.Count = 1;
		smpl_desc.Quality = 0;

		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = width;
		tex_desc.Height = height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.SampleDesc = smpl_desc;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// MRT�����_�����O�����@���낢��ȃt�H�[�}�b�g�������Ă݂�
		D3D11_TEXTURE2D_DESC mrt_desc[4] = { tex_desc, tex_desc, tex_desc, tex_desc };

		// �f�v�X
		{
			auto& depth_decs = mrt_desc[0];
			depth_decs.Format = DXGI_FORMAT_R24G8_TYPELESS;
		}
		// �@���o�b�t�@
		{
			auto& nor_decs = mrt_desc[1];
			nor_decs.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		}
		// Diffuse�i�����p�@���C�g�v���p�X�ł͕s�v�j
		{
			auto& nor_decs = mrt_desc[2];
			nor_decs.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		// SpecularPow
		{
			auto& nor_decs = mrt_desc[3];
			nor_decs.Format = DXGI_FORMAT_R8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		// MRT�̊e�o�b�t�@�̃t�H�[�}�b�g�����Ȃǂ���̂�
		// �Ƃ肠�����T�C�Y�̈Ⴄ�t�H�[�}�b�g�����݂��Ă݂�
		for (auto& d : mrt_desc) {
			MRTResource mrt;
			ComPtr<ID3D11RenderTargetView> rtview;
			hr = Device::Get()->CreateTexture2D(&d, nullptr, &mrt.pTex2D);
			if (FAILED(hr)) {
				return;
			}
			if (&d == &mrt_desc[0]) {
				// �f�v�X�o�b�t�@
				D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
				ZeroMemory(&dsv_desc, sizeof(dsv_desc));
				dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

				hr = Device::Get()->CreateDepthStencilView(mrt.pTex2D.Get(), &dsv_desc, &dsview);
				if (FAILED(hr)) {
					return;
				}

				// �f�v�X�l���V�F�[�_����
				D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
				ZeroMemory(&srv_desc, sizeof(srv_desc));
				srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srv_desc.Texture2D.MipLevels = 1;
				srv_desc.Texture2D.MostDetailedMip = 0;
				hr = Device::Get()->CreateShaderResourceView(mrt.pTex2D.Get(), &srv_desc, &dsrview);
				if (FAILED(hr)) {
					return;
				}

				// �X�e���V���l���V�F�[�_����
				srv_desc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				hr = Device::Get()->CreateShaderResourceView(mrt.pTex2D.Get(), &srv_desc, &ssrview);
				if (FAILED(hr)) {
					return false;
				}
			}
			else {

				hr = Device::Get()->CreateRenderTargetView(mrt.pTex2D.Get(), nullptr, &mrt.pRTView);
				if (FAILED(hr)) {
					return false;
				}

				hr = Device::Get()->CreateShaderResourceView(mrt.pTex2D.Get(), nullptr, &mrt.pSRView);
				if (FAILED(hr)) {
					return false;
				}

				mrtres.push_back(mrt);
			}
		}
	}
}
#endif
