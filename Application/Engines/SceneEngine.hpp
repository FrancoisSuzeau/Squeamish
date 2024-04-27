/******************************************************************************************************************************************/
// File : SceneEngine.hpp
// Purpose : The Scene engine of the application
/******************************************************************************************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include <map>
#include <string>
#include "../Builders/ViewModelBuilder.hpp"

#include "IEngine.hpp"
#include "Constants/StringConstants.hpp"
#include "Renderers/Renderers.hpp"
#include "../Services/Services.hpp"
#include "../Transformer.hpp"

namespace Engines
{
	class SceneEngine : public IEngine
	{
	public:
		~SceneEngine() override;
		void Construct() override;

		void RenderScene(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void RefreshScene(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void RenderScreen(unsigned int const screen_texture);
		void UpdateAll(SDL_Event event);
		
	private:
		void InitFrame() override;
		void EndFrame() override;
		std::shared_ptr<Services::ShaderService> m_shader_service;
		std::shared_ptr<Services::CameraService> m_camera_service;
		std::shared_ptr<Services::StateService> m_state_service;
		std::shared_ptr<Services::MouseInputService> m_mouse_input_service;
		std::shared_ptr<Services::KeyboardInputService> m_keyboad_input_service;
		std::shared_ptr<Services::RunTimeService> m_runtime_service;
		std::unique_ptr<Renderers::ScreenRenderer> m_screen_renderer;
		std::shared_ptr<Component::TexturedComponent> m_screen_component;
		
	};
}


#endif