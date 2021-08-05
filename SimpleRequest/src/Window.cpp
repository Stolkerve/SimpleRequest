#include "Window.h"
#include "logging/SimpleLogger.h"
#include <glad/glad.h>


namespace simple {
	Window::Window(uint32_t width, uint32_t height, std::string_view title)
		: m_Width(width), m_Height(height)
	{
		// init glfw
		if (!glfwInit())
		{
			Logger::Error("Error inicializando glfw");
		}

		// set hits
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Creating the window
		m_Window = glfwCreateWindow(static_cast<int>(m_Width), static_cast<int>(m_Height), title.data(), nullptr, nullptr);
		if (!m_Window)
		{
			Logger::Error("Error creando la ventana");
			glfwTerminate();
		}

		// Setting the context
		glfwMakeContextCurrent(m_Window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Logger::Error("Error inicializando opengl");
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

		std::string versionString = std::string((const char*)glGetString(GL_VERSION));
		Logger::Info(versionString);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Clear()
	{
		int display_w, display_h;
		glfwGetFramebufferSize(m_Window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::Display()
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::Close(bool value)
	{
		glfwSetWindowShouldClose(m_Window, value);
	}

	bool Window::isClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	bool Window::isVsyncActive()
	{
		return false;
	}

	void Window::SetVsync()
	{

	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	GLFWwindow* Window::GetWindow()
	{
		return m_Window;
	}
};