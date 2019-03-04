#pragma once

#include "../DX/DX11/Image.h"
#include "../DX/DX11/Texture.h"
#include "../DX/DX11/Sprite.h"
#include "../base/Math.h"
#include "../base/Geometry.h"
#include "../base/Tween.h"
#include <DirectXTK\SpriteBatch.h>


class Fader
{
private:
	tama::Sprite m_sprite;
	tama::Texture m_black;
	float m_alpha = 0.f;
	bool m_isCalled = false;
	tama::RectF m_rc;
	tama::Palette m_color = tama::Palette::Black;

public:
	Fader(uint32_t w, uint32_t h) :
		m_black(tama::Image{ w, h })
		, m_rc(w, h)
	{
		m_sprite.SetTexture(m_black);
	}

	void FadeIn(float duration, const std::function<void()>& callback)
	{
		m_isCalled = true;
		m_alpha = 1.f;
		tama::TamaTween::I().To(&m_alpha, 0.f, duration)
			.OnComplete([this, callback]()
		{
			callback();
			this->m_isCalled = false;
		});
	}

	void FadeOut(float duration, const std::function<void()>& callback)
	{
		m_isCalled = true;
		m_alpha = 0.f;
		tama::TamaTween::I().To(&m_alpha, 1.f, duration)
			.OnComplete([this, callback]()
		{
			callback();
			this->m_isCalled = false;
		});
	}

	void FadeOut(float duration, Easing::EasingFunc* easingFunc, const std::function<void()>& callback)
	{
		m_isCalled = true;
		m_alpha = 0.f;
		tama::TamaTween::I().To(&m_alpha, 1.f, duration)
			.SetEase(easingFunc)
			.OnComplete([this, callback]()
		{
			callback();
			this->m_isCalled = false;
		});
	}

	// rendererÇÃBegin Endä‘Ç…èëÇ¢ÇƒÇ≠ÇæÇ≥Ç¢
	void Draw(DirectX::SpriteBatch* renderer = nullptr) const
	{
		if (m_isCalled)
		{
		//	m_sprite.DrawImmediate(tama::ColorF{ 0,0,0,m_alpha });
			m_rc.Fill(tama::ColorF(m_color, m_alpha));
		}
	}
};
