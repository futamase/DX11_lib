#include "Math.h"

namespace tama
{
	std::wostream &operator<<(std::wostream &out, const Vec2& v)
	{
		out << L"(" << v.x << L"," << v.y  << L")";
		return out;
	}

	std::wostream &operator<<(std::wostream &out, const Vec3& v)
	{
		out << L"(" << v.x << L"," << v.y  << L"," << v.z << L")";
		return out;
	}
}