#include "apple.h"

namespace console_snake {
	Apple::Apple() : Apple({ 0, 0 }) {}

	Apple::Apple(Vector2 position) : _position(position) {}

	Vector2 Apple::GetPosition() const {
		return _position;
	}

	void Apple::SetPosition(Vector2 position) {
		_position = position;
	}
}
