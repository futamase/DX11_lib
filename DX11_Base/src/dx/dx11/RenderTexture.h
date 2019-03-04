#pragma once
#include "Texture.h"
#include "../../base/Math.h"

namespace tama 
{
	// �����_�[�^�[�Q�b�g��ς���Ƃ���d2d���ς��Ȃ��Ƃ����Ȃ�
	class RenderTexture final : public Texture 
	{
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	public:
		RenderTexture(uint32_t width, uint32_t height);

		void SetRenderTarget(bool clear = true) const;

		~RenderTexture();
	};
}
