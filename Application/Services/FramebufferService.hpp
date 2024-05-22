/******************************************************************************************************************************************/
// File : FramebufferService.hpp
// Purpose : A service for communicate between loader and shader
/******************************************************************************************************************************************/
#ifndef FRAMEBUFFERSERVICE_H
#define FRAMEBUFFERSERVICE_H

#include "IService.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include <iostream>
#include "StateService.hpp"
#include "RunTimeService.hpp"

namespace Services {


	class FramebufferService : public IService
	{

	public:
		void Init() override;
		void DeInit() override;

		unsigned int GetTextureId(int id) const;
		unsigned int GetTextureId() const;
		unsigned int GetTextureId(bool horizontal) const;
		unsigned int GetDephtTextureId() const;

		void ClearFramebuffer();

		
		void BuildNormalFrameBuffer();
		void BuildMultiSampleFrameBuffer();

		void BindFramebuffer(Enums::FramebufferType type);
		void BlitFramebuffer();
		void BindFramebuffer(bool horizontal);
		void UnbindFramebuffer();

	private:

		void SetFrameBufferDim();

		void BuildScreenTexture(Enums::FramebufferType type);
		void BuildMultisampleColorTexture();
		void BuildPingPongTexture();
		void BuildDepthMapTexture();

		void BuildNormalRenderFB();
		void BuildMultiSampleRenderFB();

		void CheckFramebufferStatus(std::string const framebuffer_name);

		unsigned int m_color_texture_ids[2] = {0, 0};
		unsigned int m_ping_pong_textures_ids[2] = { 0, 0 };
		unsigned int m_depth_map_texture_id;
		unsigned int m_color_multisample_texture;

		unsigned int m_color_texture_id;

		unsigned int m_color_fb;
		unsigned int m_color_multisample_fb;
		unsigned int m_render_fb;
		unsigned int m_ping_pong_fb[2] = { 0, 0 };
		unsigned int m_depth_map_fb;

		

		int m_fb_width;
		int m_fb_height;

		std::shared_ptr<Services::StateService> m_state_service;
		std::shared_ptr<Services::RunTimeService> m_runtime_service;

	};
}

#endif