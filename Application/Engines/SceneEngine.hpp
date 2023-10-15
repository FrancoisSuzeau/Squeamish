/******************************************************************************************************************************************/
// File : SceneEngine.hpp
// Purpose : The Scene engine of the application
/******************************************************************************************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include <map>
#include <string>
#include "../Builders/ViewModelBuilder.hpp"

#include "../Shaders/Shader.hpp"
#include "IEngine.hpp"
#include "Constants/StringConstants.hpp"
#include "Renderers/Renderers.hpp"
#include "../Services/Services.hpp"

namespace Engines
{
	class SceneEngine : public IEngine
	{
	public:
		void Construct() override;

		void RenderScene(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void RenderFrameBuffer(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder, unsigned int fb_texture_id);
		void RenderSkybox(std::shared_ptr<Builders::ViewModelBuilder> view_model_builder);
		void MoveCamera(SDL_Event event);
		
	private:
		void InitFrame() override;
		void EndFrame() override;
		
		std::shared_ptr<Services::ShaderService> m_shader_service;
		std::shared_ptr<Services::CameraService> m_camera_service;
		unsigned int m_skybox_texture;
		
	};
}


#endif