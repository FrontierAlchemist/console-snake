#pragma once

#include "vector2.h"

#include <vector>

namespace console_snake {
	class Snake {
	public:
		Vector2 head_position;
		Vector2 direction;
		
		std::vector<Vector2> body_parts_positions;

		Snake();

		Snake(Vector2 head_position, Vector2 direction);

		void SetDirection(Vector2 direction);
	};
}
