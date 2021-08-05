#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <array>

#include <nlohmann/json.hpp>

#include "Window.h"
#include "ImGuiApp.h"

namespace simple {
	class App
	{
	public:
		App();
		void Start();

	private:
		void RenderImGui();
		void RenderRespondePanel();
		void RenderRequestPanel();
		void CreateRequest(const std::string& method, const std::string& body, const std::vector<std::string>& headers);

	private:
		int m_TimeOut = 10000;
		int m_StatusCode = 0;
		int m_SelectedMethod = 0;
		std::unique_ptr<Window> m_Window;
		std::vector<std::string> m_RequestHeaders;
		std::string m_RequestBody;
		std::string m_ResponseHeader;
		std::string m_ResponseBody;
		char* m_Url;
		bool m_BlockInputAndSend = false;
		ImGuiApp m_ImGuiApp;
		std::array<const char*, 5> m_Methods = { "GET", "POST", "PUT", "PATCH", "DELETE" };
		nlohmann::json m_Json;
	};
};
