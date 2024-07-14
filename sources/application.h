#pragma once

#include "snake.h"
#include "vector2.h"

namespace console_snake {
	class Application {
	public:
		static Application* GetInstance();

		void Run();
	private:
		static Application* instance;

		Snake snake;
		bool was_game_exited = false;
		int eaten_apples = 0;

		Application();

		void InitializeScreen();

		void InitializeColorPairs();

		void RunApplicationLoop();

		void UpdateMap(const Vector2& apple_position);

		void UpdateSnake(Vector2& apple_position, Vector2 random_apple_position);

		void ShowGameOverDialog(Vector2& apple_position);

		void UpdateInterface();

		void UpdateField();

		void UpdateApple(const Vector2& apple_position);
	};
}
