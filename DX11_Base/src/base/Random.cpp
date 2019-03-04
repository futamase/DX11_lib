#include "Random.h"

#include <random>
#include <cassert>

namespace tama
{
	namespace Random
	{
		std::random_device& GetDevice()
		{
			static std::random_device randomMaker;
			return randomMaker;
		}

		int Range(int begin, int end) {
			if (begin < end) {
				static std::mt19937 mersenneTwister(GetDevice()());

				std::uniform_int_distribution<int> distribution(begin, end);
				return distribution(mersenneTwister);
			}
			else
				return 0;
		}

		float Range(float begin, float end) {
			if (begin < end) {
				static std::mt19937 mersenneTwister(GetDevice()());

				std::uniform_real_distribution<float> distribution(begin, end);
				return distribution(mersenneTwister);
			}
			else
				return 0;
		}

		bool Bool(float p)
		{
			std::bernoulli_distribution dist(p);
			return dist(GetDevice());
		}
	}
}