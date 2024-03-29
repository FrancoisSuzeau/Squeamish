/******************************************************************************************************************************************/
// File : AudioInitializerService.cpp
// Purpose : Implementing Audio service  Initialisation
/******************************************************************************************************************************************/
#include "AudioInitializerService.hpp"

namespace Services
{
	void AudioInitializerService::Init()
	{
		init_succeded = true;
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
		{
			SQ_APP_ERROR("Class {} in function {} : SDL mixer FAILED to initialize - MIX error : {}", __FILE__, __FUNCTION__, Mix_GetError());
			init_succeded = false;

		}
		assert(init_succeded);
		SQ_APP_DEBUG("All audio services SUCCESSFULLY initialized");
	}

	void AudioInitializerService::DeInit()
	{
		Mix_CloseAudio();
		SQ_APP_DEBUG("All audio services shutdown");
	}
}

