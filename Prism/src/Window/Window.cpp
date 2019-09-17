#include "pch.h"
#include "Log/Log.h"
#include "GLFW/glfw3.h"
#include "Window.h"

namespace Prism {

	Window::Window(Properties& properties)
		: m_Properties(properties)
		, m_Data(properties)
	{
		m_Data.EventCallback = [&](Event& e) {
			// callback function
			for (const EventCallbackFn& callback : m_Callbacks)
				callback(e);
		};
	}

	// required as a function pointer for GLFW
	static void GLFWErrorCallback(int error, const char* description)
	{
		PR_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	std::unique_ptr<Window> Window::Create(Properties properties)
	{
		// initialize GLFW only once (with local static)
		static bool initialized = [] {
			PR_CORE_INFO("Initialzie GLFW Context");
			int success = glfwInit();
			PR_CORE_ASSERT(success, "glfwInit() failed!");
			glfwSetErrorCallback(GLFWErrorCallback);
			return success;
		}();

		PR_CORE_ASSERT(glfwVulkanSupported(), "Vulkan not supported");

		PR_CORE_TRACE("Initialize Vulkan window");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, properties.Resizable);

		// create Prism::Window
		auto window = std::make_unique<Window>(properties);

		// pointer to GLFW Vulkan window
		window->m_WindowHandle = glfwCreateWindow(properties.Width, properties.Height,
			properties.Title.c_str(), nullptr, nullptr);

		// setup callbacks from GLFW to Prism Event system
		window->SetGLFWCallbacks();

		PR_CORE_INFO("Window created");
		return window;
	}

	Window::~Window()
	{
		if (m_WindowHandle) glfwDestroyWindow((GLFWwindow*)m_WindowHandle);
		else PR_CORE_WARN("Destruction of GLFW_Window could not find a window to delete");
		PR_CORE_INFO("Window destroyed.");
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}

	double Window::GetTime() const
	{
		return glfwGetTime();
	}

	void Window::AddEventCallback(const EventCallbackFn& callback)
	{
		PR_CORE_TRACE("Window callback function added");
		m_Callbacks.push_back(callback);
	}

	void Window::SetGLFWCallbacks()
	{
		GLFWwindow* window = (GLFWwindow*)m_WindowHandle;

		// user pointer to data for using it inside callback
		glfwSetWindowUserPointer(window, &m_Data);

		// callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// update m_Data.Properties (m_Properties)
			data.Properties.Width = width;
			data.Properties.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});
	}
}