#pragma once
#include <cstdint>
#include <iostream>
#include "Math.h"

namespace tama
{
	template<class Type, class ElementType>
	struct Rectangle;
	using Rect = Rectangle<tama::Int2, int32_t>;
	using RectF = Rectangle<tama::Vec2, float>;

	template <class Type, class ElementType>
	struct Rectangle
	{
#pragma warning(disable:4201)
		union
		{
			struct
			{
				ElementType x;
				ElementType y;
				ElementType w;
				ElementType h;
			};

			struct
			{
				Type pos;
				Type size;
			};
		};
#pragma warning(default:4201)

		Rectangle() {};

		explicit Rectangle(ElementType _size) :
			x(0), y(0), w(_size), h(_size)
		{}

		Rectangle(ElementType _w, ElementType _h) :
			x(0), y(0), w(_w), h(_h)
		{}

		explicit Rectangle(Type _size) :
			x(0), y(0), w(_size.x), h(_size.y)
		{}

		Rectangle(ElementType _x, ElementType _y, ElementType _w, ElementType _h) :
			x(_x), y(_y), w(_w), h(_h)
		{}

		Rectangle(ElementType _x, ElementType _y, ElementType _size) :
			x(_x), y(_y), w(_size), h(_size)
		{}

		Rectangle(ElementType _x, ElementType _y, Type _size) :
			x(_x), y(_y), w(_size.x), h(_size.y)
		{}

		Rectangle(Type _pos, ElementType _size) :
			x(_pos.x), y(_pos.y), w(_size), h(_size)
		{}

		Rectangle(Type _pos, ElementType _w, ElementType _h) :
			x(_pos.x), y(_pos.y), w(_w), h(_h)
		{}

		Rectangle(Type _pos, Type _size) :
			x(_pos.x), y(_pos.y), w(_size.x), h(_size.y)
		{}

		Rectangle(const Rectangle& other) { pos = other.pos; size = other.size; }
		Rectangle& operator = (const Rectangle& other) { pos = other.pos; size = other.size; return *this; }

		Rectangle& operator *= (ElementType s) { size *= s; return *this; }

		Rectangle& operator /= (ElementType s) { size /= s; return *this; }

		bool operator == (const Rectangle& r) const
		{
			return pos == r.pos
				&& size == r.size;
		}

		bool operator != (const Rectangle& r) const
		{
			return !(*this == r);
		}

		Rectangle& SetPos(ElementType _x, ElementType _y) 
		{ 
			pos.x = _x;
			pos.y = _y;
			return *this; 
		}

		Rectangle& SetPos(const Type& _pos) 
		{ 
			pos.x = _pos.x; 
			pos.y = _pos.y; 
			return *this; 
		}

		Rectangle& SetSize(ElementType _w, ElementType _h) 
		{
			size.x = _w; 
			size.y = _h; 
			return *this;
		}

		Rectangle& SetSize(const Type& _size)
		{
			size.x = _size.y; 
			size.y = _size.y; 
			return *this;
		}

		Rectangle& Set(ElementType _x, ElementType _y, ElementType _w, ElementType _h) 
		{
			pos.x = _x; 
			pos.y = _y; 
			size.x = _w; 
			size.y = _h; 
			return *this;
		}

		Rectangle& Set(const Type& _pos, ElementType _w, ElementType _h) 
		{
			pos = _pos; 
			size.x = _w;
			size.y = _h;
			return *this; 
		}

		Rectangle& Set(ElementType _x, ElementType _y, const Type& _size) 
		{ 
			pos.x = _x;
			pos.y = _y;
			size = _size; 
			return *this; 
		}

		Rectangle& Set(const Type& _pos, const Type& _size) 
		{ 
			pos = _pos; 
			size = _size; 
			return *this; 
		}

		Rectangle& Set(const Rectangle& rect) 
		{
			return *this = rect; 
		}

		Rectangle& Move(ElementType _x, ElementType _y)
		{
			pos.x += _x;
			pos.y += _y;
			return *this;
		}

		Rectangle& Move(const Type& v)
		{
			pos += v;
			return *this;
		}

		Rectangle Move(ElementType _x, ElementType _y) const
		{
			return{ pos.x + _x, pos.y + _y, size };
		}

		Rectangle Move(const Type& v) const
		{
			return{ pos + v, size };
		}

		Rectangle& SetCenter(ElementType _x, ElementType _y) 
		{
			pos = { _x - w / 2, _y - h / 2 };
			return *this; 
		}

		Rectangle& SetCenter(const Type& _pos) 
		{
			pos = { _pos - size / 2 };
			return *this; 
		}

		Rectangle Stretched(ElementType xy) const
		{
			return Stretched({ xy, xy });
		}

		Rectangle Stretched(ElementType _x, ElementType _y) const
		{
			return Stretched({ _x, _y });
		}

		Rectangle Stretched(const Type& xy) const
		{
			return{ pos - xy, size + xy * 2 };
		}

		Rectangle Stretched(ElementType top, ElementType right, ElementType bottom, ElementType left) const
		{
			return{ pos.x - left, pos.y - top, size.x + left + right, size.y + top + bottom };
		}

		RectF Scaled(float s) const
		{
			return RectF(x + w*0.5, y + h*0.5, w*s, h*s);
		}

		Rectangle Scaled(ElementType sx, ElementType sy) const
		{
			return RectF(x + w*0.5, y + h*0.5, w*sx, h*sy);
		}

		Rectangle Scaled(const Type& s) const
		{
			return RectF(x + w*0.5, y + h*0.5, w*s.x, h*s.y);
		}

		ElementType Area() const { return w*h; }

		ElementType Perimeter() const { return (w + h) * 2; }

		void Draw(const ColorF& color = Palette::White, float thickness = 1.f) const;

		void Fill(const ColorF& color = Palette::White) const;
	};

	template<class Type, class ElementType>
	std::wostream &operator<<(std::wostream &out, const Rectangle<Type, ElementType>& rect)
	{
		out << "(" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << ")";
		return out;
	}

	struct Line
	{
		tama::Vec2 begin, end;

		Line() = default;

		Line(float x0, float y0, float x1, float y1)
			: begin(x0, y0)
			, end(x1, y1) {}

		Line(const tama::Vec2& p0, float x1, float y1)
			: begin(p0)
			, end(x1, y1) {}

		Line(float x0, float y0, const tama::Vec2& p1)
			: begin(x0, y0)
			, end(p1) {}

		Line(const tama::Vec2& p0, const tama::Vec2& p1)
			: begin(p0)
			, end(p1) {}

		Line& Set(float x0, float y0, float x1, float y1)
		{
			begin.x = x0;
			begin.y = y0;
			end.x = x1; 
			end.y = y1;
			return *this;
		}

		Line& Set(const tama::Vec2& p0, float x1, float y1)
		{
			begin = p0;
			end.x = x1;
			end.y = y1;
			return *this;
		}

		Line& Set(float x0, float y0, const tama::Vec2& p1)
		{
			begin.x = x0;
			begin.y = y0;
			end = p1;
			return *this;
		}

		Line& Set(const tama::Vec2& p0, const tama::Vec2& p1)
		{
			begin = p0;
			end = p1;
			return *this;
		}

		Line& Set(const Line& line)
		{
			return *this = line;
		}

		Line& Move(float x, float y) 
		{
			begin += {x, y};
			end += {x, y};
			return *this;
		}

		Line& Move(const tama::Vec2& v)
		{
			begin += v;
			end += v;
			return *this;
		}

		Line Move(float x, float y) const
		{
			return{ begin + tama::Vec2(x,y), end + tama::Vec2(x,y) };
		}

		Line Move(const tama::Vec2& v) const
		{
			return{ begin + v, end + v };
		}

		float Length() const
		{
			tama::Vec2::Distance(begin, end);
		}

		void Draw(const ColorF& color = Palette::White, float thickness = 1.f) const;
	};

	struct Circle
	{

	};

	struct Elipse
	{

	};

}
