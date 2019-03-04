#pragma once

#include "Stopwatch.h"
#include <functional>

namespace tama
{
	// ���Ԃ�P�̂ł��g����
	class Timer
	{
	private:
		Stopwatch m_watch;
		std::function<void()> m_func;
		const float m_t = 0;

	public:
		Timer(float t, const std::function<void()>& func)
			: m_t(t)
			, m_func(func)
			, m_watch(true)
		{}


		bool Update() const
		{
			if (m_watch.ms() / 1000.f >= m_t)
			{
				return true;
			}

			return false;
		}

		// t�a���func�����s����֐�
		// Stopwatch::PauseAll�łƂ܂�
		static void Time(float t, const std::function<void()>& func);

		static void UpdateAll();

		static std::vector<Timer> s_timerList;
	};

}
