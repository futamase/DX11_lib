#include "DX11Device.h"

#include <stdexcept>

#pragma comment(lib, "d3d11")

namespace DX11
{
	Device::Device()
	{
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_SOFTWARE,
		};

		UINT createDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		ID3D11Device* device = nullptr;
		for (auto type : driverTypes) {
			auto hr = ::D3D11CreateDevice(
				nullptr,            // �g�p����A�_�v�^�[��ݒ�Bnullptr�̏ꍇ�̓f�t�H���g�̃A�_�v�^�[�B
				type,               // D3D_DRIVER_TYPE�̂����ꂩ�B�h���C�o�[�̎�ށB
									// pAdapter�� nullptr �ȊO�̏ꍇ�́AD3D_DRIVER_TYPE_UNKNOWN���w�肷��B
				nullptr,            // �\�t�g�E�F�A���X�^���C�U����������DLL�ւ̃n���h���B
									// D3D_DRIVER_TYPE �� D3D_DRIVER_TYPE_SOFTWARE �ɐݒ肵�Ă���ꍇ�� nullptr �ɂł��Ȃ��B
				createDeviceFlag,   // D3D11_CREATE_DEVICE_FLAG�̑g�ݍ��킹�B�f�o�C�X���쐬���Ɏg�p�����p�����[�^�B
				featureLevels,      // D3D_FEATURE_LEVEL�̃|�C���^
				_countof(featureLevels), // D3D_FEATURE_LEVEL�z��̗v�f��
				D3D11_SDK_VERSION,  // DirectX SDK�̃o�[�W�����B���̒l�͌Œ�B
				&m_device,            // ���������ꂽ�f�o�C�X
				nullptr,            // �̗p���ꂽ�t�B�[�`���[���x��
				nullptr             // ���������ꂽ�f�o�C�X�R���e�L�X�g
				);
			if (SUCCEEDED(hr)) break;
		}
		if (!m_device) 
			throw std::runtime_error("ID3D11Device�̐����Ɏ��s���܂���.");
	}

	ID3D11Device* Device::Get()
	{
		static Device instance;
		return instance.m_device.Get();
	}
}
