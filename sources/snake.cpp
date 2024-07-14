#include "snake.h"

namespace console_snake {
	Snake::Snake() {}

	Snake::Snake(Vector2 head_position, Vector2 direction) {
		this->head_position = head_position;
		this->direction = direction;

		body_parts_positions = std::vector<Vector2>();
	}

	void Snake::SetDirection(Vector2 direction) {
		this->direction = direction;
	}
}
