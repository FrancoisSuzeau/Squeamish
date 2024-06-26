/******************************************************************************************************************************************/
// File : AppAboutComponent.cpp
// Purpose : Implementing the GUI view app info
/******************************************************************************************************************************************/

#include "AppAboutComponent.hpp"

namespace Views
{
	AppAboutComponent::~AppAboutComponent()
	{
		if (m_state_service)
		{
			m_state_service.reset();
		}

		if (m_parent_view_model)
		{
			m_parent_view_model.reset();
		}
	}
	AppAboutComponent::AppAboutComponent(std::shared_ptr<ViewModels::IViewModel> parent)
	{
		m_parent_view_model = parent;
		m_state_service = IoC::Container::Container::GetInstanceContainer()->GetReference<Services::StateService>();
		if (!m_state_service)
		{
			SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
		}
	}
	void AppAboutComponent::Render()
	{
		if (m_state_service)
		{
			bool show_infos = m_state_service->getShowInfos();
			if (show_infos) 
			{ 
				ImGui::ShowAboutWindow(&show_infos); 
			}
			m_state_service->setShowInfos(show_infos);
		}
	}
}