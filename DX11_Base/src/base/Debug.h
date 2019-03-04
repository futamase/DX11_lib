#pragma once

#include <string>
#include <sstream>
#include "GetTextImpl.h"

namespace tama
{
	/**@brief デバッグ時に使用するユーティリティクラス
	*@detail リリース時には何も行わない*/
	class Debug
	{
	private:
		static void _outputDebugString(const std::wstring& str);
	public:
		/**@brief visual studio のコンソールに文字列を表示する*/
		template<typename ...Args>
		static void Log(const Args& ...args)
		{
#ifdef _DEBUG 
			std::wstringstream stream;
			impl::get_text_impl(&stream, args...);
			std::wstring text = stream.str() + L"\n";
			_outputDebugString(text.c_str());
#endif
		}
	};
}