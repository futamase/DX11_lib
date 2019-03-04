#pragma once

#include <cmath>

#include <DirectXMath.h>

//イージング関数
namespace Easing
{
	using EasingFunc = float(float, float, float, float);


	inline float InQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max*t*t + min;
	}

	inline float OutQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max*t*(t - 2) + min;
	}

	inline float InOutQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime * 2.f;
		if (t < 1.f)
			return max / 2.f * t * t + min;
		else
			return -max / 2.f * ((t-1.f) * (t - 3.f) - 1.f) + min;
	}

	inline float InCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * std::pow(t,3) + min;
	}

	inline float OutCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1.f;
		return max * (std::powf(t, 3) + 1.f) + min;
	}

	inline float InOutCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime * 2.f;
		if (t < 1)
			return max / 2.f * t*t*t + min;
		else
		{
			t -= 2.f;
			return max / 2.f * (std::pow(t,3) + 2.f) + min;
		}
	}

	inline float InQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * std::pow(t,4) + min;
	}

	inline float OutQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return -max*(std::pow(t,4) - 1) + min;
	}

	inline float InOutQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime * 2.f;
		if (t < 1)
			return max / 2.f * std::pow(t,4) + min;
		{
			t -= 2;
			return -max / 2.f * (std::pow(t, 4) - 2.f) + min;
		}
	}

	inline float InQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * std::pow(t,5) + min;
	}

	inline float OutQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (std::pow(t,5) + 1) + min;
	}

	inline float InOutQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime * 2.f;
		if (t < 1)
			return max / 2 * std::pow(t,5) + min;
		else
		{
			t -= 2;
			return max / 2 * (std::pow(t, 5) + 2) + min;
		}
	}

	inline float InSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max*std::cos(t * DirectX::XM_PIDIV2 / totaltime) + max + min;
	}

	inline float OutSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return max * std::sin(t*DirectX::XM_PIDIV2 / totaltime) + min;
	}

	inline float InOutSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max / 2 * (std::cos(t*DirectX::XM_PI / totaltime) - 1) + min;
	}

	inline float InExp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == 0.f ? 
			min :
			max * std::powf(2.f, 10.f * (t / totaltime - 1.f)) + min - max * 0.001f;
	}

	inline float OutExp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == totaltime ?
			max + min : 
			max * 1.001f * (-std::powf(2.f, -10 * t / totaltime) + 1) + min;
	}

	inline float InOutExp(float t, float totaltime, float max, float min)
	{
		if (t == 0.0f)
			return min;
		if (t == totaltime)
			return max + min;
		max -= min;
		t = t / totaltime * 2.f;

		if (t < 1)
		{
			return max / 2.f * std::powf(2.f, 10 * (t - 1)) + min - max * 0.0005f;
		}
		else
		{
			--t;
			return max / 2.f * 1.0005f * (-std::powf(2.f, -10 * t) + 2) + min;
		}

	}

	inline float InCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max*(std::sqrt(1 - t*t) - 1) + min;
	}

	inline float OutCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max*std::sqrt(1 - t*t) + min;
	}

	inline float InOutCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime * 2.f;
		if (t < 1)
			return -max / 2 * (std::sqrt(1 - t*t) - 1) + min;
		else
		{
			t -= 2;
			return max / 2 * (std::sqrt(1 - t*t) + 1) + min;
		}
	}

	inline float InElastic(float t, float totaltime, float max, float min)
	{
		float a = 1.f;
		float p = 0.3f;

		if (t == 0)
			return min;

		t /= totaltime;

		if (t == 1)
			return max + min;

		if (p != 0)
			p = totaltime * 0.3f;

		float s;

		if (a != 0 || a < std::fabs(max))
		{
			a = max;
			s = p / 4.f;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * std::asinf(max / a);
		}

		--t;

		return -(a * std::powf(2.f, 10.f * t) * std::sinf((t * totaltime - s) * (2.f * DirectX::XM_PI) / p)) + min;
	}

	inline float OutElastic(float t, float totaltime, float max, float min)
	{
		float a = 1.f;
		float p = 0.3f;

		if (t == 0)
			return min;

		t /= totaltime;

		if (t == 1)
			return max + min;

		if (p != 0)
			p = totaltime * 0.3f;

		float s;

		if (a != 0 || a < std::fabs(max))
		{
			a = max;
			s = p / 4.f;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * std::asinf(max / a);
		}

		--t;

		return a * std::powf(2, -10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p) + min + max;
	}

	inline float InOutElastic(float t, float totaltime, float max, float min)
	{
		float a = 1.f;
		float p = 0.3f;

		if (t == 0)
			return min;

		t = t / totaltime * 2.f;

		if (t == 2)
			return max + min;

		if (p != 0)
			p = totaltime * (0.3f * 1.5f);
		if (a != 0)
			a = 0;

		float s;

		if (a != 0 || a < std::fabs(max))
		{
			a = max;
			s = p / 4.f;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * std::asinf(max / a);
		}

		if (t < 1.f)
		{
			--t;
			return -0.5f * (a * std::powf(2, 10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p)) + min;
		}
		else
		{
			--t;
			return a * std::powf(2, -10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p) * 0.5f + min + max;
		}

	}

	inline float InBack(float t, float totaltime, float max, float min)
	{
		const float s = 1.70158f;
		max -= min;
		t /= totaltime;
		return max*t*t*((s + 1)*t - s) + min;
	}

	inline float OutBack(float t, float totaltime, float max, float min)
	{
		const float s = 1.70158f;
		max -= min;
		t = t / totaltime - 1;
		return max*(t*t*((s + 1)*t*s) + 1) + min;
	}

	inline float InOutBack(float t, float totaltime, float max, float min)
	{
		const float s = 1.70158f * 1.525f;
		max -= min;
		t = t / totaltime * 2;
		if (t < 1)
		{
			return max / 2 * (t*t*((s + 1)*t - s)) + min;
		}
		t -= 2;
		return max / 2 * (t*t*((s + 1)*t + s) + 2) + min;
	}

	inline float OutBounce(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;

		if (t < 1 / 2.75f)
			return max*(7.5625f*t*t) + min;
		else if (t < 2 / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return max*(7.5625f*t*t + 0.75f) + min;
		}
		else if (t < 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return max*(7.5625f*t*t + 0.9375f) + min;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return max*(7.5625f*t*t + 0.984375f) + min;
		}
	}

	inline float InBounce(float t, float totaltime, float max, float min)
	{
		return max - OutBounce(totaltime - t, totaltime, max - min, 0) + min;
	}

	inline float InOutBounce(float t, float totaltime, float max, float min)
	{
		if (t < totaltime / 2)
			return InBounce(t * 2, totaltime, max - min, max)*0.5f + min;
		else
			return OutBounce(t * 2 - totaltime, totaltime, max - min, 0)*0.5f + min + (max - min)*0.5f;
	}

	inline float Linear(float t, float totaltime, float max, float min)
	{
		return (max - min)*t / totaltime + min;
	}

}