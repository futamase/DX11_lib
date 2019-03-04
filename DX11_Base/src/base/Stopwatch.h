#pragma once

#include <vector>
#include <chrono>
#include <cstdint>

namespace tama 
{
	class Stopwatch
	{
		std::chrono::time_point<std::chrono::system_clock> start;
		std::chrono::milliseconds elapsed;
		bool isPaused = false;
		bool isStarted = false;

		static std::vector<Stopwatch*> s_pInstances;
	public:

		Stopwatch(bool startImmediate = false);
		~Stopwatch();
		Stopwatch& operator= (const Stopwatch&) = default;

		void Start()
		{
			start = std::chrono::system_clock::now();
			elapsed = std::chrono::milliseconds(0);
			isStarted = true;
		}

		void Pause()
		{
			using namespace std::chrono;

			if (isPaused)
				return;
			isPaused = true;

			elapsed += duration_cast<milliseconds>(system_clock::now() - start);
		}

		void Stop()
		{
			isStarted = false;
			isPaused = false;
		}

		void Resume()
		{
			if (!isPaused)
				return;
			isPaused = false;

			start = std::chrono::system_clock::now();
		}

		void Restart()
		{
			this->Start();
		
			isPaused = false;
		}

		std::chrono::milliseconds Elapsed() const 
		{
			using namespace std::chrono;

			if (!isStarted)
				return milliseconds(0);

			if (!isPaused)
				return elapsed + duration_cast<milliseconds>(system_clock::now() - start);
			else
				return elapsed;
		}

		int32_t ms() const 
		{
			return static_cast<int32_t>(this->Elapsed().count());
		}

		int32_t hours() const
		{
			return ms() / (1000 * 60 * 60);
		}

		int32_t minutes() const { return ms() / (1000 * 60); }

		int32_t seconds() const { return ms() / 1000; }

		bool IsActive() const { return !isPaused || isStarted; }

		static void PauseAll();
		static void ResumeAll();
	};
}