/******************************************************************************************************************************************/
// File : MenuFileComponent.cpp
// Purpose : Implementing the GUI view app style editor
/******************************************************************************************************************************************/

#include "MenuFileComponent.hpp"

namespace Views
{
	MenuFileComponent::MenuFileComponent() : show_save_as(false), filename("")
	{
		m_state_service = IoC::Container::Container::GetInstanceContainer()->GetReference<Services::StateService>();
		if (!m_state_service)
		{
			SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
		}
	}
	MenuFileComponent::~MenuFileComponent()
	{
		this->SetParent(nullptr);
		if (m_state_service)
		{
			m_state_service.reset();
		}

		if (m_scene_viewmodel)
		{
			m_scene_viewmodel.reset();
		}
	}
	void MenuFileComponent::Render()
	{	
		if (ImGui::BeginMenu("File") && m_parent_view_model && m_state_service)
		{
			if (ImGui::MenuItem("New", "Ctrl+Shift+N")) 
			{
			}
			if (ImGui::BeginMenu("Open", m_state_service->getContinued()) )
			{
				for (auto it : m_parent_view_model->GetConfig()->GetCreatedScenes())
				{
					if (ImGui::MenuItem(it.c_str()))
					{
						std::cout << it << std::endl;
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Save scene", "Ctrl+S", false, m_state_service->getContinued()))
			{
				this->SaveScene();
			}

			ImGui::MenuItem("Save scene As..", "Ctrl+Shift+S", &show_save_as, m_state_service->getContinued());
			

			ImGui::Separator();

			if (ImGui::MenuItem("Quit", "Alt+F4") && m_parent_view_model)
			{
				m_parent_view_model->OnCommand(new Commands::SaveConfigCommand());
				m_parent_view_model->OnCommand(new Commands::ExitCommand());
			}
			ImGui::EndMenu();
		}
		
		this->ShowSaveAsWindow(400, 200);
		
	}
	void MenuFileComponent::SetSceneViewModel(ViewModels::IViewModel* scene_viewmodel)
	{
		m_scene_viewmodel = std::shared_ptr<ViewModels::IViewModel>(scene_viewmodel);
	}
	void MenuFileComponent::ShowSaveAsWindow(int w_width, int w_height)
	{
		m_state_service->setGuiOpen(show_save_as);
		if (show_save_as)
		{
			ImGui::SetNextWindowPos(ImVec2((float)((m_state_service->getWidth() / 2) - (w_width / 2)), (float)((m_state_service->getHeight() / 2) - (w_height / 2))));
			ImGui::SetNextWindowSize(ImVec2((float)w_width, (float)w_height));
			
			if (ImGui::Begin("Scene name :", &show_save_as))
			{
				
				ImGui::InputText("Enter name here ...", filename, IM_ARRAYSIZE(filename));
				ImGuiStyle& style = ImGui::GetStyle();
				float frame_rounding_save = style.FrameRounding;
				style.FrameRounding = 20.f;
				if (ImGui::Button("Save", ImVec2((float)(w_width - 15), 30.f)) && m_state_service)
				{
					m_state_service->setFileName(filename);
					if (m_parent_view_model)
					{
						m_parent_view_model->ChangeConfig(Enums::ConfigModifier::ADDFILE, filename);
						m_parent_view_model->OnCommand(new Commands::SendToJsonServiceCommand());
					}
					this->SaveScene();
					show_save_as = false;
					filename[0] = '\0';
				}
				style.FrameRounding = frame_rounding_save;
				ImGui::End();
			}

		}
	}
	void MenuFileComponent::SaveScene()
	{
		if (m_state_service && m_scene_viewmodel)
		{
			m_scene_viewmodel->OnCommand(new Commands::SendToJsonServiceCommand());
			m_parent_view_model->OnCommand(new Commands::SaveSceneCommand(m_state_service->getFileName()));
		}
	}
}