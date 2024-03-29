/******************************************************************************************************************************************/
// File : SceneViewModel.cpp
// Purpose : Implementing the GUI view model
/******************************************************************************************************************************************/

#include "SceneViewModel.hpp"

namespace ViewModels
{
	SceneViewModel::~SceneViewModel()
	{
		if (m_canvas)
		{
			m_canvas->Clean();
			m_canvas.reset();
		}

		if (m_loader_service)
		{
			m_loader_service.reset();
		}

		if (m_shader_service)
		{
			m_shader_service.reset();
		}

		if (m_state_service)
		{
			m_state_service.reset();
		}

		if (m_framebuffer_service)
		{
			m_framebuffer_service.reset();
		}

		if (m_runtime_service)
		{
			m_runtime_service.reset();
		}

		for (std::map<Enums::RendererType, std::unique_ptr<Renderers::IRenderer>>::iterator it = m_renderers.begin(); it != m_renderers.end(); it++)
		{
			if (it->second)
			{
				it->second->Clean();
				it->second.reset();
			}
		}
		m_renderers.clear();

		for (std::map<Enums::RendererType, std::shared_ptr<Component::IComponent>>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			if (it->second)
			{
				it->second->Clean();
				it->second.reset();
			}
		}
		m_components.clear();
	}
	void SceneViewModel::Construct()
	{
		IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
		if (container)
		{
			m_state_service = container->GetReference<Services::StateService>();
			if (!m_state_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
			}
			
			m_canvas = std::make_unique<Views::Canvas>(shared_from_this());
			if (m_canvas)
			{
				//m_canvas->SetParent(shared_from_this());
				m_canvas->ConstructRenderer();
			}
			else
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Canvas component is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_shader_service = container->GetReference<Services::ShaderService>();
			if (!m_shader_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Shader service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_loader_service = container->GetReference<Services::LoaderService>();
			if (!m_loader_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Loader service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_framebuffer_service = container->GetReference<Services::FramebufferService>();
			if (!m_framebuffer_service)
			{
				SQ_APP_ERROR("Class {} in function {} : Framebuffer service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_runtime_service = container->GetReference<Services::RunTimeService>();
			if (!m_runtime_service)
			{
				SQ_APP_ERROR("Class {} in function {} : Runtime service is not referenced yet", __FILE__, __FUNCTION__);
			}

			if (m_loader_service)
			{
				m_components.insert_or_assign(Enums::RendererType::SKYBOX, std::make_shared<Component::TexturedComponent>(glm::vec3(0.f), glm::vec3(0.f), Enums::RendererType::SKYBOX, m_loader_service->LoadSkybox("resources/skybox/calm_lake")));
			}
			m_components.insert_or_assign(Enums::RendererType::GRID, std::make_unique<Component::ComponentBase>(glm::vec3(-10.f, -1.f, -5.f), glm::vec3(20.f), Enums::RendererType::GRID, glm::vec4(1.f)));


			m_renderers.insert_or_assign(Enums::RendererType::SKYBOX, std::make_unique<Renderers::Skybox>());
			m_renderers.insert_or_assign(Enums::RendererType::GRID, std::make_unique<Renderers::Grid>(48));
			for (std::map<Enums::RendererType, std::unique_ptr<Renderers::IRenderer>>::iterator it = m_renderers.begin(); it != m_renderers.end(); it++)
			{
				if (it->second)
				{
					it->second->Construct();
				}
			}
		}
	}

	void SceneViewModel::RenderComponents()
	{
		if (m_state_service)
		{
			std::vector<std::shared_ptr<Component::IComponent>> components = m_state_service->getComponents();
			if (m_canvas)
			{
				m_canvas->Render(components);
			}
		}
	}
	void SceneViewModel::ManageScene()
	{
		if (m_state_service)
		{
			std::vector<std::shared_ptr<Component::IComponent>> components = m_state_service->getComponents();
			if (m_canvas)
			{
				m_canvas->DragComponents(components);
				m_canvas->TransformComponents(components);
			}
		}

		this->TransformSceneElements();
	}

	void SceneViewModel::RenderSceneElements(Enums::RendererType element)
	{

		if (m_renderers.contains(element) && m_renderers.at(element) && m_shader_service && m_runtime_service && m_components.contains(element) && m_components.at(element))
		{

			switch (element)
			{
			case Enums::GRID:
				m_shader_service->BindShaderProgram(Constants::UNTEXTURED_SHADER);
				Component::Transformer::PutIntoShader(m_components.at(element), m_shader_service, Constants::UNTEXTURED_SHADER);
				m_renderers.at(element)->Draw();
				m_shader_service->UnbindShaderProgram();
				break;
			case Enums::SKYBOX:
				m_runtime_service->LequalDepth();
				m_shader_service->BindShaderProgram(Constants::SKYBOX_SHADER);
				Component::Transformer::PutIntoShader(m_components.at(element), m_shader_service, Constants::SKYBOX_SHADER);
				m_renderers.at(element)->Draw(m_components.at(element)->GetTextureId());
				m_shader_service->UnbindShaderProgram();
				m_runtime_service->LessDepth();
				break;
			case Enums::TRIANGLE:
			case Enums::SQUARE:
			case Enums::SQUARE_TEXTURED:
			case Enums::NONE:
			default:
				break;
			}
		}
	}
	
	void SceneViewModel::TransformSceneElements()
	{
		for (std::map<Enums::RendererType, std::shared_ptr<Component::IComponent>>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			if (it->second)
			{
				Component::Transformer::ReinitModelMat(it->second);
				Component::Transformer::Move(it->second);
				Component::Transformer::Resize(it->second);
			}
		}
	}
}

