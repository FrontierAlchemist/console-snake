#include "application.h"
#include "curses.h"

#include <chrono>
#include <random>
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
		snake = Snake({ 10, 10 }, { 1, 0 });

		// постоянные координаты яблока 
		Vector2 apple_position { 15,15 };
		// случайные координаты яблока
		Vector2 random_apple_position;
		// частота смены кадров
		int frame_rate = 100;
		// количество собранных яблок
		int eaten_apples = 0;

		// **** начало работы генератора случайных чисел ****
		// момент системного времени
		long long seed = std::chrono::system_clock::now().time_since_epoch().count();
		// запуск генератора случайных чисел
		std::default_random_engine rnd(static_cast<unsigned int>(seed));
		// установка диапазона случайных координат яблока
		std::uniform_int_distribution<int> apple_x(10, 97);
		std::uniform_int_distribution<int> apple_y(5, 22);
		// **** конец работы генератора случайных чисел ****

		// **** игровой цикл ****
		while (!was_game_exited) {
			// присваиваем случайные значения переменным случайных координат яблока
			random_apple_position.x = apple_x(rnd);
			random_apple_position.y = apple_y(rnd);
			// рисуем игровое поле
			UpdateMap(apple_position);
			UpdateInterface();
			// перемещаем змейку по игровому полю
			UpdateSnake(apple_position, random_apple_position);
			// частота сменны игрового кадра
			timeout(frame_rate);

			// **** управление змейкой и игрой ****
			// проверка нажатой клавиши
			switch (getch()) {
			case KEY_UP:
				if (snake.direction.y == 0) {
					snake.direction.y = -1;
					snake.direction.x = 0;
					frame_rate = 170;
				}
				break;
			case KEY_DOWN:
				if (snake.direction.y == 0) {
					snake.direction.y = 1;
					snake.direction.x = 0;
					frame_rate = 170;
				}
				break;
			case KEY_LEFT:
				if (snake.direction.x == 0) {
					snake.direction.x = -1;
					snake.direction.y = 0;
					frame_rate = 100;
				}
				break;
			case KEY_RIGHT:
				if (snake.direction.x == 0) {
					snake.direction.x = 1;
					snake.direction.y = 0;
					frame_rate = 100;
				}
				break;
			case 'q':
				was_game_exited = true;
				break;
			default:
				break;
			}
		}
		endwin();
	}

	void Application::UpdateMap(const Vector2& apple) {
		clear();

		UpdateField();
		UpdateApple(apple);
	}

	void Application::UpdateSnake(Vector2& apple_position, Vector2 random_apple_position) {
		const int snake_color_pair = 3;
		// устанавливаем цвет змейки
		attrset(A_BOLD | COLOR_PAIR(snake_color_pair));

		// Если  змейка с хвостом, рисуем хвост
		if (!snake.body_parts_positions.empty()) {
			for (auto const& mov : snake.body_parts_positions) {
				move(mov.y, mov.x);
				printw("#");
			}
		}
		// изменяем координаты головы змейки
		snake.head_position += snake.direction;
		// перемещаем курсов в координаты головы змейки
		move(snake.head_position.y, snake.head_position.x);
		// проверяем символ в установленных координатах курсора
		auto s = static_cast<char>(winch(stdscr));
		// *** Если змейка столкнулась с хвостом или границами игрового поля
		//  ### Выводим игровое меню ###
		if (s == '*' || s == '#') {
			ShowGameOverDialog(apple_position);
		}
		// ### Конец игрового меню ###

		//  Если змейка съедает яблоко
		if (s == '@') {
			// увеличиваем количество съеденных яблок
			eaten_apples++;
			// добавляем хвост змейки
			snake.body_parts_positions.push_back(snake.head_position);
			// рисуем голову змейки
			printw("$");

			do {
				// задаём новые координаты яблока
				apple_position = random_apple_position;
				move(apple_position.y, apple_position.x);
				auto s = static_cast<char>(winch(stdscr));
				// повторяем цикл пока координаты яблока совпадают с хвостом змейки
			} while (s == '#');
		}
		// Если змейка переместилась в свободное поле
		else {
			// рисуем голову змейки
			printw("$");
			// обновляем координаты хвоста змеи
			if (!snake.body_parts_positions.empty()) {
				snake.body_parts_positions.erase(snake.body_parts_positions.begin());
				snake.body_parts_positions.push_back(snake.head_position);
			}
		}
	}
	
	void Application::ShowGameOverDialog(Vector2& apple_position) {
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
				was_game_exited = true;
				return;
			}
			if (getch() == 'n') {
				snake.head_position = { 10, 10 };
				snake.direction = { 10, 10 };
				snake.body_parts_positions.clear();
				apple_position = { 15,15 };
				eaten_apples = 0;
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
		std::string eaten_apples_text_value = std::to_string(eaten_apples);
		printw(eaten_apples_text_value.c_str());
	}

	void Application::UpdateField() {
		for (int y = 4; y < 28; y++) {
			for (int x = 5; x < 112; x++) {
				if (y == 4 || y == 27 || x == 5 || x == 111) {
					move(y, x);
					printw("*");
				}
			}
		}
	}

	void Application::UpdateApple(const Vector2& apple) {
		const int apple_color_pair = 2;
		attrset(A_BOLD | COLOR_PAIR(apple_color_pair));

		move(apple.y, apple.x);
		printw("@");
	}
}
