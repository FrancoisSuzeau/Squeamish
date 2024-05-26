/******************************************************************************************************************************************/
// File : TexturedComponent.hpp
// Purpose : The Component view implementing basis of element on canvas
/******************************************************************************************************************************************/
#ifndef TEXTUREDCOMPONENT_H
#define TEXTUREDCOMPONENT_H


#include "../../Interfaces/IComponent.hpp"

namespace Component
{
	class TexturedComponent : public IComponent
	{
	public:
		TexturedComponent(glm::vec3 const position, glm::vec3 const size, Enums::RendererType const type, std::string const texture_name, 
			bool const mixe = false, bool const is_light_source = false, float const ambiant_occlusion = 0.1f, int const specular_shininess = 2);
		unsigned int GetTextureId() const override;
		void Clean() override;
		bool GetHorizontal() override;
		void SetHorizontal(bool const new_val) override;
		std::string GetTextureName() const override;
		void SetTextureId(unsigned int texture_id) override;
		void SetTextureName(std::string const new_name) override;

		bool GetMixeTextureColor() const override;
		void SetMixeTextureColor(const bool new_val) override;

		void SetTextureColor(glm::vec4 const texture_color) override;
		glm::vec4 GetTextureColor() const override;

	private:
		unsigned int m_texture_id;
		std::string m_texture_name;
		bool m_horizontal;
		bool m_mixe_texture_and_color;

	};
}

#endif