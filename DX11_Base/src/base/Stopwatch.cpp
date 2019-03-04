#include "Stopwatch.h"
#include <algorithm>

namespace tama
{
	std::vector<Stopwatch*> Stopwatch::s_pInstances;

	Stopwatch::Stopwatch(bool startImmediate)
	{
		s_pInstances.push_back(this);

		if (startImmediate)
			this->Start();
	}

	Stopwatch::~Stopwatch()
	{
		s_pInstances.erase(std::remove_if(s_pInstances.begin(), s_pInstances.end(),
			[this](Stopwatch* p) { return p == this; }),
			s_pInstances.end());
	}

	void Stopwatch::PauseAll()
	{
		for (auto&& elem : s_pInstances)
		{
			elem->Pause();
		}
	}

	void Stopwatch::ResumeAll()
	{
		for (auto&& elem : s_pInstances)
		{
			elem->Resume();
		}
	}
}