#pragma once

namespace Prism {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent();
	};

	// 
	// Must be defined in the Client's entry point file.
	Application* CreateApplication();
}