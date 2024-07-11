#pragma once
namespace console_snake {
	class Application {
	public:
		static Application* GetInstance();

		void Run();
	private:
		static Application* instance;

		Application();

		void InitializeScreen();

		void RunApplicationLoop();
	};
}
