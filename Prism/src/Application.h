#pragma once
#include "Events/Event.h"
#include "Window/Window.h"

namespace Prism {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void EventCallback(Event&);

		virtual void OnEvent(Event&) = 0;
		virtual void OnTick(float deltaT) = 0;

	private:
		std::unique_ptr<Window> m_Window;

		float m_LastFrameTime = 0.0f;
		bool m_Running = true;
	};

	// Must be defined in the client's entry point file.
	Application* CreateApplication();
}