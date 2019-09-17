#pragma once
#include "Events/Event.h"

namespace Prism {

	class Window
	{
	public:
		struct Properties
		{
			std::string Title;
			unsigned int Width, Height;
			bool Resizable;

			Properties(
				const std::string& title = "Prism",
				unsigned int width = 1280,
				unsigned int height = 720,
				bool resizable = false
			) : Title(title),
				Width(width),
				Height(height),
				Resizable(resizable) {}
		};

		using EventCallbackFn = std::function<void(Event&)>;

	private:
		// contains window data to pass into GLFW callbacks
		struct WindowData {
			Properties& Properties;
			EventCallbackFn EventCallback;

			WindowData(Window::Properties& props)
				: Properties(props) {}
		};

		// all callback functions Window events will trigger
		std::vector<EventCallbackFn> m_Callbacks;

		// set callbacks from GLFW to Window events
		void SetGLFWCallbacks();

	public:
		// Returns a Window with the given properties
		static std::unique_ptr<Window> Create(Properties p = Properties());

		// Don't use directly - use Create instead
		Window(Properties& properties);

		// Destructor (also destroys GLFW window)
		~Window();

		// Updates the Window content
		void OnUpdate();

		// Time returned by the windowing system (GLFW)
		double GetTime() const;

		// Set callback function for window events (resize, close)
		void AddEventCallback(const EventCallbackFn& callback);

		// Get the pointer to the GLFWwindow
		inline void* GetWindowHandle() const { return m_WindowHandle; }

	private:
		Properties m_Properties;
		WindowData m_Data;
		void* m_WindowHandle; // GLFWwindow*
	};
}