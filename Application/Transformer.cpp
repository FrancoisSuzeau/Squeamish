/******************************************************************************************************************************************/
// File : Transformer.cpp
// Purpose : Implementing the transformer 
/******************************************************************************************************************************************/

#include "Transformer.hpp"

namespace Component
{
	void Transformer::PutIntoShader(std::shared_ptr<Component::IComponent> component, std::shared_ptr<Services::ShaderService> shader_service, std::string const shader_name)
	{
		IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
		if (container)
		{
			std::shared_ptr<Services::StateService> state_service = container->GetReference<Services::StateService>();
			if (!state_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
			}

			std::shared_ptr<Services::CameraService> camera_service = container->GetReference<Services::CameraService>();
			if (!camera_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Camera service is not referenced yet", __FILE__, __FUNCTION__);
			}

			std::shared_ptr<Services::RunTimeService> runtime_service = container->GetReference<Services::RunTimeService>();
			if (!runtime_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Runtime service is not referenced yet", __FILE__, __FUNCTION__);
			}

			std::shared_ptr<Services::PhysicsService> physics_service = container->GetReference<Services::PhysicsService>();
			if (!physics_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Physics service is not referenced yet", __FILE__, __FUNCTION__);
			}

			if (component && shader_service && state_service && state_service->getConfigs() && camera_service && runtime_service && physics_service)
			{
				SetWorldParameters(shader_service, shader_name, camera_service, state_service);
				SetComponentParameters(component, shader_service, state_service, shader_name);

				shader_service->setInt(shader_name, "render_skybox", state_service->getConfigs()->GetRenderSkybox());
				shader_service->setVec(shader_name, "camera_pos", camera_service->GetPos());
				if (state_service->GetScene())
				{
					if (component->GetType() == Enums::RendererType::SPHERE_X || component->GetType() == Enums::RendererType::SPHERE_Y || component->GetType() == Enums::RendererType::SPHERE_Z)
					{
						shader_service->setInt(shader_name, "there_is_light", false);
					}
					else
					{
						bool there_is_light = physics_service->GetLigthSources().size() > 0 || state_service->GetScene()->GetIsThereDirectionLight();
						shader_service->setInt(shader_name, "there_is_light", there_is_light);
					}
				}
				
				for (size_t i = 0; i < physics_service->GetLightSourcesTextureIds().size(); i++)
				{
					shader_service->setInt(shader_name, "light_textures[" + std::to_string((int)i) + "]", 2 + (int)i);
				}
				shader_service->setInt(shader_name, "light_sources_count", (int)physics_service->GetLigthSources().size());
				shader_service->setInt(shader_name, "bloom", state_service->getConfigs()->GetBloom());


				

				state_service.reset();
				camera_service.reset();
				runtime_service.reset();
			}
			
		}
		
	}

	void Transformer::PutIntoShader(std::vector<Renderers::Texturate> mesh_material, std::shared_ptr<Services::ShaderService> shader_service, std::string const shader_name)
	{
		if (shader_service)
		{
			unsigned int diffuse_nr = 1;
			unsigned int specular_nr = 1;
			unsigned int normal_nr = 1;
			unsigned int height_nr = 1;

			for (unsigned int i(0); i < mesh_material.size(); i++)
			{
				std::string number;
				std::string name = mesh_material[i].type;

				if (name == "texture_diffuse")
				{
					number = std::to_string(diffuse_nr++);
				}
				if (name == "texture_specular")
				{
					number = std::to_string(specular_nr++);
				}
				if (name == "texture_normal")
				{
					number = std::to_string(normal_nr++);
				}
				if (name == "texture_height")
				{
					number = std::to_string(height_nr++);
				}

				std::string complete_location = "component." + (name + number);

				shader_service->setTexture(shader_name, complete_location.c_str(), i);
			}
		}
	}

	void Transformer::Move(std::shared_ptr<Component::IComponent> component)
	{
		if (component)
		{
			glm::mat4 model = component->GetModelMat(Enums::ModelMatType::GENERAL);
			model = glm::translate(model, component->GetPosition());
			component->SetModelMat(model);
		}
	}
	void Transformer::Resize(std::shared_ptr<Component::IComponent> component)
	{
		if (component)
		{
			glm::mat4 model = component->GetModelMat(Enums::ModelMatType::GENERAL);
			model = glm::scale(model, component->GetSize());
			component->SetModelMat(model);
		}
	}
	void Transformer::Resize(std::shared_ptr<Component::IComponent> component, float const offset)
	{
		if (component)
		{
			glm::mat4 model = component->GetModelMat(Enums::ModelMatType::GENERAL);
			glm::vec3 const actual_size = component->GetSize();
			model = glm::scale(model, glm::vec3(actual_size.x + offset, actual_size.y + offset, actual_size.z + offset));
			component->SetModelMat(model);
		}
	}
	void Transformer::Rotate(std::shared_ptr<Component::IComponent> component, Enums::OrientationAngle const or_angle)
	{
		glm::mat4 model = component->GetModelMat(Enums::ModelMatType::GENERAL);
		glm::mat4 pitch_mat = component->GetModelMat(Enums::ModelMatType::PITCH);
		glm::mat4 yaw_mat = component->GetModelMat(Enums::ModelMatType::YAW);
		glm::mat4 roll_mat = component->GetModelMat(Enums::ModelMatType::ROLL);
		switch (or_angle)
		{
		case Enums::OrientationAngle::XAXIS:
			pitch_mat = glm::rotate(pitch_mat, glm::radians(component->GetAngle1()), glm::vec3(1.f, 0.f, 0.f));
			break;
		case Enums::OrientationAngle::YAXIS:
			yaw_mat = glm::rotate(yaw_mat, glm::radians(component->GetAngle2()), glm::vec3(0.f, 1.f, 0.f));
			break;
		case Enums::OrientationAngle::ZAXIS:
			roll_mat = glm::rotate(roll_mat, glm::radians(component->GetAngle3()), glm::vec3(0.f, 0.f, 1.f));
			break;
		default:
			break;
		}

		model *= (yaw_mat * pitch_mat * roll_mat);
		component->SetModelMat(model);
	}
	void Transformer::ReinitModelMat(std::shared_ptr<Component::IComponent> component)
	{
		if (component)
		{
			component->SetModelMat(glm::mat4(1.f));
		}
	}
	void Transformer::SetWorldParameters(std::shared_ptr<Services::ShaderService> shader_service, std::string const shader_name, std::shared_ptr<Services::CameraService> camera_service, std::shared_ptr<Services::StateService> state_service)
	{
		if (camera_service && shader_service && state_service)
		{
			shader_service->setMat4(shader_name, "projection", state_service->GetPerspectiveProjectionMatrix());

			if (shader_name == Constants::SKYBOX_SHADER)
			{
				shader_service->setMat4(shader_name, "view", glm::mat4(glm::mat3(camera_service->GetCameraView())));
			}
			else
			{
				shader_service->setMat4(shader_name, "view", camera_service->GetCameraView());

			}
		}
	}
	
	void Transformer::SetComponentParameters(std::shared_ptr<Component::IComponent> component, std::shared_ptr<Services::ShaderService> shader_service, std::shared_ptr<Services::StateService> state_service, std::string const shader_name)
	{
		if (component && shader_service && state_service && state_service->getConfigs())
		{
			shader_service->setInt(shader_name, "component.render_line", component->GetHovered() || component->GetSelected());
			shader_service->setInt(shader_name, "component.mixe_texture_color", component->GetMixeTextureColor());
			shader_service->setVec(shader_name, "component.background_color", component->GetBackgroundColor());
			shader_service->setMat4(shader_name, "model", component->GetModelMat(Enums::ModelMatType::GENERAL));
			shader_service->setInt(shader_name, "component.is_light_source", component->GetIsALightSource());
			shader_service->setInt(shader_name, "component.is_spot_light", component->GetLightType() == Enums::LightType::SPOTLIGHT);
			shader_service->setVec(shader_name, "component.direction", component->GetDirection());
			shader_service->setFloat(shader_name, "component.cut_off", glm::cos(glm::radians(component->GetCutOff())));
			shader_service->setFloat(shader_name, "component.intensity", component->GetIntensity());
			shader_service->setInt(shader_name, "component.specular_shininess", component->GetSpecularShininess());
			shader_service->setFloat(shader_name, "component.specular_strength", component->GetSpecularStrength());
			shader_service->setFloat(shader_name, "component.ambiant_strength", component->GetAmbiantOcclusion());
			shader_service->setTexture(shader_name, "component.texture0", 0);
			shader_service->setTexture(shader_name, "component.texture1", 1);

			bool render_bloom = state_service->getConfigs()->GetBloom();
			if (render_bloom)
			{
				shader_service->setInt(shader_name, "component.horizontal", component->GetHorizontal());
				shader_service->setFloat(shader_name, "component.alpha_strength", state_service->getConfigs()->GetMultiSample() ? 0.5f : 0.2f);
			}
			else
			{
				shader_service->setInt(shader_name, "component.horizontal", false);
				shader_service->setFloat(shader_name, "component.alpha_strength", 0.8f);

			}
		}
	}
}