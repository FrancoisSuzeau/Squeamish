/******************************************************************************************************************************************/
// File : MousePickerService.cpp
// Purpose : Implementing the mouse picker service
/******************************************************************************************************************************************/
#include "MousePickerService.hpp"

namespace Services
{
	void MousePickerService::Init()
	{
		IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
		if (container)
		{
			m_state_service = container->GetReference<StateService>();
			if (!m_state_service)
			{
				SQ_APP_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
			}
			else
			{
				m_proj_mat = m_state_service->GetProjectionMatrix();
				m_view_mat = m_state_service->GetViewMatrix();
			}
		}
	}

	void MousePickerService::DeInit()
	{
		if (m_state_service)
		{
			m_state_service.reset();
		}
	}
	void MousePickerService::Update()
	{
		if (m_state_service)
		{
			m_view_mat = m_state_service->GetViewMatrix();
			this->SetNormalizedDeviceCoords();
			m_current_ray = this->CalculateMouseRay();
		}
	}

	glm::vec3 MousePickerService::GetCurrentRay() const
	{
		return m_current_ray;
	}
	glm::vec3 MousePickerService::CalculateMouseRay()
	{
		if (m_state_service)
		{	
			glm::vec4 clip_coords = glm::vec4(m_mouse_normalized_pos.x, m_mouse_normalized_pos.y, -1.f, 1.f);
			glm::vec4 eye_coords = this->ConvertToEyeCoords(clip_coords);
			glm::vec3 world_ray = this->ConvertToWorldCoords(eye_coords);

			return world_ray;
		}
		return glm::vec3(0.f);
	}
	void MousePickerService::SetNormalizedDeviceCoords()
	{
		if (m_state_service)
		{
			float mouse_x = (float)m_state_service->getXPos();
			float mouse_y = (float)m_state_service->getYPos();

			float x = (2.f * mouse_x) / m_state_service->getWidth() - 1.f;
			float y = (2.f * mouse_y) / m_state_service->getHeight() - 1.f;
			m_mouse_normalized_pos = glm::vec3(x, -y, 0.f);
		}
		
	}
	glm::vec4 MousePickerService::ConvertToEyeCoords(glm::vec4 clip_coords)
	{
		if (m_state_service)
		{
			glm::mat4 inverted_proj_mat = glm::inverse(m_state_service->GetProjectionMatrix());
			glm::vec4 eye_coords = inverted_proj_mat * clip_coords;

			return glm::vec4(eye_coords.x, eye_coords.y, -1.f, 0.f);
		}
		return glm::vec4(0.f);
	}
	glm::vec3 MousePickerService::ConvertToWorldCoords(glm::vec4 eye_coords)
	{
		glm::mat4 inverted_view_mat = glm::inverse(m_view_mat);
		glm::vec4 ray_world = inverted_view_mat * eye_coords;
		return glm::normalize(glm::vec3(ray_world));

	}
}
