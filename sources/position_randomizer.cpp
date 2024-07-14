#include "position_randomizer.h"

#include <chrono>

namespace console_snake {
	PositionRandomizer::PositionRandomizer() : PositionRandomizer(0, 0, 0, 0) {}

	PositionRandomizer::PositionRandomizer(int min_x, int max_x, int min_y, int max_y) {
		long long seed = std::chrono::system_clock::now().time_since_epoch().count();
		_random_engine = std::default_random_engine(static_cast<unsigned int>(seed));
		_x_distribution = std::uniform_int_distribution<int>(min_x, max_x);
		_y_distribution = std::uniform_int_distribution<int>(min_y, max_y);
	}

	Vector2 PositionRandomizer::GetRandomPosition() {
		return {
			_x_distribution(_random_engine),
			_y_distribution(_random_engine)
		};
	}
}
