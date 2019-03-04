#include "Geometry.h"
#include "../DX/D2D/Direct2D.h"

namespace tama
{
	template <class Type, class ElementType>
	void Rectangle<Type, ElementType>::Draw(const ColorF& color, float thickness) const
	{
		DX11::D2DManager::GetInstance()->DrawRect(
			(float)x, (float)y, (float)w, (float)h,
			thickness, 
			D2D1::ColorF{ color.x, color.y, color.z, color.w });
	}

	template <class Type, class ElementType>
	void Rectangle<Type, ElementType>::Fill(const ColorF& color) const
	{
		DX11::D2DManager::GetInstance()->FillRect(
			(float)x, (float)y, (float)w, (float)h,
			D2D1::ColorF{ color.x,color.y,color.z,color.w });
	}
	 
	template void Rectangle<Int2, int32_t>::Draw(const ColorF& color, float thickness) const;
	template void Rectangle<Vec2, float>::Draw(const ColorF& color, float thickness) const;

	template void Rectangle<Int2, int32_t>::Fill(const ColorF& color) const;
	template void Rectangle<Vec2, float>::Fill(const ColorF& color) const;

	void Line::Draw(const ColorF& color, float thickness) const
	{
		DX11::D2DManager::GetInstance()->DrawLine(
			begin.x, begin.y, end.x, end.y,
			thickness,
			D2D1::ColorF{ color.x, color.y,color.z,color.w });
	}
}