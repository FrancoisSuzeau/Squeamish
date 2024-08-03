/******************************************************************************************************************************************/
// File : ComponentBase.cpp
// Purpose : Implementing the basic component view
/******************************************************************************************************************************************/

#include "ComponentBase.hpp"

namespace Component
{
	ComponentBase::ComponentBase(glm::vec3 const position, glm::vec3 const size, Enums::RendererType const type, glm::vec4 const color, 
		bool const is_light_source, float const ambiant_occlusion, int const specular_shininess, float const specular_strength, Enums::LightType const light_type, glm::vec3 direction, float const cut_off,
		float const outer_cut_off, bool const is_attenuation, float const intensity)
	{
		m_type = type;

		m_model_mat = glm::mat4(1.f);
		m_position = position;
		m_size = size;
		m_hovered = false;
		m_selected = false;
		m_angle_1 = 0.f;
		m_angle_2 = 0.f;
		m_angle_3 = 0.f;
		m_back_ground_color = color;
		m_is_light_source = is_light_source;
		m_ambiant_occlusion = ambiant_occlusion;
		m_specular_shininess = specular_shininess;
		m_specular_strength = specular_strength;
		m_light_type = light_type;
		m_direction = direction;
		m_cut_off = cut_off;
		m_outer_cut_off = outer_cut_off;
		m_is_attenuation = is_attenuation;
		m_intensity = intensity;
	}

	void ComponentBase::Clean()
	{

	}
}