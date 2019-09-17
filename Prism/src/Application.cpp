#include "pch.h"
#include "Application.h"

Prism::Application::Application()
{
	m_Window = Window::Create({ "Prism-Test", 1280, 720, true });
	m_Window->AddEventCallback(std::bind(&Application::EventCallback, this, std::placeholders::_1));

	m_Running = true;
}

Prism::Application::~Application()
{
	PR_CORE_TRACE("Application destructed.");
}

void Prism::Application::Run()
{
	while (m_Running)
	{
		float currentTime = (float)m_Window->GetTime();
		float deltaT = m_LastFrameTime - currentTime;
		m_LastFrameTime = currentTime;

		// implemented by client application
		OnTick(deltaT);

		m_Window->OnUpdate();
	}
}

void Prism::Application::EventCallback(Event& event)
{
	event.Handle<WindowCloseEvent>([&](WindowCloseEvent& e)
		{
			PR_CORE_TRACE("WindowCloseEvent handled by Application");
			m_Running = false;
			return true;
		}
	);

	event.Handle<WindowResizeEvent>([&](WindowResizeEvent& e)
		{
			PR_CORE_TRACE("WindowResizeEvent ({0}, {1})", e.GetWidth(), e.GetHeight());
			return true;
		}
	);

	// handle remaining events in client application
	if (!event.Handled) OnEvent(event);
}
