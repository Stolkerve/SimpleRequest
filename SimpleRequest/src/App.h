#pragma once

#include <memory>

#include "Window.h"
#include "ImGuiApp.h"

namespace simple {
	class App
	{
	public:
		App();
		void Start();
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiApp m_ImGuiApp;
	};
};