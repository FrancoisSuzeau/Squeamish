/******************************************************************************************************************************************/
// File : SendToJsonServiceCommand.cpp
// Purpose : Implementing the load new shader command
/******************************************************************************************************************************************/
#include "SendToJsonServiceCommand.hpp"

using namespace Commands;

SendToJsonServiceCommand::SendToJsonServiceCommand()
{
	m_json_service = IoC::Container::Container::GetInstanceContainer()->GetReference<Services::JsonService>();
	if (!m_json_service)
	{
		SQ_APP_ERROR("Class {} in function {} : Json service is not referenced yet", __FILE__, __FUNCTION__);
	}
}

void Commands::SendToJsonServiceCommand::SetRenderers(std::vector<std::shared_ptr<Renderers::IRenderer>> const renderers)
{
	m_renderers = renderers;
}

void SendToJsonServiceCommand::Execute()
{
	if (m_json_service)
	{
		SQ_APP_INFO("Save scene command is called");

		m_json_service->SetScene(m_renderers);
		m_json_service.reset();
		for (std::vector<std::shared_ptr<Renderers::IRenderer>>::iterator it = m_renderers.begin(); it != m_renderers.end(); it++)
		{
			if (it[0])
			{
				it[0].reset();
			}
		}

		m_renderers.clear();
	}
}