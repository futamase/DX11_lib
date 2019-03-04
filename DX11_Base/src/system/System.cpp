#include "System.h"
#include "detail/Time.h"
#include "../win/Window.h"
#include "../dx/dxgi/Swapchain.h"
#include "../system/Input.h"

namespace tama
{
	namespace System
	{
		bool Update()
		{
			DXGI::Swapchain::GetInstance()->End();

			Input::Update();

			bool isContinue = Time::_TimeImpl::GetInstance()->Update();


			DXGI::Swapchain::GetInstance()->Begin();

			return isContinue;
		}

		void Exit()
		{
			::PostQuitMessage(0);
		}
	}

	namespace Window
	{
		float GetWidth()
		{
			return Win::Window::I()->Width();
		}

		float GetHeight()
		{
			return Win::Window::I()->Height();
		}

		RectF GetRect()
		{
			return RectF{ static_cast<float>(GetWidth()), static_cast<float>(GetHeight()) };
		}
	}
}