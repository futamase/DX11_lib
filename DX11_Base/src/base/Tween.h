#pragma once

#include "Easing.h"
#include "Stopwatch.h"
#include <vector>
#include <functional>
#include <boost\any.hpp>

namespace tama
{
	enum class LoopType
	{
		Incremental,
		Restart,
		PingPong,
	};

	class TamaTween;

	class TweenerBase
	{

	public:
		virtual bool _InternalUpdate() = 0;
		virtual ~TweenerBase(){}
	};

	template<typename Type>
	class Tweener : public TweenerBase
	{
		friend class TamaTween;
	private:
		using InterpolationFunc = std::function<Type(Type, Type, float)>;
		using PlayingFunc = std::function<void()>;
		using CompleteFunc = std::function<void()>;

		Stopwatch m_watch;
		Type* m_value;
		Type m_start;
		Type m_end;
		float m_duration = 2.0;
		InterpolationFunc m_interpolationFunc; // <ret(start, end, time)>
		std::function<float(float,float,float,float)> m_easingFunc = Easing::Linear;
		PlayingFunc m_playingCallback = []() {};
		CompleteFunc m_completeCallback = []() {};
		bool m_isLoop = false;
		unsigned int m_loops = 1;
		LoopType m_loopType = LoopType::Restart;

		bool m_ishoge = false;

		Tweener(Type* value, Type start, Type end, float duration, const InterpolationFunc& func) :
			m_watch(true),
			m_value(value),
			m_start(start),
			m_end(end),
			m_duration(duration),
			m_interpolationFunc(func)
		{
			if (!value)
			{
				m_value = new Type();
				m_ishoge = true;
			}
		}

		void Release()
		{
			if (m_ishoge)
				delete m_value;
		}

		void _ApplyLoop()
		{
			switch (m_loopType)
			{
				case LoopType::Restart:
				{
					break;
				}
				case LoopType::Incremental:
				{
					auto tmp = m_end - m_start;
					m_start += tmp;
					m_end += tmp;
					break;
				}
				case LoopType::PingPong:
				{
					auto tmp = m_start;
					m_start = m_end;
					m_end = tmp;
					break;
				}
				default:
					break;
			}
		}

		bool _InternalUpdate() override
		{
			auto time = m_watch.ms() / 1000.0;

			auto t = m_easingFunc(m_watch.ms() / 1000.f, m_duration, 1.f, 0.f);
			*m_value = m_interpolationFunc(m_start, m_end, t);
			m_playingCallback();

			if (time >= m_duration)
			{
				m_loops--;
				if (m_loops == 0)
				{
					m_completeCallback();
					return false;
				}
				else if(m_loops > 0 || m_loops == -1)
				{
					this->_ApplyLoop();
					m_watch.Restart();
				}
			}

			return true;
		}

	public:
		Tweener<Type>& SetLoop(unsigned int count, LoopType loopType = LoopType::Restart)
		{
			m_loops = count;
			m_loopType = loopType;
			return *this;
		}

		Tweener<Type>& SetEase(Easing::EasingFunc func)
		{
			m_easingFunc = func;
			return *this;
		}

		Tweener<Type>& OnPlay(const PlayingFunc& callback)
		{
			m_playingCallback = callback;
			return *this;
		}

		Tweener<Type>& OnComplete(const CompleteFunc& callback)
		{
			m_completeCallback = callback;
			return *this;
		}
	};

	class TamaTween final
	{
	private:
		Stopwatch timer;
		std::vector<Tweener<float>> m_floatVec;
		std::vector<boost::any> m_vecVec;
		std::vector<TweenerBase*> m_pVec;

	public:
		Tweener<float>& To(float* start, float end, float duration)
		{
			//x(t) = s(1-t) + et
			auto tweener = Tweener<float>(start, *start, end, duration,
				[](float start, float end, float t) -> float
			{
				return start * (1 - t) + end * t;
			});

			m_floatVec.emplace_back(std::move(tweener));
			return m_floatVec.back();
		}

		Tweener<float>& To(float start, float end, float duration)
		{
			auto tweener = Tweener<float>(nullptr, start, end, duration,
				[](float start, float end, float t) -> float
			{
				return start * (1 - t) + end * t;
			});

			m_floatVec.emplace_back(std::move(tweener));
			return m_floatVec.back();
		}

		// ÉNÉâÉXì‡Ç…staticÇÃLerpä÷êîÇ™íËã`Ç≥ÇÍÇƒÇ¢ÇÈÇ±Ç∆Ç™èåè
		template<typename Type>
		Tweener<Type>& To(Type* start, Type end, float duration)
		{
			using namespace	std::placeholders;
			auto func = std::bind((Type(*)(const Type&, const Type&, float))&Type::Lerp, _1, _2, _3);
			auto tweener = Tweener<Type>(start, end, duration, func);

			m_vecVec.emplace_back(tweener);

			auto& t = boost::any_cast<Tweener<Type>&>(m_vecVec.back());
			m_pVec.emplace_back(&t);

			return t;
		}

		void Update()
		{
			auto it = m_floatVec.begin();
			while (it != m_floatVec.end()) 
			{
				if (!it->_InternalUpdate()) 
				{
					it->Release();
					it = m_floatVec.erase(it);
				}
				else ++it;
			}

			auto pit = m_pVec.begin();
			while (pit != m_pVec.end())
			{
				auto p = *pit;
				if (!p->_InternalUpdate())
				{
					pit = m_pVec.erase(pit);
				}
				else ++pit;
			}
		}

	public:
		static TamaTween& I()
		{
			static TamaTween instance;
			return instance;
		}

	private:
		TamaTween() = default;
		TamaTween(const TamaTween&) = delete;
		TamaTween& operator= (const TamaTween&) = delete;
	};
}