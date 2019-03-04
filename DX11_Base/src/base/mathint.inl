#pragma once

namespace tama 
{
	// ###################Bein of Int2####################

	inline bool Int2::operator == (const Int2& i) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR v2 = XMLoadSInt2(&i);
		return XMVector2Equal(v1, v2);
	}

	inline bool Int2::operator != (const Int2& i) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR v2 = XMLoadSInt2(&i);
		return XMVector2NotEqual(v1, v2);
	}

	inline Int2& Int2::operator+= (const Int2& i)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR v2 = XMLoadSInt2(&i);
		XMVECTOR x = XMVectorAdd(v1, v2);
		XMStoreSInt2(this, x);
		return *this;
	}

	inline Int2& Int2::operator-= (const Int2& i)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR v2 = XMLoadSInt2(&i);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		XMStoreSInt2(this, x);
		return *this;
	}

	inline Int2& Int2::operator*= (const Int2& i)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR v2 = XMLoadSInt2(&i);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		XMStoreSInt2(this, x);
		return *this;
	}

	inline Int2& Int2::operator*= (int32_t S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(S));
		XMStoreSInt2(this, x);
		return *this;
	}

	inline Int2& Int2::operator/= (int32_t S)
	{
		using namespace DirectX;
		assert(S != 0);
		XMVECTOR v1 = XMLoadSInt2(this);
		XMVECTOR x = XMVectorScale(v1, 1.f / static_cast<float>(S));
		XMStoreSInt2(this, x);
		return *this;
	}

	inline void Int2::Min(const Int2& i1, const Int2& i2, Int2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt2(&i1);
		XMVECTOR x2 = XMLoadSInt2(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);
		XMStoreSInt2(&result, x);
	}

	inline Int2 Int2::Min(const Int2& i1, const Int2& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt2(&i1);
		XMVECTOR x2 = XMLoadSInt2(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);

		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline void Int2::Max(const Int2& i1, const Int2& i2, Int2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt2(&i1);
		XMVECTOR x2 = XMLoadSInt2(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);
		XMStoreSInt2(&result, x);
	}

	inline Int2 Int2::Max(const Int2& i1, const Int2& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt2(&i1);
		XMVECTOR x2 = XMLoadSInt2(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);

		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator+ (const Int2& I1, const Int2& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I1);
		XMVECTOR v2 = XMLoadSInt2(&I2);
		XMVECTOR x = XMVectorAdd(v1, v2);
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator- (const Int2& I1, const Int2& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I1);
		XMVECTOR v2 = XMLoadSInt2(&I2);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator* (const Int2& I1, const Int2& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I1);
		XMVECTOR v2 = XMLoadSInt2(&I2);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator* (const Int2& I1, int32_t& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I1);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(I2));
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator/ (const Int2& I1, const Int2& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I1);
		XMVECTOR v2 = XMLoadSInt2(&I2);
		XMVECTOR x = XMVectorDivide(v1, v2);
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}

	inline Int2 operator* (int32_t i, const Int2& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt2(&I);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(i));
		Int2 r;
		XMStoreSInt2(&r, x);
		return r;
	}
	//#################End of Int2#####################

	//#################Begin of Int3###################
	inline bool Int3::operator == (const Int3& I) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR v2 = XMLoadSInt3(&I);
		return XMVector2Equal(v1, v2);
	}

	inline bool Int3::operator != (const Int3& I) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR v2 = XMLoadSInt3(&I);
		return XMVector2NotEqual(v1, v2);
	}

	inline Int3& Int3::operator += (const Int3& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR v2 = XMLoadSInt3(&I);
		XMVECTOR x = XMVectorAdd(v1, v2);
		XMStoreSInt3(this, x);
		return *this;
	}

	inline Int3& Int3::operator -= (const Int3& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR v2 = XMLoadSInt3(&I);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		XMStoreSInt3(this, x);
		return *this;
	}

	inline Int3& Int3::operator *= (const Int3& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR v2 = XMLoadSInt3(&I);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		XMStoreSInt3(this, x);
		return *this;
	}

	inline Int3& Int3::operator *= (int32_t I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(I));
		XMStoreSInt3(this, x);
		return *this;
	}

	inline Int3& Int3::operator /= (int32_t S)
	{
		using namespace DirectX;
		assert(S != 0);
		XMVECTOR v1 = XMLoadSInt3(this);
		XMVECTOR x = XMVectorScale(v1, 1.f / static_cast<float>(S));
		XMStoreSInt3(this, x);
		return *this;
	}

	inline Int3 operator + (const Int3& I1, const Int3& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I1);
		XMVECTOR v2 = XMLoadSInt3(&I2);
		XMVECTOR x = XMVectorAdd(v1, v2);
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline Int3 operator - (const Int3& I1, const Int3& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I1);
		XMVECTOR v2 = XMLoadSInt3(&I2);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline Int3 operator * (const Int3& I1, const Int3& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I1);
		XMVECTOR v2 = XMLoadSInt3(&I2);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline Int3 operator * (const Int3& I1, int32_t I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I1);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(I2));
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline Int3 operator / (const Int3& I1, const Int3& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I1);
		XMVECTOR v2 = XMLoadSInt3(&I2);
		XMVECTOR x = XMVectorDivide(v1, v2);
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline Int3 operator * (int32_t i, const Int3& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt3(&I);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(i));
		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline void Int3::Min(const Int3& i1, const Int3& i2, Int3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt3(&i1);
		XMVECTOR x2 = XMLoadSInt3(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);
		XMStoreSInt3(&result, x);
	}

	inline Int3 Int3::Min(const Int3& i1, const Int3& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt3(&i1);
		XMVECTOR x2 = XMLoadSInt3(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);

		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	inline void Int3::Max(const Int3& i1, const Int3& i2, Int3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt3(&i1);
		XMVECTOR x2 = XMLoadSInt3(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);
		XMStoreSInt3(&result, x);
	}

	inline Int3 Int3::Max(const Int3& i1, const Int3& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt3(&i1);
		XMVECTOR x2 = XMLoadSInt3(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);

		Int3 r;
		XMStoreSInt3(&r, x);
		return r;
	}

	// ####################End of Int3########################
	// ####################Begin of Int4######################

	inline bool Int4::operator == (const Int4& I) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR v2 = XMLoadSInt4(&I);
		return XMVector2Equal(v1, v2);
	}

	inline bool Int4::operator != (const Int4& I) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR v2 = XMLoadSInt4(&I);
		return XMVector2NotEqual(v1, v2);
	}

	inline Int4& Int4::operator+= (const Int4& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR v2 = XMLoadSInt4(&I);
		XMVECTOR x = XMVectorAdd(v1, v2);
		XMStoreSInt4(this, x);
		return *this;
	}

	inline Int4& Int4::operator-= (const Int4& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR v2 = XMLoadSInt4(&I);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		XMStoreSInt4(this, x);
		return *this;
	}

	inline Int4& Int4::operator*= (const Int4& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR v2 = XMLoadSInt4(&I);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		XMStoreSInt4(this, x);
		return *this;
	}

	inline Int4& Int4::operator*= (int32_t I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(I));
		XMStoreSInt4(this, x);
		return *this;
	}

	inline Int4& Int4::operator/= (int32_t I)
	{
		using namespace DirectX;
		assert(I != 0);
		XMVECTOR v1 = XMLoadSInt4(this);
		XMVECTOR x = XMVectorScale(v1, 1.f / static_cast<float>(I));
		XMStoreSInt4(this, x);
		return *this;
	}

	inline Int4 operator+ (const Int4& I1, const Int4& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I1);
		XMVECTOR v2 = XMLoadSInt4(&I2);
		XMVECTOR x = XMVectorAdd(v1, v2);
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline Int4 operator- (const Int4& I1, const Int4& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I1);
		XMVECTOR v2 = XMLoadSInt4(&I2);
		XMVECTOR x = XMVectorSubtract(v1, v2);
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline Int4 operator* (const Int4& I1, const Int4& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I1);
		XMVECTOR v2 = XMLoadSInt4(&I2);
		XMVECTOR x = XMVectorMultiply(v1, v2);
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline Int4 operator* (const Int4& I1, int32_t I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I1);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(I));
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline Int4 operator/ (const Int4& I1, const Int4& I2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I1);
		XMVECTOR v2 = XMLoadSInt4(&I2);
		XMVECTOR x = XMVectorDivide(v1, v2);
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline Int4 operator* (int32_t i, const Int4& I)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadSInt4(&I);
		XMVECTOR x = XMVectorScale(v1, static_cast<float>(i));
		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline void Int4::Min(const Int4& i1, const Int4& i2, Int4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt4(&i1);
		XMVECTOR x2 = XMLoadSInt4(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);
		XMStoreSInt4(&result, x);
	}

	inline Int4 Int4::Min(const Int4& i1, const Int4& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt4(&i1);
		XMVECTOR x2 = XMLoadSInt4(&i2);
		XMVECTOR x = XMVectorMin(x1, x2);

		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}

	inline void Int4::Max(const Int4& i1, const Int4& i2, Int4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt4(&i1);
		XMVECTOR x2 = XMLoadSInt4(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);
		XMStoreSInt4(&result, x);
	}

	inline Int4 Int4::Max(const Int4& i1, const Int4& i2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadSInt4(&i1);
		XMVECTOR x2 = XMLoadSInt4(&i2);
		XMVECTOR x = XMVectorMax(x1, x2);

		Int4 r;
		XMStoreSInt4(&r, x);
		return r;
	}
}

