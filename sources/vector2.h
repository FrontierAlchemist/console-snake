#pragma once
namespace console_snake {
	struct Vector2 {
		int x;
		int y;

		Vector2() : x(0), y(0) {}

		Vector2(int x, int y) : x(x), y(y) {}

		Vector2& operator+=(const Vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
	};
}
