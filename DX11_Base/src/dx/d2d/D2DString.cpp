#include "D2DString.h"
#include "Direct2D.h"

#include "DWriteCustomFont\ResourceFontContext.h"

#include <dwrite_1.h>
#include <vector>
#include <algorithm>

namespace tama
{
	std::vector<TextFormat> s_formatContainer;

	Font::Font(const TextFormat& format)
	{
		m_format = format;

		if (std::any_of(s_formatContainer.begin(), s_formatContainer.end(),
			[&format](const TextFormat& fmt) { return (format == fmt); }))
		{
			return;
		}

		s_formatContainer.emplace_back(format);

		DX11::D2DManager::GetInstance()->CreateTextFormat(
			format.size, format.font,
			static_cast<DWRITE_FONT_WEIGHT>(format.weight),
			static_cast<DWRITE_FONT_STYLE>(format.style),
			static_cast<DWRITE_FONT_STRETCH>(format.stretch));
	}

	Font::Font() :
		Font(TextFormat())
	{}

	void Font::Draw(float x, float y, float w, float h, const tama::ColorF& color) const
	{
		DX11::D2DManager::GetInstance()->DrawString(
			m_format.GetHashKey(),
			m_text,
			x, y,
			w, h,
			D2D1::ColorF(color.x, color.y, color.z, color.w));
	}

//	FixedString::FixedString(const std::wstring& text, const TextFormat& format) :
//		m_text(text), m_format(format)
//	{
//		DX11::D2DManager::GetInstance()->CreateTextLayout(
//			m_text, m_format.font, &m_layout);
//	}

	namespace CustomFont
	{
		static ResourceFontContext s_fontContext;

		void SetUp()
		{
			static bool alreadyInit = false;
			if (!alreadyInit) {
				s_fontContext.SetDwriteFactory(
					DX11::D2DManager::GetInstance()->GetDWriteFactory());
				s_fontContext.Initialize();
				alreadyInit = true;
			}
		}

		void Register(const std::wstring& familyName, uint32_t id)
		{
			UINT ids[] = { id };

			Microsoft::WRL::ComPtr<IDWriteFontCollection> collection;
			s_fontContext.CreateFontCollection(
				ids,
				sizeof(UINT),//std::wstring),
				&collection);

			s_formatContainer.emplace_back(
				TextFormat(40, familyName));

			auto format = s_formatContainer.back();

			DX11::D2DManager::GetInstance()->CreateTextFormat(
				format.size, format.font,
				static_cast<DWRITE_FONT_WEIGHT>(format.weight),
				static_cast<DWRITE_FONT_STYLE>(format.style),
				static_cast<DWRITE_FONT_STRETCH>(format.stretch),
				collection.Get());
		}

		TextFormat Get(const std::wstring& familyName)
		{
			for (auto&& format : s_formatContainer)
			{
				if (format.font == familyName)
				{
					return format;
				}
			}

			throw std::exception("“o˜^‚³‚ê‚Ä‚¢‚È‚¢");
		}
	}
}
