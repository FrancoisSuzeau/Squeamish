/******************************************************************************************************************************************/
// File : PannelComponent.hpp
// Purpose : The GUI view of confirm
/******************************************************************************************************************************************/
#ifndef PANNELCOMPONENT_H
#define PANNELCOMPONENT_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

#define  IMGUI_INITIALIZE 0
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "IView.hpp"
#include "Services/StateService.hpp"
#include "Container/Container.hpp"

#include <iostream>
#include <list>

#include "Commands/ModifyConfigsCommand.hpp"
#include "Commands/ActualizeCommand.hpp"
#include "Commands/SaveConfigCommand.hpp"

namespace Views
{
	class PannelComponent : public IView
	{

	public:
		~PannelComponent() override;
		PannelComponent(std::shared_ptr<ViewModels::IViewModel> parent);
		void Render() override;
	private:
		std::shared_ptr < Services::StateService> m_state_service;
		void RenderGridPannelModifier();
		void RenderBloomPannelModifier();
		void RenderShadowPannelModifier();
		void RenderSkyboxPannelModifier();
		void RendererAntiAliasingModifier();
		void RenderDebugModifier();
		void RenderButtons();
		void OnClose();
		int item_current;
		int previous_item_current;
		bool render_grid;
		bool activate_bloom;
		float trigger;
		int bloom_strength;
		bool activate_debug;
		bool active_skybox;
		bool activate_shadow;
		bool activate_multisample;
		bool show;
		
	};
}

#endif