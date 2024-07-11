#include "application.h"
#include "curses.h"

#include <iostream>

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
		std::cout << "Start application" << std::endl;
		InitializeScreen();
		RunApplicationLoop();
	}

	Application::Application() {
	}

	void Application::InitializeScreen() {
	}

	void Application::RunApplicationLoop() {
		while (true) {
		}
	}
}
