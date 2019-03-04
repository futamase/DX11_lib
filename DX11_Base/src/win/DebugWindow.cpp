#include "DebugWindow.h"
#include <Windows.h>
#include <cstdio>

namespace tama
{
	DebugWindow::DebugWindow()
	{
		m_isAllocated = ::AllocConsole() != FALSE;
		if (m_isAllocated)
		{
			::freopen_s(&m_pout, "CONOUT$", "w", stdout);
			::freopen_s(&m_pin, "CONIN$", "r", stdin);
		}
	}

	DebugWindow::~DebugWindow()
	{
		if (m_isAllocated) {
			::fclose(m_pout);
			::fclose(m_pin);
			::FreeConsole();
		}
	}
}