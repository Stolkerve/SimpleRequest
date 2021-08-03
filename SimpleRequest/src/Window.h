#pragma once
#include <string_view>
#include <GLFW/glfw3.h>

namespace simple {
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();
		void Clear();
		void Display();
		void Close(bool value);
		bool isClose();
		bool isVsyncActive();
		void SetVsync();
		void PollEvents();
		GLFWwindow* GetWindow();

	private:
		GLFWwindow* m_Window;
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Title;
		bool m_Vsync;
	};
}