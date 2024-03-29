/******************************************************************************************************************************************/
// File : LoadSceneCommand.cpp
// Purpose : Implementing the load new shader command
/******************************************************************************************************************************************/
#include "LoadSceneCommand.hpp"

namespace Commands
{
	LoadSceneCommand::LoadSceneCommand(std::string const filename)
	{
		IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
		if (container)
		{
			m_state_service = container->GetReference<Services::StateService>();
			if (!m_state_service)
			{
				SQ_APP_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
			}
			else
			{
				m_state_service->setFileName(filename);
			}

			m_loader_service = container->GetReference<Services::LoaderService>();
			if (!m_loader_service)
			{
				SQ_APP_ERROR("Class {} in function {} : Json service is not referenced yet", __FILE__, __FUNCTION__);
			}
		}
	}

	LoadSceneCommand::~LoadSceneCommand()
	{
		if (m_state_service)
		{
			m_state_service.reset();
		}

		if (m_loader_service)
		{
			m_loader_service.reset();
		}
	}

	void LoadSceneCommand::Execute()
	{
		if (m_loader_service && m_state_service)
		{
			m_state_service->setComponents(m_loader_service->LoadScene(m_state_service->getFileName()));
			m_state_service.reset();
			m_loader_service.reset();
		}
	}
}


