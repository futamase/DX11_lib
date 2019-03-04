#pragma once

#include <vector>
#include <d3d11.h>

namespace tama
{
	/**@ビューポートの分割モード*/
	enum class DevideMode
	{
		Horizontally,	//!水平に分割
		Vertically		//!垂直に分割
	};

	/**@brief ビューポート作成クラス
	*@atention 分割数とカメラの数は一致していなければならない
	*/
	class Viewport
	{
		std::vector<D3D11_VIEWPORT> m_vps;
	public:
		/**@brief コンストラクタ
		*@attention 1~4しか認めない*/
		Viewport(unsigned int width, unsigned int height, unsigned int num, DevideMode mode);
		unsigned int size() const; 
		const D3D11_VIEWPORT* data() const;
	};
}