/******************************************************************************************************************************************/
// File : MetricsComponent.cpp
// Purpose : Implementing the GUI view metrics
/******************************************************************************************************************************************/

#include "MetricsComponent.hpp"

namespace Views
{
	MetricsComponent::~MetricsComponent()
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
	MetricsComponent::MetricsComponent(std::shared_ptr<ViewModels::IViewModel> parent)
	{
		m_parent_view_model = parent;
		m_state_service = IoC::Container::Container::GetInstanceContainer()->GetReference<Services::StateService>();
		if (!m_state_service)
		{
			SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
		}
	}
	void MetricsComponent::Render()
	{
		if (m_state_service)
		{
			bool show_metrics = m_state_service->getShowMetrics();
		
			if (show_metrics) 
			{ 
				ImGui::ShowMetricsWindow(&show_metrics); 
			}
			m_state_service->setShowMetrics(show_metrics);
		}
	}
}