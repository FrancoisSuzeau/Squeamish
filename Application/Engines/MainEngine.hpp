/******************************************************************************************************************************************/
// File : MainEngine.hpp
// Purpose : The main engine of the application
/******************************************************************************************************************************************/
#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "GUIEngine.hpp"
#include "SceneEngine.hpp"
#include "Enums/EngineEnum.hpp"
#include "Container/Container.hpp"

#include "../Services/StateService.hpp"
#include "../Services/FramebufferService.hpp"
#include "../Services/RunTimeService.hpp"
#include "../Builders/ViewModelBuilder.hpp"

#include "Constants/StringConstants.hpp"


namespace Engines {

	class MainEngine : public IEngine
	{
	public:

		~MainEngine() override;
		void Construct() override;
		void StartScreen(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void LoadScreen(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void MainLoop(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);


	private:
		std::shared_ptr<SDL_Window> m_window;
		Uint32                  start_loop = 0, end_loop = 0, time_past = 0;
		unsigned int            frame_rate = 0;

		std::shared_ptr<GUIEngine> m_gui_engine;
		std::shared_ptr<SceneEngine> m_scene_engine;

		void InitFrame() override;
		void EndFrame() override;
		void FpsCalculation(Enums::EngineEnum ee);
		void RenderStencilBuffer(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);

		void PassToFrameBuffer(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);

		std::shared_ptr<Services::StateService> m_state_service;
		std::shared_ptr<Services::FramebufferService> m_framebuffer_service;
		std::shared_ptr<Services::RunTimeService> m_runtime_service;
		
	};
}

#endif