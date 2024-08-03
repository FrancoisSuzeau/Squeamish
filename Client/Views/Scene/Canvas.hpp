/******************************************************************************************************************************************/
// File : Canvas.hpp
// Purpose : The canvas view of the scene
/******************************************************************************************************************************************/
#ifndef CANVAS_H
#define CANVAS_H

#include "IView.hpp"
#include "Services/ShaderService.hpp"
#include "Services/RunTimeService.hpp"
#include "Services/StateService.hpp"
#include "Services/PhysicsService.hpp"
#include "IViewModel.hpp"

#include "Renderers/Untextured/Square.hpp"
#include "Renderers/Untextured/Triangle.hpp"
#include "Renderers/Textured/CubeTextured.hpp"
#include "Renderers/Untextured/Cube.hpp"
#include "Renderers/Untextured/Sphere.hpp"
#include "Renderers/Textured/TriangleTextured.hpp"
#include "Renderers/Textured/SquareTextured.hpp"
#include "Renderers/Textured/SphereTextured.hpp"

#include "Enums/EngineEnum.hpp"
#include "IComponent.hpp"

#include "Draggable.hpp"
#include "Transformer.hpp"
#include <map>
#include <iostream>

namespace Views
{
	class Canvas : public IView
	{
	public:
		Canvas(std::shared_ptr<ViewModels::IViewModel> parent);
		void Clean() override;
		void Render(std::vector<std::shared_ptr<Component::IComponent>> components) override;
		void TransformComponents(std::vector<std::shared_ptr<Component::IComponent>> components) override;
		void DragComponents(std::vector<std::shared_ptr<Component::IComponent>> components) override;
		void ConstructRenderer() override;
	private:
		std::shared_ptr<Services::ShaderService> m_shader_service;
		std::unique_ptr<Views::Draggable> m_draggable_component;
		std::map<Enums::RendererType, std::unique_ptr<Renderers::IRenderer>> m_renderers;
		std::shared_ptr<Services::RunTimeService> m_runtime_service;
		std::shared_ptr<Services::StateService> m_state_service;
		std::shared_ptr<Services::PhysicsService> m_physics_service;

		void RenderLightSources(std::vector<std::shared_ptr<Component::IComponent>> components);
		void RenderOtherComponents(std::vector<std::shared_ptr<Component::IComponent>> components);

		void DrawComponent(std::shared_ptr<Component::ComponentBase> component, std::string const shader_name);
		void DrawComponent(std::shared_ptr<Component::TexturedComponent> component, std::string const shader_name);
	};
}

#endif