#pragma once

#include <vector>
#include <d3d11.h>

namespace tama
{
	/**@�r���[�|�[�g�̕������[�h*/
	enum class DevideMode
	{
		Horizontally,	//!�����ɕ���
		Vertically		//!�����ɕ���
	};

	/**@brief �r���[�|�[�g�쐬�N���X
	*@atention �������ƃJ�����̐��͈�v���Ă��Ȃ���΂Ȃ�Ȃ�
	*/
	class Viewport
	{
		std::vector<D3D11_VIEWPORT> m_vps;
	public:
		/**@brief �R���X�g���N�^
		*@attention 1~4�����F�߂Ȃ�*/
		Viewport(unsigned int width, unsigned int height, unsigned int num, DevideMode mode);
		unsigned int size() const; 
		const D3D11_VIEWPORT* data() const;
	};
}