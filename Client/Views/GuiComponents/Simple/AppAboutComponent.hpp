/******************************************************************************************************************************************/
// File : AppAboutComponent.hpp
// Purpose : The GUI view of app info
/******************************************************************************************************************************************/
#ifndef APPABOUTCOMPONENT_H
#define APPABOUTCOMPONENT_H

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

namespace Views
{
	class AppAboutComponent : public IView
	{

	public:
		~AppAboutComponent() override;
		AppAboutComponent();
		void Render() override;
	private:
		std::shared_ptr < Services::StateService> m_state_service;
	};
}

#endif