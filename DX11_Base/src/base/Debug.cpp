#include "Debug.h"

#include <Windows.h>
#include <WinBase.h>

namespace tama
{
	void Debug::_outputDebugString(const std::wstring& str)
	{
		::OutputDebugString(str.c_str());
	}
}