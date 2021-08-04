#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <GLFW/glfw3.h>

namespace simple {
	class ImGuiApp
	{
	public:
		ImGuiApp(GLFWwindow* window);
		~ImGuiApp();

		void Begin();
		void End();

	private:
		GLFWwindow* m_Window;
		ImGuiViewport* m_Viewport;
	};
}