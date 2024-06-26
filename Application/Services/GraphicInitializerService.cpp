/******************************************************************************************************************************************/
// File : GraphicInitializerService.cpp
// Purpose : Implementing service Opengl / SDL  initialisation
/******************************************************************************************************************************************/
#include "GraphicInitializerService.hpp"

namespace Services
{
	void GraphicInitializerService::Init()
	{

		init_succeded = true;
		InitialiseSDL();
		assert(init_succeded);
		SetGLAttributes();
		assert(init_succeded);
		SetDimensions();
		assert(init_succeded);
		SetSDLWindow();
		assert(init_succeded);
		SetSDLGLContext();
		assert(init_succeded);
		InitGlew();
		assert(init_succeded);

		int major_version;
		int minor_version;

		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);

		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		SQ_APP_DEBUG("All graphics services SUCCESSFULLY initialized");
		SQ_APP_INFO("OpenGL v{}.{} ready", major_version, minor_version);
	}


	void GraphicInitializerService::DeInit()
	{
		DestroySDLGLContext();
		DestroySDLWindow();
		DeInitSDL();
		SQ_APP_DEBUG("All graphics services shutdown");
	}

	SDL_GLContext GraphicInitializerService::GetGLContext() const
	{
		return gl_context;
	}

	std::shared_ptr<SDL_Window> GraphicInitializerService::GetSDLWindow() const
	{
		return m_window;
	}

	int GraphicInitializerService::GetWidth() const
	{
		return m_width;
	}

	int GraphicInitializerService::GetHeight() const
	{
		return m_height;
	}

	void GraphicInitializerService::InitialiseSDL()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : SDL FAILED to initialize - SDL Error : {}", __FILE__, __FUNCTION__, SDL_GetError());
			init_succeded = false;
		}
	}

	void GraphicInitializerService::SetGLAttributes()
	{
		if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot set Stencil buffer - SDL Error : {}", __FILE__, __FUNCTION__, Constants::Major_version, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Constants::Major_version) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot set Open GL Major version to {} - SDL Error : {}", __FILE__, __FUNCTION__, Constants::Major_version, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}

		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Constants::Minor_version) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot set Open GL Minor version to {} - SDL Error : {}", __FILE__, __FUNCTION__, Constants::Minor_version, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}

		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot use profile mask and compatibility - SDL Error : {}", __FILE__, __FUNCTION__, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}

		if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot use double buffering - SDL Error : {}", __FILE__, __FUNCTION__, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}
	}

	void GraphicInitializerService::SetDimensions()
	{
		m_width = GetSystemMetrics(SM_CXSCREEN);
		m_height = GetSystemMetrics(SM_CYSCREEN);
	}

	void GraphicInitializerService::SetSDLWindow()
	{
		m_window = std::shared_ptr<SDL_Window> (SDL_CreateWindow("Squeamish", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL), SDLWindowDeleter());
		if (!m_window)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot create SDL Window - SDL Error : {}", __FILE__, __FUNCTION__, SDL_GetError());
			init_succeded = false;
			SDL_Quit();
		}
	}

	void GraphicInitializerService::SetSDLGLContext()
	{
		gl_context = SDL_GL_CreateContext(m_window.get());
		if (gl_context == 0)
		{
			SQ_APP_ERROR("Class {} in function {} : Cannot create SDL GL context - SDL Error : {}", __FILE__, __FUNCTION__, SDL_GetError());
			init_succeded = false;
			if (m_window)
			{
				SDL_DestroyWindow(m_window.get());
				m_window.reset();
			}
			SDL_Quit();
		}
	}

	void GraphicInitializerService::InitGlew()
	{
		GLenum glew = 0;
		glew = glewInit();
		if (glew != GLEW_OK)
		{
			SQ_APP_ERROR("Class {} in function {} : Glew FAILED to initialize - Glew Error : {}", __FILE__,  __FUNCTION__, (const char*)glewGetErrorString(glew));
			init_succeded = false;
			SDL_GL_DeleteContext(gl_context);
			if (m_window)
			{
				SDL_DestroyWindow(m_window.get());
				m_window.reset();
			}
			SDL_Quit();
		}
	}

	void GraphicInitializerService::DestroySDLWindow()
	{
		if (m_window.use_count() != 0)
		{
			SDL_DestroyWindow(m_window.get());
			m_window.reset();
		}
	}

	void GraphicInitializerService::DestroySDLGLContext()
	{
		SDL_GL_DeleteContext(gl_context);
	}

	void GraphicInitializerService::DeInitSDL()
	{
		SDL_Quit();
	}
}

