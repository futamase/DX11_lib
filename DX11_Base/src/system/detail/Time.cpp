#include "Time.h"

#include <Windows.h>
#include <utility>
#include <vector>

#pragma comment(lib, "winmm")

namespace
{
	bool Wait(DWORD wait_time)
	{
		MSG msg;
		DWORD start_time = ::timeGetTime();

		do {
			// ���b�Z�[�W����
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return false;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (wait_time > 0)
				::Sleep(1); // CPU�̐�L���������邽��

		} while (::timeGetTime() < wait_time + start_time); // wait_time �������

		return true;
	}

	//1000 / 60��16.6666�����Aint�ł�16�ɂȂ��Ă��܂��B
	//2�t���[�����Ƃɐ؂�グ���s�����Ƃɂ�蒲��
	unsigned int calcFloat(DWORD origin)
	{
		static float f = 0.f;
		f += 0.666f;

		static bool flip = false;

		if (std::exchange(flip, !flip))	//�����؂�グ�̂Ƃ�
			return origin;
		else  //�����؂�̂Ă̂Ƃ�
			return origin;
	}
}

namespace tama
{
	namespace System
	{
		namespace Time
		{
			class _TimeImpl::Impl
			{
				float m_updateInterval = 0.5f; // 0.5 second
				DWORD m_lastTime = 0;
				//FPS accumulated over the interval
				float m_accum = 0.f;
				int m_frames = 0;
				float m_timeLeft = 0.f;
				DWORD m_appStartTime;
			public:
				DWORD desireFPS = 60;
				float FPS;
				DWORD delta;
				DWORD frameCount;

				Impl()
				{
					::timeBeginPeriod(1);

					m_timeLeft = m_updateInterval;
					m_appStartTime = m_lastTime = ::timeGetTime();
				}

				~Impl()
				{
					::timeEndPeriod(1);
				}

				bool Update();
				DWORD GetElapsedTime() { return m_appStartTime - ::timeGetTime(); }
			};

			bool _TimeImpl::Impl::Update()
			{
				static DWORD start = ::timeGetTime();
				static DWORD end = ::timeGetTime();

				delta = end - start;

				auto isContinued = (1000 / desireFPS > delta) ?
					Wait(calcFloat(1000 / desireFPS - delta)) :
					Wait(0);

				// ���߂�delta���v�����āAWait���܂߂�1�t���[�����̎��Ԃ��v������
				end = ::timeGetTime();
				delta = end - start;

				m_timeLeft -= delta / 1000.f;
				m_accum += 1000.f / (float)delta;

				++m_frames;
				++frameCount;

				// Interval ended - update result
				if (m_timeLeft <= 0)
				{
					FPS = m_accum / m_frames;
					m_timeLeft = m_updateInterval;
					m_accum = 0;
					m_frames = 0;
				}

				start = ::timeGetTime();

				return isContinued;
			}

			_TimeImpl::_TimeImpl() :
				m_pImpl(std::make_unique<_TimeImpl::Impl>())
			{}

			bool _TimeImpl::Update()
			{
				bool isContinued = m_pImpl->Update();

				this->delta = m_pImpl->delta;
				this->frameCount = m_pImpl->frameCount;
				this->FPS = m_pImpl->FPS;

				this->TotalTime += this->delta / 1000.f;

				return isContinued;
			}

			void _TimeImpl::SetFPS(unsigned fps)
			{
				this->m_pImpl->desireFPS = fps;
			}

			float _TimeImpl::GetElapsedTime()
			{
				return static_cast<float>(this->m_pImpl->GetElapsedTime());
			}

			_TimeImpl* _TimeImpl::GetInstance()
			{
				static _TimeImpl instance;
				return &instance;
			}

			void SetFPS(float fps)
			{
				_TimeImpl::GetInstance()->SetFPS(fps);
			}
			double GetDeltaTime()
			{
				return _TimeImpl::GetInstance()->delta / 1000.0;
			}

			unsigned GetFrameCount()
			{
				return _TimeImpl::GetInstance()->frameCount;
			}

			float GetFPS()
			{
				return _TimeImpl::GetInstance()->FPS;
			}

			void SetFPS(unsigned fps)
			{
				_TimeImpl::GetInstance()->SetFPS(fps);
			}

			float GetElapsedTime()
			{
				return _TimeImpl::GetInstance()->TotalTime;
			}
		}
	}
}