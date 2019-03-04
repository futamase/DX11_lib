#pragma once
#include <iostream>

namespace tama
{
	class DebugWindow
	{
		bool m_isAllocated = false;

		FILE* m_pin;
		FILE* m_pout;
	public:
		DebugWindow();
		~DebugWindow();
	};
}