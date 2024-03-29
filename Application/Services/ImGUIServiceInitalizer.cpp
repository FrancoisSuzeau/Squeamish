/******************************************************************************************************************************************/
// File : ImGUIServiceInitializer.cpp
// Purpose : Implementing service ImGUI initialisation
/******************************************************************************************************************************************/
#include "ImGUIServiceInitalizer.hpp"

namespace Services
{
    void ImGUIServiceInitializer::Init()
    {
        IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
        std::shared_ptr<Services::GraphicInitializerService> graph_service_init = container->GetReference<Services::GraphicInitializerService>();
        if (graph_service_init)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            m_io = ImGui::GetIO();
            ImGui::StyleColorsDark();
            ImGui_ImplSDL2_InitForOpenGL(graph_service_init->GetSDLWindow().get(), graph_service_init->GetGLContext());
            ImGui_ImplOpenGL3_Init(Constants::IMGUIVERSION.c_str());

            SQ_APP_DEBUG("ImGui service SUCCESSFULLY initialized");
            graph_service_init.reset();
        }
        else
        {
            SQ_APP_ERROR("Graphic service initializer is not referenced yet");
        }

        
    }

    void ImGUIServiceInitializer::DeInit()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SQ_APP_DEBUG("ImGui service shutdown");
    }

    ImGuiIO ImGUIServiceInitializer::GetIO()
    {
        return m_io;
    }
}
