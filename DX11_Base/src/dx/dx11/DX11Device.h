#pragma once

#include <d3d11_1.h>
#include <wrl.h>

namespace DX11
{
	class Device
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;

	private:
		Device();
		Device(const Device&) = delete;
		Device& operator= (const Device&) = delete;

	public:
		static ID3D11Device* Get();
	};


}
