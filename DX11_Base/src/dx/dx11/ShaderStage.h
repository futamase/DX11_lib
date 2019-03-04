#pragma once

namespace tama
{
	/**@brief �V�F�[�_�X�e�[�W��\��*/
	enum class ShaderStage : unsigned char 
	{
		VS = 0b00000001,	//! ���_�V�F�[�_
		GS = 0b00000010,	//! �W�I���g���V�F�[�_
		PS = 0b00000100,	//! �s�N�Z���V�F�[�_
		CS = 0b00001000,	//! �R���s���[�g�V�F�[�_
		DS = 0b00010000,	//! �h���C���V�F�[�_
		HS = 0b00100000,	//! �n���V�F�[�_

		VS_GS = VS | GS,
		VS_PS = VS | PS,
	};
}
