#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "../D2D/D2DString.h"
#include "../../base/Math.h"
#include "../../base/Stopwatch.h"

namespace tama
{
	class SpriteAnimator
	{
		Int2 m_pixelSize, m_cellSize;
		int m_cellCount = 0;
		const Texture*const m_pTexture = nullptr;
		Stopwatch m_watch;
		float m_duration = 5.f;

	public:
		SpriteAnimator(const Texture& texture, const Int2& pixelSize, const Int2& cellSize, unsigned cellCount) :
			m_pixelSize(pixelSize),
			m_cellSize(cellSize),
			m_cellCount(cellCount),
			m_pTexture(&texture)
		{
			m_watch.Start();
		}

		/*

		time count

		*/

		const TextureRegion GetCell() const
		{
			static int current = 0;
			auto t = (m_duration / static_cast<float>(m_cellCount));
			auto time = m_watch.ms() / 1000.f;

			static Font font;
			font(time).Draw(500, 500, 100, 100);

			if (t * (current + 1) < (m_watch.ms() / 1000.f))
				current++;

			if (current > m_cellCount)
			{
				current = 0;
			}

			//	—ñ
			auto xxx = current % m_cellSize.x;
			// s
			auto yyy = current / m_cellSize.x;

			if (xxx > 5)
				int a = 0;

			font(m_watch.ms() / 100).Draw(500, 600, 100, 100);

			return m_pTexture->Region(
			{ xxx * m_pixelSize.x, yyy * m_pixelSize.y, m_pixelSize });
		}
	};
}