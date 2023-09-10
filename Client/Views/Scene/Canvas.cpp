/******************************************************************************************************************************************/
// File : Canvas.cpp
// Purpose : Implementing the GUI view app style editor
/******************************************************************************************************************************************/

#include "Canvas.hpp"

namespace Views
{
	Canvas::Canvas()
	{
		m_shader_service = IoC::Container::Container::GetInstanceContainer()->make<Services::ShaderService>();
		if (m_shader_service)
		{
			m_shader_service->LoadShader(Constants::TRIANGLE_SHADER, Enums::NORMAL);
		}

		m_triangle = std::make_unique<Renderers::Triangle>();
		if (m_triangle)
		{
			m_triangle->Construct();
		}
	}

	void Canvas::Clean()
	{
		if (m_triangle)
		{
			m_triangle->Clean();
			m_triangle.reset();
		}

		if (m_shader_service)
		{
			m_shader_service->DeInit();
			m_shader_service.reset();
		}
	}

	void Canvas::Render()
	{
		if (m_shader_service)
		{
			GLuint program_id = m_shader_service->GetProgramId(Constants::TRIANGLE_SHADER);
			m_shader_service->setVec3(Constants::TRIANGLE_SHADER, "background_color", glm::vec3(0.f, 1.f, 0.f));
			if (program_id != Constants::Return_error && m_triangle)
			{
				m_triangle->Render(program_id);
			}
		}
	}
}