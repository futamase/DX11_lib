#pragma once

namespace tama
{
	/**@brief シェーダステージを表す*/
	enum class ShaderStage : unsigned char 
	{
		VS = 0b00000001,	//! 頂点シェーダ
		GS = 0b00000010,	//! ジオメトリシェーダ
		PS = 0b00000100,	//! ピクセルシェーダ
		CS = 0b00001000,	//! コンピュートシェーダ
		DS = 0b00010000,	//! ドメインシェーダ
		HS = 0b00100000,	//! ハルシェーダ

		VS_GS = VS | GS,
		VS_PS = VS | PS,
	};
}
