#include "ViewPort.h"
#include <cassert>

namespace tama
{
	
	std::vector<D3D11_VIEWPORT> HorizontalDevide(float width, float height, unsigned int num)
	{
		std::vector<D3D11_VIEWPORT> vps;
		switch (num)
		{
		case 1: //画面いっぱい
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height));
			break;
		case 2: //上下半分
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0, height / 2.f, width, height / 2.f));
			break;
		case 3:	//上半分と下半分を2分割
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0, height / 2.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
			break;
		case 4: //4分割 (左上->左下->右上->右下)
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0.f, height / 2.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
		}
		return vps;
	}

	std::vector<D3D11_VIEWPORT> VerticalDevide(float width, float height, unsigned num)
	{
		std::vector<D3D11_VIEWPORT> vps;
		switch (num)
		{
		case 1: //画面いっぱい
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height));
			break;
		case 2: //左右半分
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height));
			break;
		case 3:	//左半分と右半分を2分割
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
			break;
		case 4: //４分割 (左上->右上->左下->右下)
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0.f, height / 2.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
		}
		return vps;
	}

	Viewport::Viewport(unsigned int width, unsigned int height, unsigned int num, DevideMode mode)
	{
		assert(num <= 4);

		if (mode == DevideMode::Horizontally)
		{
			m_vps = HorizontalDevide(static_cast<float>(width), static_cast<float>(height), num);
		}
		else if (mode == DevideMode::Vertically)
		{
			m_vps = VerticalDevide(static_cast<float>(width), static_cast<float>(height), num);
		}
	}

	unsigned int Viewport::size() const
	{
		return m_vps.size();
	}

	const D3D11_VIEWPORT* Viewport::data() const
	{
		return m_vps.data();
	}
}