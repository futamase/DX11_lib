#pragma once

#include <memory>
 
namespace tama
{
	namespace System
	{
		namespace Time
		{
			// �����̓s������J������𓾂Ȃ��̂͂悭�Ȃ�
			class _TimeImpl final
			{
				class Impl;
				std::unique_ptr<Impl> m_pImpl;

			public:
				float FPS = 0.f;
				unsigned long delta = 0;
				unsigned long frameCount = 0;
				float TotalTime = 0.f;

			private:
				_TimeImpl();
				_TimeImpl& operator= (const _TimeImpl&) = delete;
				_TimeImpl(const _TimeImpl&) = delete;

			public:
				bool Update();
				void SetFPS(unsigned fps);
				float GetElapsedTime();

			public:
				static _TimeImpl* GetInstance();
			};

			/**@brief �t���[���Ԃ̌o�ߎ��Ԃ�Ԃ�*/
			double GetDeltaTime();

			/**@brief �N������̃t���[������Ԃ�*/
			unsigned GetFrameCount();

			/**@brief FPS��Ԃ�*/
			float GetFPS();

			/**@���z��FPS��ݒ肷��(60�ȉ�)*/
			void SetFPS(unsigned fps);

			/**@brief �V�X�e���N������̌o�ߎ��Ԃ�Ԃ�*/
			float GetElapsedTime();
		}
	}
}