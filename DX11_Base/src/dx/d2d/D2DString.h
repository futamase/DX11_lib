#pragma once

#include <string>
#include <sstream>


#include "../../base/Math.h"
#include "../../base/GetTextImpl.h"

namespace tama
{
	enum class FontWeight
	{
		Normal = 400,//DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
		Bold = 700,//DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD,
		Thin = 100,//DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_THIN,

//		UnUsed0 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BLACK,
//		UnUsed1 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_DEMI_BOLD,
//		UnUsed2 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_BLACK,
//		UnUsed3 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_BOLD,
//		UnUsed4 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_LIGHT,
//		UnUsed5 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_HEAVY,
//		UnUsed6 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT,
//		UnUsed7 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM,
//		UnUsed8 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_REGULAR,
//		UnUsed9 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_BOLD,
//		UnUsed10 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_LIGHT,
//		UnUsed11 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_BLACK,
//		UnUsed12 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_BOLD,
//		UnUsed13 = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_LIGHT,
	};

	enum class FontStyle
	{
		Normal,	//= DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		Oblique,// = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE,
		Italic,	// = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC,
	};

	enum class FontStretch
	{
		Normal = 5,//DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		Medium = 5,//DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_MEDIUM,
		Expanded = 7,//DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_EXPANDED,
		Condenced = 3,//DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_CONDENSED,

//		UnUsed0 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
//		UnUsed1 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
//		UnUsed2 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_SEMI_CONDENSED,
//		UnUsed3 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_SEMI_EXPANDED,
//		UnUsed4 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
//		UnUsed5 = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_ULTRA_EXPANDED,
	};

	struct TextFormat
	{
		float size;
		std::wstring font;
		FontWeight weight;
		FontStyle style;
		FontStretch stretch;

		TextFormat(float _size = 30, 
			const std::wstring& _font = L"���C���I", 
			FontWeight _weight = FontWeight::Normal, 
			FontStyle _style = FontStyle::Normal,
			FontStretch _stretch = FontStretch::Normal) :
			size(_size), font(_font), weight(_weight), style(_style), stretch(_stretch)
		{}

		bool operator == (const TextFormat& other) const
		{
			return 
				(size == other.size &&
				font == other.font &&
				weight == other.weight &&
				style == other.style &&
				stretch == other.stretch);
		}

		bool operator != (const TextFormat& other)
		{
			return !(*this == other);
		}

		size_t GetHashKey() const
		{
			return std::hash<float>()(size) ^ std::hash<std::wstring>()(font);
		}
	};

	class FixedString;

	//�`��p������
	class Font
	{
		friend class FixedString;

		TextFormat m_format;
		std::wstring m_text;
	
	public:
		Font();
		Font(const TextFormat& format);

		void Draw(float x, float y, float w, float h, 
			const tama::ColorF& color = tama::Palette::White) const;

		/**
		*@brief �`�悷�镶�����ݒ肷��
		*@return �����񂪐ݒ肳�ꂽ����*/
		template<typename ...Args>
		const Font& operator ()(const Args& ...args) 
		{
			std::wstringstream stream;
			impl::get_text_impl(&stream, args...);
			m_text = stream.str();
			return *this;
		}

		/**
		*@brief �`�悷�镶�����ݒ肷��
		*@return �����񂪐ݒ肳�ꂽ����*/
		template<typename ...Args>
		const Font operator ()(const Args& ...args) const
		{
			std::wstringstream stream;
			impl::get_text_impl(&stream, args...);

			Font string(m_format);
			string.m_text = stream.str();
			return string;
		}

		/**
		*@brief �`�悷�镶�����ݒ肷��
		*@return �����񂪐ݒ肳�ꂽ����*/
		const Font& operator ()(const std::wstring& text)
		{
			m_text = text;
			return *this;
		}
	};

	/**@brief �����񂪌Œ肳�ꂽ�`��\������I�u�W�F�N�g
	*@detail �Œ肳��Ă��Ȃ��Ǝg���Ȃ��A1�������̃��C�A�E�g�̕ύX���\
	* �ɂȂ�\��
	*/
//	class FixedString
//	{
//		TextFormat m_format;
//		const std::wstring m_text;
//		
//		Microsoft::WRL::ComPtr<IDWriteTextLayout> m_layout;
//	public:
//		
//		FixedString(const std::wstring& text, const TextFormat& format); 

//		FixedString(Font&& str):
//			FixedString(str.m_text, str.m_format)
//		{}

//		void Draw() const
//		{}
//	};

	namespace CustomFont
	{
		void SetUp();

		/**
		*@detail 1. .rc�t�@�C���Ƀt�H���g�t�@�C����ǉ�����
					IDR_HOGE FONT filepath	�̌`��
		*@detail 2. resource.h��IDR_HOGE��萔�l�Ƃ���define����
					#define IDR_HOGE 100
		*@attention �������̃t�H�[�}�b�g�ł����g���Ȃ�
		*@param familyName ttf���J�����Ƃ��ɏ����Ă���t�H���g��(���{��ł����̂܂܂ŗǂ�)
		*/
		void Register(const std::wstring& familyName, uint32_t id);

		TextFormat Get(const std::wstring& familyName);
	}


	//�`��p�e�L�X�g
	//TODO: �t�H���g�̐ݒ肪�ł���悤��
//	struct String
//	{
//		TextFormat m_format;

//		std::wstring text;
//		float size;
//		D2D_POINT_2F pos;
//		D2D_COLOR_F color;

//		explicit String(float _size = 30.f, const D2D1::ColorF& _color = D2D1::ColorF(0x0)) :
//			text(L""), size(_size), pos(D2D1::Point2F()), color(_color)
//		{}

//		explicit String(const TextFormat& format);

//		template<typename ...Args>
//		const String& operator ()(const Args& ...args)
//		{
//			std::wstringstream stream;
//			get_text_impl(&stream, args...);
//			text = stream.str();
//			return *this;
//		}
//	};

//	struct Rect
//	{
//		D2D_POINT_2F pos, size;
//		bool fillRect;
//		D2D_COLOR_F color;

//		D2D_RECT_F GetRect()const
//		{
//			return D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
//		}
//		Rect() {};
//	};

//	class CRect
//	{
//		Rect rect;
//	public:
//		CRect(float x, float y, float w, float h,
//			const D2D1::ColorF& color = D2D1::ColorF(0, 0),
//			bool _fillRect = true)
//		{
//			rect.pos = D2D1::Point2F(x, y);
//			rect.size = D2D1::Point2F(w, h);
//			rect.color = color;
//			rect.fillRect = _fillRect;
//		}

//		operator Rect& ()
//		{
//			return rect;
//		}
//	};

//	struct Segment
//	{
//		
//	};
}
