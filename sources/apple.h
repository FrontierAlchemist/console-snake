#include "vector2.h"

#pragma once
namespace console_snake {
	class Apple {
	public:
		Apple();

		Apple(Vector2 position);

		Vector2 GetPosition() const;

		void SetPosition(Vector2 position);
	private:
		Vector2 _position;
	};
}
