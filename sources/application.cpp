﻿#include "application.h"
#include "curses.h"

#include <chrono>
#include <string>

namespace console_snake
{
	Application* Application::instance;

	Application* Application::GetInstance() {
		if (instance == nullptr) {
			instance = new Application();
		}
		return instance;
	}

	void Application::Run() {
		InitializeScreen();
		RunApplicationLoop();
	}

	Application::Application() {
	}

	void Application::InitializeScreen() {
		initscr();
		const int ivisible_cursor_value = 0;
		curs_set(ivisible_cursor_value);
		start_color();
		InitializeColorPairs();
		const int background_color_pair = 1;
		bkgd(COLOR_PAIR(background_color_pair));
		bool can_use_special_keypads = true;
		keypad(stdscr, can_use_special_keypads);
		noecho();
	}

	void Application::InitializeColorPairs() {
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		init_pair(2, COLOR_MAGENTA, COLOR_BLUE);
		init_pair(3, COLOR_GREEN, COLOR_BLUE);
		init_pair(4, COLOR_RED, COLOR_BLUE);
		init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	}

	void Application::RunApplicationLoop() {
		_snake = Snake({ 10, 10 }, { 1, 0 });
		_apple_position_randomizer = PositionRandomizer(10, 97, 5, 22);
		_apple = Apple({ 15, 15 });

		int frame_rate = 180;

		while (!_was_game_exited) {
			UpdateMap();
			UpdateInterface();
			UpdateSnake();

			timeout(frame_rate);

			switch (getch()) {
			case KEY_UP:
				_snake.SetDirection({ 0, -1 });
				break;
			case KEY_DOWN:
				_snake.SetDirection({ 0, 1 });
				break;
			case KEY_LEFT:
				_snake.SetDirection({ -1, 0 });
				break;
			case KEY_RIGHT:
				_snake.SetDirection({ 1, 0 });
				break;
			case 'q':
				_was_game_exited = true;
				break;
			default:
				break;
			}
		}
		endwin();
	}

	void Application::UpdateMap() {
		clear();

		UpdateField();
		DrawApple();
	}

	void Application::UpdateSnake() {
		const int snake_color_pair = 3;
		attrset(A_BOLD | COLOR_PAIR(snake_color_pair));

		// Если  змейка с хвостом, рисуем хвост
		if (!_snake.body_parts_positions.empty()) {
			for (Vector2 const& snake_part_position : _snake.body_parts_positions) {
				move(snake_part_position.y, snake_part_position.x);
				printw("#");
			}
		}
		// изменяем координаты головы змейки
		_snake.head_position += _snake.direction;
		// перемещаем курсов в координаты головы змейки
		move(_snake.head_position.y, _snake.head_position.x);
		// проверяем символ в установленных координатах курсора
		auto checked_char = static_cast<char>(winch(stdscr));
		// *** Если змейка столкнулась с хвостом или границами игрового поля
		if (checked_char == '*' || checked_char == '#') {
			ShowGameOverDialog();
		}
		else if (checked_char == '@') {
			// увеличиваем количество съеденных яблок
			_eaten_apples_count++;
			// добавляем хвост змейки
			_snake.body_parts_positions.push_back(_snake.head_position);
			// рисуем голову змейки
			printw("$");

			CreateNewApple();
			DrawApple();
		}
		else {
			// рисуем голову змейки
			printw("$");
			// обновляем координаты хвоста змеи
			if (!_snake.body_parts_positions.empty()) {
				_snake.body_parts_positions.erase(_snake.body_parts_positions.begin());
				_snake.body_parts_positions.push_back(_snake.head_position);
			}
		}
	}

	void Application::ShowGameOverDialog() {
		const int game_over_dialog_color_pair = 4;
		attrset(A_BOLD | COLOR_PAIR(game_over_dialog_color_pair));

		const Vector2 game_over_label_position(55, 13);
		move(game_over_label_position.y, game_over_label_position.x);
		std::string game_over_label = "Game over!";
		printw(game_over_label.c_str());

		const Vector2 game_over_instruction_position(42, 14);
		move(game_over_instruction_position.y, game_over_instruction_position.x);
		std::string game_over_instruction = "Quit - < q > Replay - < n >";
		printw(game_over_instruction.c_str());
		while (true) {
			if (getch() == 'q') {
				_was_game_exited = true;
				return;
			}
			if (getch() == 'n') {
				_snake.head_position = { 10, 10 };
				_snake.direction = { 1, 0 };
				_snake.body_parts_positions.clear();
				_apple.SetPosition({ 15, 15 });
				_eaten_apples_count = 0;
				return;
			}
		}
	}

	void Application::UpdateInterface() {
		const int interface_text_color_pair = 1;
		attrset(A_DIM | COLOR_PAIR(interface_text_color_pair));

		const Vector2 game_label_position(55, 2);
		std::string game_label = "Snake!";
		move(game_label_position.y, game_label_position.x);
		printw(game_label.c_str());

		const Vector2 collected_apples_label_position(88, 2);
		std::string collected_apples_label = "Collected apples: ";
		move(collected_apples_label_position.y, collected_apples_label_position.x);
		printw(collected_apples_label.c_str());
		std::string eaten_apples_text_value = std::to_string(_eaten_apples_count);
		printw(eaten_apples_text_value.c_str());
	}

	void Application::UpdateField() {
		const int field_color_pair = 1;
		attrset(A_DIM | COLOR_PAIR(field_color_pair));
		for (int y = 4; y < 28; y++) {
			for (int x = 5; x < 112; x++) {
				if (y == 4 || y == 27 || x == 5 || x == 111) {
					move(y, x);
					printw("*");
				}
			}
		}
	}

	void Application::CreateNewApple() {
		Vector2 new_apple_position;
		char checked_char;
		do {
			new_apple_position = _apple_position_randomizer.GetRandomPosition();
			move(new_apple_position.y, new_apple_position.x);
			checked_char = static_cast<char>(winch(stdscr));
		} while (checked_char == '#' || checked_char == '*' || checked_char == '$');
		_apple.SetPosition(new_apple_position);
	}

	void Application::DrawApple() {
		const int apple_color_pair = 2;
		attrset(A_BOLD | COLOR_PAIR(apple_color_pair));

		Vector2 apple_position = _apple.GetPosition();
		move(apple_position.y, apple_position.x);
		printw("@");
	}
}
