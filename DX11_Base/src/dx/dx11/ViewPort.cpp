#include "ViewPort.h"
#include <cassert>

namespace tama
{
	
	std::vector<D3D11_VIEWPORT> HorizontalDevide(float width, float height, unsigned int num)
	{
		std::vector<D3D11_VIEWPORT> vps;
		switch (num)
		{
		case 1: //��ʂ����ς�
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height));
			break;
		case 2: //�㉺����
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0, height / 2.f, width, height / 2.f));
			break;
		case 3:	//�㔼���Ɖ�������2����
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(0, height / 2.f, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
			break;
		case 4: //4���� (����->����->�E��->�E��)
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
		case 1: //��ʂ����ς�
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width, height));
			break;
		case 2: //���E����
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height));
			break;
		case 3:	//�������ƉE������2����
			vps.emplace_back(CD3D11_VIEWPORT(0.f, 0.f, width / 2.f, height));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, 0, width / 2.f, height / 2.f));
			vps.emplace_back(CD3D11_VIEWPORT(width / 2.f, height / 2.f, width / 2.f, height / 2.f));
			break;
		case 4: //�S���� (����->�E��->����->�E��)
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