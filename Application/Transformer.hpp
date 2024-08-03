/******************************************************************************************************************************************/
// File : Transformer.hpp
// Purpose : The Transformer of all component
/******************************************************************************************************************************************/
#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "IComponent.hpp"

#include "Services/ShaderService.hpp"
#include "Services/StateService.hpp"
#include "Services/CameraService.hpp"
#include "Services/RunTimeService.hpp"

#include "Container/Container.hpp"
#include "Constants/NumberConstants.hpp"
#include "Services/PhysicsService.hpp"
#include "Enums/EngineEnum.hpp"

namespace Component
{
	class Transformer
	{
	public:
		static void PutIntoShader(std::shared_ptr<Component::IComponent> component, std::shared_ptr<Services::ShaderService> shader_service, std::string const shader_name);
		static void Move(std::shared_ptr<Component::IComponent> component);
		static void Resize(std::shared_ptr<Component::IComponent> component);
		static void Resize(std::shared_ptr<Component::IComponent> component, float const offset);
		static void Rotate(std::shared_ptr<Component::IComponent> component, Enums::OrientationAngle const or_angle);
		static void ReinitModelMat(std::shared_ptr<Component::IComponent> component);
	private:
		static void SetWorldParameters(std::shared_ptr<Services::ShaderService> shader_service, std::string const shader_name, 
			std::shared_ptr<Services::CameraService> camera_service, std::shared_ptr<Services::StateService> state_service);
		static void SetComponentParameters(std::shared_ptr<Component::IComponent> component, std::shared_ptr<Services::ShaderService> shader_service, 
			std::shared_ptr<Services::StateService> state_service, std::string const shader_name);
	
	};
}

#endif