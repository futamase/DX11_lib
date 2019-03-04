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
				nullptr,            // 使用するアダプターを設定。nullptrの場合はデフォルトのアダプター。
				type,               // D3D_DRIVER_TYPEのいずれか。ドライバーの種類。
									// pAdapterが nullptr 以外の場合は、D3D_DRIVER_TYPE_UNKNOWNを指定する。
				nullptr,            // ソフトウェアラスタライザを実装するDLLへのハンドル。
									// D3D_DRIVER_TYPE を D3D_DRIVER_TYPE_SOFTWARE に設定している場合は nullptr にできない。
				createDeviceFlag,   // D3D11_CREATE_DEVICE_FLAGの組み合わせ。デバイスを作成時に使用されるパラメータ。
				featureLevels,      // D3D_FEATURE_LEVELのポインタ
				_countof(featureLevels), // D3D_FEATURE_LEVEL配列の要素数
				D3D11_SDK_VERSION,  // DirectX SDKのバージョン。この値は固定。
				&m_device,            // 初期化されたデバイス
				nullptr,            // 採用されたフィーチャーレベル
				nullptr             // 初期化されたデバイスコンテキスト
				);
			if (SUCCEEDED(hr)) break;
		}
		if (!m_device) 
			throw std::runtime_error("ID3D11Deviceの生成に失敗しました.");
	}

	ID3D11Device* Device::Get()
	{
		static Device instance;
		return instance.m_device.Get();
	}
}
