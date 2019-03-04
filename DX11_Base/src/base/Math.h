#pragma once

#include <iostream>

#include <DirectXTK\SimpleMath.h>

namespace tama 
{

	using Vec2 = DirectX::SimpleMath::Vector2;
	using Vec3 = DirectX::SimpleMath::Vector3;
	using Vec4 = DirectX::SimpleMath::Vector4;

	std::wostream &operator<<(std::wostream &out, const Vec2& v);

	std::wostream &operator<<(std::wostream &out, const Vec3& v);

	using DirectX::SimpleMath::Plane;
	using DirectX::SimpleMath::Matrix;
	using DirectX::SimpleMath::Quaternion;
	using DirectX::SimpleMath::Ray;

	struct Int2 : public DirectX::XMINT2
	{
		Int2() : XMINT2(0, 0) {}
		explicit Int2(int32_t x) : XMINT2(x, x) {}
		Int2(int32_t _x, int32_t _y) : XMINT2(_x, _y) {}
		explicit Int2(_In_reads_(2) const int32_t* pArray) : XMINT2(pArray) {}

		bool operator == (const Int2& i) const;
		bool operator != (const Int2& i) const;

		Int2& operator= (const Int2& i) { x = i.x; y = i.y; return *this; }
		Int2& operator+= (const Int2& i);
		Int2& operator-= (const Int2& i);
		Int2& operator*= (const Int2& i);
		Int2& operator*= (int32_t S);
		Int2& operator/= (int32_t S);

		Int2 operator + () const { return *this; }
		Int2 operator - () const { return{ -x,-y }; }

		static void Min(const Int2& i1, const Int2& i2, Int2& result);
		static Int2 Min(const Int2& i1, const Int2& i2);

		static void Max(const Int2& i1, const Int2& i2, Int2& result);
		static Int2 Max(const Int2& i1, const Int2& i2);
	};

	Int2 operator+ (const Int2& I1, const Int2& I2);
	Int2 operator- (const Int2& I1, const Int2& I2);
	Int2 operator* (const Int2& I1, const Int2& I2);
	Int2 operator* (const Int2& I, int32_t S);
	Int2 operator/ (const Int2& I1, const Int2& I2);
	Int2 operator* (int32_t S, const Int2& V);

	struct Int3 : public DirectX::XMINT3
	{
		Int3() : XMINT3(0, 0, 0) {}
		explicit Int3(int32_t x) : XMINT3(x, x, x) {}
		Int3(int32_t _x, int32_t _y, int32_t _z) : XMINT3(_x, _y, _z) {}
		explicit Int3(_In_reads_(3) const int32_t* pArray) : XMINT3(pArray) {}

		bool operator == (const Int3& I) const;
		bool operator != (const Int3& I) const;

		Int3& operator = (const Int3& I) { x = I.x; y = I.y; z = I.z; return *this; }
		Int3& operator += (const Int3& I);
		Int3& operator -= (const Int3& I);
		Int3& operator *= (const Int3& I);
		Int3& operator *= (int32_t S);
		Int3& operator /= (int32_t S);

		Int3 operator + () const { return *this; }
		Int3 operator - () const { return{ -x, -y, -z }; }

		static void Min(const Int3& i1, const Int3& i2, Int3& result);
		static Int3 Min(const Int3& i1, const Int3& i2);

		static void Max(const Int3& i1, const Int3& i2, Int3& result);
		static Int3 Max(const Int3& i1, const Int3& i2);
	};

	Int3 operator+ (const Int3& I1, const Int3& I2);
	Int3 operator- (const Int3& I1, const Int3& I2);
	Int3 operator* (const Int3& I1, const Int3& I2);
	Int3 operator* (const Int3& I1, int32_t I2);
	Int3 operator/ (const Int3& I1, const Int3& I2);
	Int3 operator* (int32_t i, const Int3& I);

	struct Int4 : public DirectX::XMINT4
	{
		Int4() : XMINT4(0, 0, 0, 0) {}
		explicit Int4(int32_t x) : XMINT4(x, x, x, x) {}
		Int4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : XMINT4(_x, _y, _z, _w) {}
		explicit Int4(_In_reads_(4) const int32_t* pArray) : XMINT4(pArray) {}

		bool operator == (const Int4& I) const;
		bool operator != (const Int4& I) const;

		Int4& operator= (const Int4& I) { x = I.x; y = I.y; z = I.z; w = I.w; return *this; }
		Int4& operator+= (const Int4& I);
		Int4& operator-= (const Int4& I);
		Int4& operator*= (const Int4& I);
		Int4& operator*= (int32_t I);
		Int4& operator/= (int32_t I);

		Int4 operator+ () const { return *this; }
		Int4 operator- () const { return{ -x, -y, -z, -w }; }

		static void Min(const Int4& i1, const Int4& i2, Int4& result);
		static Int4 Min(const Int4& i1, const Int4& i2);

		static void Max(const Int4& i1, const Int4& i2, Int4& result);
		static Int4 Max(const Int4& i1, const Int4& i2);
	};

	Int4 operator+ (const Int4& I1, const Int4& I2);
	Int4 operator- (const Int4& I1, const Int4& I2);
	Int4 operator* (const Int4& I1, const Int4& I2);
	Int4 operator* (const Int4& I, int32_t i);
	Int4 operator/ (const Int4& I1, const Int4& I2);
	Int4 operator* (int32_t i, const Int4& I);

	class ColorF;

	enum class Palette : uint32_t
	{
		AliceBlue = 0xF0F8FF,
		AntiqueWhite = 0xFAEBD7,
		Aqua = 0x00FFFF,
		Aquamarine = 0x7FFFD4,
		Azure = 0xF0FFFF,
		Beige = 0xF5F5DC,
		Bisque = 0xFFE4C4,
		Black = 0x000000,
		BlanchedAlmond = 0xFFEBCD,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		BurlyWood = 0xDEB887,
		CadetBlue = 0x5F9EA0,
		Chartreuse = 0x7FFF00,
		Chocolate = 0xD2691E,
		Coral = 0xFF7F50,
		CornflowerBlue = 0x6495ED,
		Cornsilk = 0xFFF8DC,
		Crimson = 0xDC143C,
		Cyan = 0x00FFFF,
		DarkBlue = 0x00008B,
		DarkCyan = 0x008B8B,
		DarkGoldenrod = 0xB8860B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkKhaki = 0xBDB76B,
		DarkMagenta = 0x8B008B,
		DarkOliveGreen = 0x556B2F,
		DarkOrange = 0xFF8C00,
		DarkOrchid = 0x9932CC,
		DarkRed = 0x8B0000,
		DarkSalmon = 0xE9967A,
		DarkSeaGreen = 0x8FBC8F,
		DarkSlateBlue = 0x483D8B,
		DarkSlateGray = 0x2F4F4F,
		DarkTurquoise = 0x00CED1,
		DarkViolet = 0x9400D3,
		DeepPink = 0xFF1493,
		DeepSkyBlue = 0x00BFFF,
		DimGray = 0x696969,
		DodgerBlue = 0x1E90FF,
		Firebrick = 0xB22222,
		FloralWhite = 0xFFFAF0,
		ForestGreen = 0x228B22,
		Fuchsia = 0xFF00FF,
		Gainsboro = 0xDCDCDC,
		GhostWhite = 0xF8F8FF,
		Gold = 0xFFD700,
		Goldenrod = 0xDAA520,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		Honeydew = 0xF0FFF0,
		HotPink = 0xFF69B4,
		IndianRed = 0xCD5C5C,
		Indigo = 0x4B0082,
		Ivory = 0xFFFFF0,
		Khaki = 0xF0E68C,
		Lavender = 0xE6E6FA,
		LavenderBlush = 0xFFF0F5,
		LawnGreen = 0x7CFC00,
		LemonChiffon = 0xFFFACD,
		LightBlue = 0xADD8E6,
		LightCoral = 0xF08080,
		LightCyan = 0xE0FFFF,
		LightGoldenrodYellow = 0xFAFAD2,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSalmon = 0xFFA07A,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightSlateGray = 0x778899,
		LightSteelBlue = 0xB0C4DE,
		LightYellow = 0xFFFFE0,
		Lime = 0x00FF00,
		LimeGreen = 0x32CD32,
		Linen = 0xFAF0E6,
		Magenta = 0xFF00FF,
		Maroon = 0x800000,
		MediumAquamarine = 0x66CDAA,
		MediumBlue = 0x0000CD,
		MediumOrchid = 0xBA55D3,
		MediumPurple = 0x9370DB,
		MediumSeaGreen = 0x3CB371,
		MediumSlateBlue = 0x7B68EE,
		MediumSpringGreen = 0x00FA9A,
		MediumTurquoise = 0x48D1CC,
		MediumVioletRed = 0xC71585,
		MidnightBlue = 0x191970,
		MintCream = 0xF5FFFA,
		MistyRose = 0xFFE4E1,
		Moccasin = 0xFFE4B5,
		NavajoWhite = 0xFFDEAD,
		Navy = 0x000080,
		OldLace = 0xFDF5E6,
		Olive = 0x808000,
		OliveDrab = 0x6B8E23,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Orchid = 0xDA70D6,
		PaleGoldenrod = 0xEEE8AA,
		PaleGreen = 0x98FB98,
		PaleTurquoise = 0xAFEEEE,
		PaleVioletRed = 0xDB7093,
		PapayaWhip = 0xFFEFD5,
		PeachPuff = 0xFFDAB9,
		Peru = 0xCD853F,
		Pink = 0xFFC0CB,
		Plum = 0xDDA0DD,
		PowderBlue = 0xB0E0E6,
		Purple = 0x800080,
		Red = 0xFF0000,
		RosyBrown = 0xBC8F8F,
		RoyalBlue = 0x4169E1,
		SaddleBrown = 0x8B4513,
		Salmon = 0xFA8072,
		SandyBrown = 0xF4A460,
		SeaGreen = 0x2E8B57,
		SeaShell = 0xFFF5EE,
		Sienna = 0xA0522D,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		SlateBlue = 0x6A5ACD,
		SlateGray = 0x708090,
		Snow = 0xFFFAFA,
		SpringGreen = 0x00FF7F,
		SteelBlue = 0x4682B4,
		Tan = 0xD2B48C,
		Teal = 0x008080,
		Thistle = 0xD8BFD8,
		Tomato = 0xFF6347,
		Turquoise = 0x40E0D0,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Yellow = 0xFFFF00,
		YellowGreen = 0x9ACD32,
	};

	struct Color
	{
		uint32_t r : 8;
		uint32_t g : 8;
		uint32_t b : 8;
		uint32_t a : 8;

# pragma warning ( default: 4201 )

		Color() = default;

		Color(uint32_t _r, uint32_t _g, uint32_t _b, uint32_t _a = 255) :
			r(_r & 0xFF), g(_g & 0xFF), b(_b & 0xFF), a(_a & 0xFF)
		{}

		explicit Color(uint32_t rgb) :
			r(rgb & 0xFF), g(rgb & 0xFF), b(rgb & 0xFF), a(0xFF)
		{}

		Color(uint32_t _rgb, uint32_t _a) :
			r(_rgb & 0xFF), g(_rgb & 0xFF), b(_rgb & 0xFF), a(_a & 0xFF)
		{}

		Color(const Color& rgb, uint32_t _a) :
			r(rgb.r), g(rgb.g), b(rgb.b), a(_a)
		{}

		Color(Palette knownColor, uint32_t alpha)
		{
			r = static_cast<uint32_t>(knownColor) & 0x0000ff >> 16;
			g = static_cast<uint32_t>(knownColor) & 0x00ff00 >> 8;
			b = static_cast<uint32_t>(knownColor) & 0xff0000 >> 0;
			a = alpha;
		}

		Color(const Color& col) = default;

		Color(const ColorF& col);

		Color& operator = (const Color& col) = default;

		Color& operator = (const ColorF& col);

		Color& operator = (uint32_t rgb)
		{
			r = g = b = (rgb & 0xFF);
			a = 255;
			return *this;
		}

		Color operator ~ () const
		{
			return{ ~r,~g,~b,a };
		}

		bool operator == (const Color& col) const
		{
			return *static_cast<const uint32_t*>(static_cast<const void*>(this))
				== *static_cast<const uint32_t*>(static_cast<const void*>(&col));
		}

		bool operator != (const Color& col) const
		{
			return !(*this == col);
		}

		Color& Set(uint32_t _r, uint32_t _g, uint32_t _b, uint32_t _a = 255)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
			return *this;
		}

		Color& SetRGB(uint32_t rgb)
		{
			r = g = b = rgb;
			return *this;
		}

		Color& SetAlpha(uint32_t alpha)
		{
			a = alpha;
			return *this;
		}

		uint8_t Grayscale() const
		{
			return static_cast<uint8_t>(0.299f*r + 0.587f*g + 0.114f*b);
		}

		float GrayscaleF() const
		{
			return (0.299f / 255.0f*r + 0.587f / 255.0f*g + 0.114f / 255.0f*b);
		}

		Color Lerp(const Color& other, float f) const
		{
			return{ static_cast<uint32_t>(int32_t(r) + (int32_t(other.r) - int32_t(r)) * f),
					static_cast<uint32_t>(int32_t(g) + (int32_t(other.g) - int32_t(g)) * f),
					static_cast<uint32_t>(int32_t(b) + (int32_t(other.b) - int32_t(b)) * f),
					static_cast<uint32_t>(int32_t(a) + (int32_t(other.a) - int32_t(a)) * f) };
		}

	};

	class ColorF : public DirectX::XMFLOAT4
	{
	public:

		ColorF() :
			XMFLOAT4(0, 0, 0, 1)
		{}

		ColorF(float _r, float _g, float _b, float _a = 1.0) :
			XMFLOAT4(_r, _g, _b, _a)
		{}

		explicit ColorF(const Vec3& color) :
			XMFLOAT4(color.x, color.y, color.z, 1.0)
		{}

		explicit ColorF(const Vec4& color) :
			XMFLOAT4(color.x, color.y, color.z, color.w)
		{}

		explicit ColorF(uint32_t rgb, float a = 1.0)
		{
			Init(rgb, a);
		}

		ColorF(Palette knownColor, float a = 1.0)
		{
			Init(static_cast<uint32_t>(knownColor), a);
		}

		ColorF(const Color& col);

		float R() const { return x; }
		void R(float _r) { x = _r; }

		float G() const { return y; }
		void G(float _g) { y = _g; }

		float B() const { return z; }
		void B(float _b) { z = _b; }

		float A() const { return w; }
		void A(float _a) { w = _a; }

		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(this); }

		bool operator == (const ColorF& c) const;
		bool operator != (const ColorF& c) const;

		ColorF& operator= (const ColorF& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
		ColorF& operator= (const Color& c);
		ColorF& operator+= (const ColorF& c);
		ColorF& operator-= (const ColorF& c);
		ColorF& operator*= (const ColorF& c);
		ColorF& operator*= (float S);
		ColorF& operator/= (const ColorF& c);

		ColorF operator+ () const { return *this; }
		ColorF operator- () const;

		Vec3 ToVec3() const;
		Vec4 ToVec4() const;

		void Negate();
		void Negate(ColorF& result) const;

		void Saturate();
		void Saturate(ColorF& result) const;

		void Premultiply();
		void Premultiply(ColorF& result) const;

		void AdjustSaturation(float sat);
		void AdjustSaturation(float sat, ColorF& result) const;

		void AdjustContrast(float contrast);
		void AdjustContrast(float contrast, ColorF& result) const;

		static void Modulate(const ColorF& c1, const ColorF& c2, ColorF& result);
		static ColorF Modulate(const ColorF& c1, const ColorF& c2);

		static void Lerp(const ColorF& c1, const ColorF& c2, float t, ColorF& result);
		static ColorF Lerp(const ColorF& c1, const ColorF& c2, float t);


	private:
		void Init(uint32_t rgb, float alpha)
		{
			x = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
			y = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
			z = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
			w = alpha;
		}

		static const uint32_t sc_redShift = 16;
		static const uint32_t sc_greenShift = 8;
		static const uint32_t sc_blueShift = 0;

		static const uint32_t sc_redMask = 0xff << sc_redShift;
		static const uint32_t sc_greenMask = 0xff << sc_greenShift;
		static const uint32_t sc_blueMask = 0xff << sc_blueShift;
	};

	ColorF operator+ (const ColorF& C1, const ColorF& C2);
	ColorF operator- (const ColorF& C1, const ColorF& C2);
	ColorF operator* (const ColorF& C1, const ColorF& C2);
	ColorF operator* (const ColorF& C, float S);
	ColorF operator/ (const ColorF& C1, const ColorF& C2);
	ColorF operator* (float S, const ColorF& C);
}

#include "MathInt.inl"
#include "Color.inl"