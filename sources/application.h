#pragma once

#include "snake.h"
#include "position_randomizer.h"
#include "vector2.h"

namespace console_snake {
	class Application {
	public:
		static Application* GetInstance();

		void Run();
	private:
		static Application* instance;

		Snake _snake;
		PositionRandomizer _apple_position_randomizer;

		bool _was_game_exited = false;
		int _eaten_apples_count = 0;

		Application();

		void InitializeScreen();

		void InitializeColorPairs();

		void RunApplicationLoop();

		void UpdateMap(const Vector2& apple_position);

		void UpdateSnake(Vector2& apple_position);

		void ShowGameOverDialog(Vector2& apple_position);

		void UpdateInterface();

		void UpdateField();

		void UpdateApple(const Vector2& apple_position);
	};
}
