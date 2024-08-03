/******************************************************************************************************************************************/
// File : IComponent.hpp
// Purpose : An interface for the renderers
/******************************************************************************************************************************************/
#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "IRenderer.hpp"

namespace Enums { enum LightType {NORMALIGHT, POINTLIGHT, SPOTLIGHT, NBLIGHTTYPE }; }

namespace Component {

	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual void Clean() = 0;

		virtual bool GetHorizontal() { return false; }
		virtual void SetHorizontal(bool const new_val) { }

		virtual unsigned int GetTextureId() const { return 0; };
		virtual std::string GetTextureName() const { return ""; }
		virtual void SetTextureId(unsigned int texture_id) {}
		virtual void SetTextureName(std::string const new_name) {}
		
		virtual bool GetMixeTextureColor() const { return true; }
		virtual void SetMixeTextureColor(const bool new_val) {}

		virtual Enums::RendererType GetType() const
		{
			return m_type;
		}


		virtual glm::mat4 GetModelMat() const
		{
			return m_model_mat;
		}

		virtual void SetModelMat(glm::mat4 model_mat)
		{
			m_model_mat = model_mat;
		}

		virtual glm::vec3 GetPosition() const
		{
			return m_position;
		}

		virtual void SetPosition(glm::vec3 const new_val)
		{
			m_position = new_val;
		}

		virtual glm::vec3 GetSize() const
		{
			return m_size;
		}

		virtual void SetSize(glm::vec3 const new_val)
		{
			m_size = new_val;
		}

		virtual void SetSize(float const new_val)
		{
			m_size = glm::vec3(new_val);
		}

		virtual void SetHovered(bool const new_val)
		{
			m_hovered = new_val;
		}

		virtual void SetSelected(bool const new_val)
		{
			m_selected = new_val;
		}

		virtual bool GetHovered() const
		{
			return m_hovered;
		}

		virtual bool GetSelected() const
		{
			return m_selected;
		}

		virtual void SetAngle1(float const angle)
		{
			m_angle_1 = angle;
		}

		virtual float GetAngle1() const
		{
			return m_angle_1;
		}

		virtual void SetAngle2(float const angle)
		{
			m_angle_2 = angle;
		}

		virtual float GetAngle2() const
		{
			return m_angle_2;
		}


		virtual void SetAngle3(float const angle)
		{
			m_angle_3 = angle;
		}

		virtual float GetAngle3() const
		{
			return m_angle_3;
		}

		virtual glm::vec4 GetBackgroundColor() const 
		{
			return m_back_ground_color;
		}
		virtual void SetBackgroundColor(glm::vec4 new_bacground_color)
		{
			m_back_ground_color = new_bacground_color;
		}

		virtual void SetIsALigthSource(bool const is_light_source) 
		{
			m_is_light_source = is_light_source; 
		}
		virtual bool GetIsALightSource() const 
		{
			return m_is_light_source; 
		}

		virtual void SetAmbiantOcclusion(float const ambiant_occlusion)
		{
			m_ambiant_occlusion = ambiant_occlusion;
		}
		virtual float GetAmbiantOcclusion() const
		{
			return m_ambiant_occlusion;
		}

		virtual void SetSpecularShininess(int const specular_shininess)
		{
			m_specular_shininess = specular_shininess;
		}

		virtual int GetSpecularShininess() const
		{
			return m_specular_shininess;
		}

		virtual void SetSpecularStrength(float const specular_strength)
		{
			m_specular_strength = specular_strength;
		}

		virtual float GetSpecularStrength() const
 		{
			return m_specular_strength;
		}

		virtual void SetLightType(Enums::LightType light_type)
		{
			m_light_type = light_type;
		}

		Enums::LightType GetLightType() const
		{
			return m_light_type;
		}

		virtual void SetDirection(glm::vec3 const direction)
		{
			m_direction = direction;
		}

		virtual glm::vec3 GetDirection() const
		{
			return m_direction;
		}

		virtual float GetCutOff() const
		{
			return m_cut_off;
		}

		virtual void SetCutOff(float const cut_off)
		{
			m_cut_off = cut_off;
		}

		virtual float GetOuterCutOff() const
		{
			return m_outer_cut_off;
		}

		virtual void SetOuterCutOff(float const outer_cut_off)
		{
			m_outer_cut_off = outer_cut_off;
		}

		virtual void SetIsAttenuation(bool const is_attenuation)
		{
			m_is_attenuation = is_attenuation;
		}

		virtual bool GetIsAttenuation() const
		{
			return m_is_attenuation;
		}

		virtual void SetIntensity(float const intensity)
		{
			m_intensity = intensity;
		}

		virtual float GetIntensity() const
		{
			return m_intensity;
		}
 
	protected:
		Enums::RendererType m_type;

		glm::mat4 m_model_mat;
		glm::vec3 m_position;
		glm::vec3 m_size;
		bool m_hovered;
		bool m_selected;
		float m_angle_1;
		float m_angle_2;
		float m_angle_3;
		glm::vec4 m_back_ground_color;
		bool m_is_light_source;
		float m_ambiant_occlusion;
		int m_specular_shininess;
		float m_specular_strength;
		glm::vec3 m_direction;
		
		Enums::LightType m_light_type;
		float m_cut_off;
		float m_outer_cut_off;
		bool m_is_attenuation;
		float m_intensity;
		

	};
}

#endif