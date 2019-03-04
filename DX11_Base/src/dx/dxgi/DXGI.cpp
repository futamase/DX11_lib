#include "DXGI.h"
#include <stdexcept>
#include <memory>
#include <vector>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
using Microsoft::WRL::ComPtr;

namespace {
    ComPtr<IDXGIFactory> CreateFactory() {
        ComPtr<IDXGIFactory> factory;
        auto hr = ::CreateDXGIFactory(IID_PPV_ARGS(&factory));
        if (FAILED(hr))
			throw std::runtime_error("IDXGIFactory�̐����Ɏ��s���܂���.");
		return factory;
    }
 
    ComPtr<IDXGIAdapter> CreateAdapter() {
        ComPtr<IDXGIAdapter> adapter;
        auto hr = CreateFactory()->EnumAdapters(0, &adapter);
        if (FAILED(hr))
			throw std::runtime_error("IDXGIAdapter�̎擾�Ɏ��s���܂���.");
		return adapter;
    }
}
 
namespace DXGI
{
	ComPtr<IDXGIOutput> AccessDisplay(UINT i) {
		ComPtr<IDXGIOutput> display;
		auto hr = CreateAdapter()->EnumOutputs(i, &display);
		if (FAILED(hr))
			throw std::runtime_error("�f�B�X�v���C�̎擾�Ɏ��s���܂���.");
		return display;
	}

	void GetDisplayModes(DXGI_MODE_DESC* pModeDesc, DXGI_FORMAT format, UINT * pNum) {
		UINT num;
		auto hr = AccessDisplay(0)->GetDisplayModeList(format, 0, &num, pModeDesc);
		if (FAILED(hr)) throw std::runtime_error("�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂���.");
		if (pModeDesc == nullptr && num == 0) throw std::runtime_error("�f�B�X�v���C���[�h������擾�ł��܂���ł���.");

		if (pNum != nullptr) *pNum = num;
	}

	UINT GetNumOfDisplayModes(DXGI_FORMAT format) {
		UINT num;
		GetDisplayModes(nullptr, format, &num);
		return num;
	}

	DXGI_MODE_DESC GetOptDisplayMode(int width, int height, DXGI_FORMAT format) {
		auto num = GetNumOfDisplayModes(format);
		auto pModeDescArray = std::shared_ptr<DXGI_MODE_DESC>(
			new DXGI_MODE_DESC[num],
			std::default_delete<DXGI_MODE_DESC[]>()
			);
		GetDisplayModes(pModeDescArray.get(), format, nullptr);

		std::vector<DXGI_MODE_DESC> h;
		for (UINT i = 0; i < num; i++) {
			h.emplace_back(pModeDescArray.get()[i]);
		}

		// �K�؂ȉ𑜓x�̃f�B�X�v���C���[�h���������� 
		for (UINT i = 0; i < num; i++) {
			if (pModeDescArray.get()[i].Width == width && pModeDescArray.get()[i].Height == height) {
				DXGI_MODE_DESC  modeDesc;
				::CopyMemory(&modeDesc, &pModeDescArray.get()[i], sizeof(DXGI_MODE_DESC));
				return modeDesc;
			}
		}
		throw std::runtime_error("�K�؂ȃf�B�X�v���C���[�h��������܂���ł���.");
	}

	ComPtr<IDXGIFactory> AccessGIFactory(IDXGIAdapter * adapter) {
		ComPtr<IDXGIFactory> factory;
		auto hr = adapter->GetParent(IID_PPV_ARGS(&factory));
		if (FAILED(hr)) throw std::runtime_error("�A�_�v�^����t�@�N�g�����擾�ł��܂���ł���.");
		return factory;
	}

	ComPtr<IDXGIFactory> AccessGIFactory(ID3D11Device * device) {
		return AccessGIFactory(AccessAdapter(device).Get());
	}

	ComPtr<IDXGIDevice1> AccessInterface(ID3D11Device * device) {
		ComPtr<IDXGIDevice1> dxgiDevice;
		auto hr = device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		if (FAILED(hr)) throw std::runtime_error("�f�o�C�X����C���^�t�F�[�X���擾�ł��܂���ł���.");
		return dxgiDevice;
	}

	ComPtr<IDXGIAdapter> AccessAdapter(ID3D11Device * device) {
		ComPtr<IDXGIAdapter> adapter;
		auto hr = AccessInterface(device)->GetAdapter(&adapter);
		if (FAILED(hr)) throw std::runtime_error("�f�o�C�X����A�_�v�^���擾�ł��܂���ł���.");
		return adapter;
	}

	ComPtr<IDXGISurface> AccessSurface(ID3D11Texture2D * tex) {
		ComPtr<IDXGISurface> surface;
		auto hr = tex->QueryInterface(IID_PPV_ARGS(&surface));
		if (FAILED(hr)) throw std::runtime_error("�e�N�X�`������T�[�t�F�C�X���擾�ł��܂���ł���.");
		return surface;
	}
}
