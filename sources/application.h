#pragma once

#include "apple.h"
#include "snake.h"
#include "position_randomizer.h"

namespace console_snake {
	class Application {
	public:
		static Application* GetInstance();

		void Run();
	private:
		static Application* instance;

		Snake _snake;
		Apple _apple;
		PositionRandomizer _apple_position_randomizer;

		bool _was_game_exited = false;
		int _eaten_apples_count = 0;

		Application();

		void InitializeScreen();

		void InitializeColorPairs();

		void RunApplicationLoop();

		void UpdateMap();

		void UpdateSnake();

		void ShowGameOverDialog();

		void UpdateInterface();

		void UpdateField();

		void UpdateApple();
	};
}
