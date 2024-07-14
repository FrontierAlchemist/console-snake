#pragma once

#include "vector2.h"

#include <random>

namespace console_snake {
	class PositionRandomizer {
	public:
		PositionRandomizer();

		PositionRandomizer(int min_x, int max_x, int min_y, int max_y);

		Vector2 GetRandomPosition();
	private:
		std::default_random_engine _random_engine;
		std::uniform_int_distribution<int> _x_distribution;
		std::uniform_int_distribution<int> _y_distribution;
	};
}
