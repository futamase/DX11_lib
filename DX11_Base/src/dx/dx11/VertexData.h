/**@file Vertex.h
*@brief ��{�I�Ȓ��_
*/
#pragma once

#include <DirectXMath.h>
#include <memory>
#include <array>

namespace DX11
{
	/**@brief ��{�I�Ȓ��_*/
	struct DefaultVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texel;
		/**@brief ���[�v�p�̉��Z�q*/
		bool operator < (const DefaultVertex& v) const {
			return std::memcmp(this, &v, sizeof(DefaultVertex)) < 0;
		}
		/**@brief �R���X�g���N�^*/
		DefaultVertex() :
			pos(0.f, 0.f, 0.f),
			color(1.f, 1.f, 1.f, 1.f),
			normal(0.f, 0.f, 0.f),
			texel(0.f, 0.f)
		{}
	};

	/**@brief �X�L�����b�V���A�j���[�V�����p�̒��_*/
	struct SkinMeshVertex 
	{
		DirectX::XMFLOAT3 pos;
		uint32_t color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texel;
		std::array<uint8_t, 4> boneIndex;
		DirectX::XMFLOAT4 boneWeight;
		bool operator < (const SkinMeshVertex& v) const {
			return std::memcmp(this, &v, sizeof(SkinMeshVertex)) < 0;
		}
		SkinMeshVertex()
		{
			pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
			color = 0;
			normal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
			texel = DirectX::XMFLOAT2(0.f, 0.f);
			std::for_each(boneIndex.begin(), boneIndex.end(), [](uint8_t i)
			{
				i = 0;
			});
			boneWeight = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		}
	};
}
