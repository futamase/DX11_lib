#pragma once

#include "../base/Geometry.h"

namespace tama
{
	namespace System
	{
		/**@ゲームループ*/
		bool Update();

		/**@brief アプリケーションを終了する*/
		void Exit();
	}
	namespace Window
	{
		float GetWidth();

		float GetHeight();

		RectF GetRect();
	}
}