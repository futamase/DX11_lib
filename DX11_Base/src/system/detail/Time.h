#pragma once

#include <memory>
 
namespace tama
{
	namespace System
	{
		namespace Time
		{
			// 実装の都合上公開せざるを得ないのはよくない
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

			/**@brief フレーム間の経過時間を返す*/
			double GetDeltaTime();

			/**@brief 起動からのフレーム数を返す*/
			unsigned GetFrameCount();

			/**@brief FPSを返す*/
			float GetFPS();

			/**@理想のFPSを設定する(60以下)*/
			void SetFPS(unsigned fps);

			/**@brief システム起動からの経過時間を返す*/
			float GetElapsedTime();
		}
	}
}