#pragma once

#include "../base/Geometry.h"

namespace tama
{
	namespace System
	{
		/**@�Q�[�����[�v*/
		bool Update();

		/**@brief �A�v���P�[�V�������I������*/
		void Exit();
	}
	namespace Window
	{
		float GetWidth();

		float GetHeight();

		RectF GetRect();
	}
}