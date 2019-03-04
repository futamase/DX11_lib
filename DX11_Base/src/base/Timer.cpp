#include "Timer.h"
#include <vector>
#include <algorithm>

namespace tama
{
	std::vector<Timer> Timer::s_timerList;

	void Timer::Time(float t, const std::function<void()>& func)
	{
		s_timerList.emplace_back(t, func);
	}

	void Timer::UpdateAll()
	{
//		for (auto&& it = s_timerList.begin(); it != s_timerList.end(); ++it)
//		{
//			if (it->Update())
//				it = s_timerList.erase(it);
//		}
//		s_timerList.erase(std::remove_if(s_timerList.begin(), s_timerList.end(),
//			[](const Timer& t) { return t.Update(); }), 
//			s_timerList.end());
	}
}
