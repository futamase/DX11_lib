#pragma once

#include <string>
#include <sstream>
#include "GetTextImpl.h"

namespace tama
{
	/**@brief �f�o�b�O���Ɏg�p���郆�[�e�B���e�B�N���X
	*@detail �����[�X���ɂ͉����s��Ȃ�*/
	class Debug
	{
	private:
		static void _outputDebugString(const std::wstring& str);
	public:
		/**@brief visual studio �̃R���\�[���ɕ������\������*/
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