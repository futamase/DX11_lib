#pragma once

namespace tama
{
	inline Color& Color::operator = (const ColorF& col)
	{
		r = static_cast<uint32_t>(col.x * 255);
		g = static_cast<uint32_t>(col.y * 255);
		b = static_cast<uint32_t>(col.z * 255);
		a = static_cast<uint32_t>(col.w * 255);
		return *this;
	}

	inline Color::Color(const ColorF& col)
	{
		r = static_cast<uint32_t>(col.x * 255);
		g = static_cast<uint32_t>(col.y * 255);
		b = static_cast<uint32_t>(col.z * 255);
		a = static_cast<uint32_t>(col.w * 255);
	}

	inline ColorF::ColorF(const Color& col)
	{
		x = static_cast<float>(col.r / 255.f);
		y = static_cast<float>(col.g / 255.f);
		z = static_cast<float>(col.b / 255.f);
		w = static_cast<float>(col.a / 255.f);
	}

	inline ColorF& ColorF::operator= (const Color& col)
	{
		x = static_cast<float>(col.r / 255.f);
		y = static_cast<float>(col.g / 255.f);
		z = static_cast<float>(col.b / 255.f);
		w = static_cast<float>(col.a / 255.f);
		return *this;
	}

	inline bool ColorF::operator == (const ColorF& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorEqual(c1, c2);
	}

	inline bool ColorF::operator != (const ColorF& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorNotEqual(c1, c2);
	}

	inline ColorF& ColorF::operator+= (const ColorF& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorAdd(c1, c2));
		return *this;
	}

	inline ColorF& ColorF::operator-= (const ColorF& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorSubtract(c1, c2));
		return *this;
	}

	inline ColorF& ColorF::operator*= (const ColorF& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorMultiply(c1, c2));
		return *this;
	}

	inline ColorF& ColorF::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorScale(c, S));
		return *this;
	}

	inline ColorF& ColorF::operator/= (const ColorF& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorDivide(c1, c2));
		return *this;
	}

	inline ColorF ColorF::operator- () const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		ColorF R;
		XMStoreFloat4(&R, XMVectorNegate(c));
		return R;
	}

	inline ColorF operator+ (const ColorF& C1, const ColorF& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		ColorF R;
		XMStoreFloat4(&R, XMVectorAdd(c1, c2));
		return R;
	}

	inline ColorF operator- (const ColorF& C1, const ColorF& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		ColorF R;
		XMStoreFloat4(&R, XMVectorSubtract(c1, c2));
		return R;
	}

	inline ColorF operator* (const ColorF& C1, const ColorF& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		ColorF R;
		XMStoreFloat4(&R, XMVectorMultiply(c1, c2));
		return R;
	}

	inline ColorF operator* (const ColorF& C, float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(&C);
		ColorF R;
		XMStoreFloat4(&R, XMVectorScale(c, S));
		return R;
	}

	inline ColorF operator/ (const ColorF& C1, const ColorF& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		ColorF R;
		XMStoreFloat4(&R, XMVectorDivide(c1, c2));
		return R;
	}

	inline ColorF operator* (float S, const ColorF& C)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C);
		ColorF R;
		XMStoreFloat4(&R, XMVectorScale(c1, S));
		return R;
	}


	inline Vec3 ColorF::ToVec3() const
	{
		return Vec3(x, y, z);
	}

	inline Vec4 ColorF::ToVec4() const
	{
		return Vec4(x, y, z, w);
	}

	inline void ColorF::Negate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorNegative(c));
	}

	inline void ColorF::Negate(ColorF& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorNegative(c));
	}

	inline void ColorF::Saturate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorSaturate(c));
	}

	inline void ColorF::Saturate(ColorF& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMVectorSaturate(c));
	}

	inline void ColorF::Premultiply()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(this, XMVectorMultiply(c, a));
	}

	inline void ColorF::Premultiply(ColorF& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(&result, XMVectorMultiply(c, a));
	}

	inline void ColorF::AdjustSaturation(float sat)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustSaturation(c, sat));
	}

	inline void ColorF::AdjustSaturation(float sat, ColorF& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustSaturation(c, sat));
	}

	inline void ColorF::AdjustContrast(float contrast)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustContrast(c, contrast));
	}

	inline void ColorF::AdjustContrast(float contrast, ColorF& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustContrast(c, contrast));
	}

	inline void ColorF::Modulate(const ColorF& c1, const ColorF& c2, ColorF& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
	}

	inline ColorF ColorF::Modulate(const ColorF& c1, const ColorF& c2)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		ColorF result;
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
		return result;
	}

	inline void ColorF::Lerp(const ColorF& c1, const ColorF& c2, float t, ColorF& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
	}

	inline ColorF ColorF::Lerp(const ColorF& c1, const ColorF& c2, float t)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		ColorF result;
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
		return result;
	}

}
