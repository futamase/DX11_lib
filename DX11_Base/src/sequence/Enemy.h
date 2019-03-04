#pragma once
#include "../base/Stopwatch.h"
#include "../base/Random.h"
#include "../dx/dx11/Sprite.h"
#include "../dx/dx11/TextureCache.h"
#include "../dx/dx11/Shader.h"
#include "../system/Sound.h"
#include "../dx/dx11/ConstantBuffer.h"

namespace game
{
	/*
		10秒経過->出現
	*/
	class Enemy
	{
		tama::Stopwatch m_watch;
		bool m_isActive = false;
		bool m_isDeath = false;
		tama::Texture m_chara, m_opendoor;

		tama::PixelShader m_alphaShader;
		tama::ConstantBuffer<tama::Vec4> m_cb;

		int m_time = 5;

		bool m_isDoor = false;

	public:
		Enemy()
			: m_alphaShader(L"Assets/Shader/SpriteAlpha.hlsl", "main")
		{
			m_cb->x = 0.5f;
			m_cb.Set(2, tama::ShaderStage::VS_PS);
		}

		void SetTexture(const std::wstring& name)
		{
			// 月のドアを除外したい
			if (!tama::TextureCache::I().Exist(L"black_" + name))
				return;
			if (!tama::TextureCache::I().Exist(L"open_" + name))
				return;
			m_opendoor = tama::TextureCache::I().At(L"open_" + name);
			m_chara = tama::TextureCache::I().At(L"black_" + name);
			m_isDoor = true;
		}

		void InitTime()
		{
			m_watch.Restart();
			m_time = 5;
		}

		void Destroy()
		{
			m_isActive = false;
			m_time = 10;
			m_cb->x = 0.5f;
			m_cb.Set(2, tama::ShaderStage::PS);
		}

		void Update()
		{
			if (!m_isDoor)
				return;

			if (!m_isActive)
			{
				if (m_watch.seconds() == m_time)
				{
					if (tama::Random::Bool(0.5f))
					{
						m_isActive = true;
						Music::SoundManager::I().PlayOneShot(L"kisimi");
					}
					m_watch.Restart();
					m_time = 5;
				}
			}
			else
			{
				if(m_watch.IsActive())
					m_cb->x += 0.001f;
				if (m_watch.seconds() == 10)
				{
					// 死
					m_isDeath = true;
					m_cb->x = 0;
				}
				m_cb.Set(2, tama::ShaderStage::PS);
			}
		}

		int GetTime() const
		{
			return m_watch.seconds();
		}

		bool IsActive() const
		{
			return m_isActive;
		}
		bool IsDeath() const
		{
			return m_isDeath;
		}

		void Draw() const
		{
			//一応
			if (!m_isDoor)
				return;

			if (m_isActive)
			{
				tama::Sprite::DrawImmediate(m_chara);
				Graphics::SetPixelShader(m_alphaShader);
				tama::Sprite::DrawImmediate(m_opendoor);
			}
			static tama::Font s;
			s(m_watch.seconds()).Draw(800, 300, 300, 300);

		}
	};
}