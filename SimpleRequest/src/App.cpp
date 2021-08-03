#include "App.h"

namespace simple {
	App::App()
		: m_Window(std::make_unique<Window>(640, 480, "Hola mundo")),
		m_ImGuiApp(m_Window->GetWindow())
	{
	}

	void App::Start()
	{
		while (!m_Window->isClose())
		{
			m_Window->PollEvents();
			m_ImGuiApp.Begin();

            ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoTabBar;
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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


                    // we now dock our windows into the docking node we made above
                    ImGui::DockBuilderDockWindow("Center", dockspaceID);
                    ImGui::DockBuilderFinish(dockspaceID);
                }
            }
            ImGui::End();

            ImGui::Begin("Center");
            ImGui::Text("Hello, center!");
            ImGui::End();

            ImGui::Render();

			m_Window->Clear();
			m_ImGuiApp.End();
			m_Window->Display();
		}
	}

}