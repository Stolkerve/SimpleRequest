#include "App.h"
#include "logging/SimpleLogger.h"
#include <HTTPRequest/include/HTTPRequest.hpp>
#include <future>
#include <thread>

namespace simple {

	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}

	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

	bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
	}

	ImVec2 CalButtomSize(std::string_view label)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		const ImVec2 label_size = ImGui::CalcTextSize(label.data(), NULL, true);
		return ImGui::CalcItemSize({ 0.f, 0.f }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
	}

	//void HeaderInput(std::string_view label)
	//{
	//	ImGui::PushID(label.data());
	//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	//	const bool open = ImGui::DragInt(label, value, 1, min, max);
	//	ImGui::SameLine();
	//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
	//	if (ImGui::Button("##a"))
	//	ImGui::PopStyleVar();
	//	ImGui::PopID();
	//}

	App::App()
		: m_Window(std::make_unique<Window>(640, 480, "Hola mundo")),
		m_ImGuiApp(m_Window->GetWindow()),
		m_RequestBody("")
	{
		m_Url = new char[2048];
		memset(m_Url, '\0', sizeof(char) * 2048);
		strcpy(m_Url, "localhost:3000");
		/*m_RequestHeaders.push_back("Content-Type: application/json; charset=utf-8");*/
	}

	void App::Start()
	{
		while (!m_Window->isClose())
		{
			m_Window->PollEvents();
			m_ImGuiApp.Begin();

			RenderImGui();

			m_Window->Clear();
			m_ImGuiApp.End();
			m_Window->Display();
		}
	}

	void App::RenderImGui()
	{
		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoTabBar;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

		auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, windowFlags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

			static auto first_time = true;
			if (first_time)
			{
				first_time = false;

				ImGui::DockBuilderRemoveNode(dockspaceID); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspaceID, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

				// Split layout
				//auto leftID = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.3f, nullptr, &dockspaceID);
				auto downID = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Down, 0.3f, nullptr, &dockspaceID);

				// build Layout
				ImGui::DockBuilderDockWindow("Center", dockspaceID);
				ImGui::DockBuilderDockWindow("Down", downID);
				ImGui::DockBuilderFinish(dockspaceID);
			}
		}
		ImGui::End();

		// Panels

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5.0f, 4.0f });

		RenderRequestPanel();
		RenderRespondePanel();

		ImGui::PopStyleVar(2);

		ImGui::ShowDemoWindow();

		ImGui::Render();
	}

	void App::CreateRequest(const std::string& method, const std::string& body, const std::vector<std::string>& headers)
	{
		try
		{
			http::Request request{ std::string(m_Url) };
			const auto response = request.send(method, body, headers, std::chrono::milliseconds{ m_TimeOut });
			m_StatusCode = response.status;
			for (auto& h : response.headers)
				m_ResponseHeader += std::move((h + '\n'));
			m_ResponseBody = std::move(std::string{ response.body.begin(), response.body.end() });
		}
		catch (const std::exception& e)
		{
			Logger::Error("Request failed, error: ", e.what());
		}
	}

	void App::RenderRequestPanel()
	{
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGuiContext& g = *GImGui;

		static ImVec2 textInputSize = CalButtomSize("Send");
		static ImVec2 comboSize = CalButtomSize("DELETE");

		ImGui::Begin("Center");

		ImGui::PushItemWidth(comboSize.x * 1.5f);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.2f, 1.f));
		ImGui::Combo("##methods", &m_SelectedMethod, m_Methods.data(), m_Methods.size());
		ImGui::PopItemWidth(); ImGui::SameLine(0, 0);

		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - textInputSize.x);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.2f, 0.2f, 1.f));
		auto returnInput = ImGui::InputText("##url", m_Url, 2048, ImGuiInputTextFlags_EnterReturnsTrue );
		ImGui::PopItemWidth(); ImGui::SameLine(0, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.f)); ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.85f, 0.0f, 1.f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.9f, 0.0f, 1.f));
		bool send = ImGui::Button("Send");

		if (!m_BlockInputAndSend && (send || returnInput))
		{
			m_BlockInputAndSend = true;

			m_ResponseHeader.clear(); m_ResponseBody.clear();

			if (m_Methods[m_SelectedMethod] == "GET")
				CreateRequest(m_Methods[m_SelectedMethod], "", m_RequestHeaders);
			else if (m_Methods[m_SelectedMethod] == "POST")
				CreateRequest(m_Methods[m_SelectedMethod], m_RequestBody, m_RequestHeaders);
			else if (m_Methods[m_SelectedMethod] == "PUT")
				CreateRequest(m_Methods[m_SelectedMethod], m_RequestBody, m_RequestHeaders);
			else if (m_Methods[m_SelectedMethod] == "PATCH")
				CreateRequest(m_Methods[m_SelectedMethod], m_RequestBody, m_RequestHeaders);
			else if (m_Methods[m_SelectedMethod] == "DELETE")
				CreateRequest(m_Methods[m_SelectedMethod], m_RequestBody, m_RequestHeaders);

			m_BlockInputAndSend = false;
		}
		ImGui::PopStyleColor(6);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.f)); ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.5f, 1.f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.8f, 0.5f, 1.f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.8f, 0.5f, 1.f));
		ImGui::Button( m_StatusCode ? ("Code: " + std::to_string(m_StatusCode)).c_str() : "" );
		ImGui::PopStyleColor(4); ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
		ImGui::InputInt("TimeOut(ms)", &m_TimeOut);
		ImGui::PopItemWidth();

		ImGui::BeginChild("CenterChild", ImVec2(ImGui::GetWindowContentRegionWidth(), 0), false);
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Headers"))
			{
				auto plusCharSize = CalButtomSize(" + ");

				size_t i = 0;
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.2f, 0.2f, 1.f));
				for (auto& h : m_RequestHeaders)
				{
					auto twoDotsPos = h.find(":");
					auto key = std::string(h.begin(), h.begin() + twoDotsPos);
					auto value = h.substr(twoDotsPos + 2);
					ImGui::PushID(i);
					ImGui::PushMultiItemsWidths(1, (ImGui::GetContentRegionAvailWidth() * 0.5f) - plusCharSize.x);
					InputText("##header key", &key, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
					InputText("##header value", &value, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
					ImGui::PopItemWidth();
					if (ImGui::Button("  - "))
					{
						m_RequestHeaders.erase(m_RequestHeaders.begin() + i);
					}
					ImGui::PopID();

					i++;
				}
				ImGui::PopStyleColor();

				static std::string key("");
				static std::string value("");
				ImGui::PushID("littleheaders");
				ImGui::PushMultiItemsWidths(1, (ImGui::GetContentRegionAvailWidth() * 0.5f) - plusCharSize.x);
				InputText("##input header key", &key, ImGuiInputTextFlags_EnterReturnsTrue); ImGui::SameLine();
				InputText("##input header value", &value, ImGuiInputTextFlags_EnterReturnsTrue); ImGui::SameLine();
				ImGui::PopItemWidth();
				if (ImGui::Button(" + ") && key.length() && value.length())
				{
					auto finalHeader = key + ": " + value;
					m_RequestHeaders.push_back(finalHeader);
					key.clear(); value.clear();
				}
				ImGui::PopID();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Body"))
			{
				InputTextMultiline("#requestbody", &m_RequestBody, { -FLT_MIN, -FLT_MIN }, ImGuiInputTextFlags_AllowTabInput );
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		ImGui::End();
	}

	void App::RenderRespondePanel()
	{
		ImGui::Begin("Down");
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Headers"))
			{
				ImGui::InputTextMultiline("##respon", m_ResponseHeader.data(),
					m_ResponseHeader.size(), { -FLT_MIN, -FLT_MIN }, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Body"))
			{
				ImGui::InputTextMultiline("##respon", m_ResponseBody.data(),
					m_ResponseBody.size(), { -FLT_MIN, -FLT_MIN }, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}