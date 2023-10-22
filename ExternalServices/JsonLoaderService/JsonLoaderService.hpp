/******************************************************************************************************************************************/
// File : JsonLoaderService.hpp
// Purpose : A service loading json files
/******************************************************************************************************************************************/
#ifndef JSONLOADERSERVICE_H
#define JSONLOADERSERVICE_H

#include <fstream>
#include <cassert>
#include "Constants/StringConstants.hpp"
#include "Logger.hpp"

#include <nlohmann/json.hpp>

#include "IService.hpp"

namespace Services {

	class JsonLoaderService : public IService
	{
	public:
		void Init() override;
		void DeInit() override;
		void SaveFile(std::string const filename);
	private:
		std::unique_ptr<nlohmann::json> datas;
		void ReadFile(std::string filename);
		std::string GetStringNode(std::string node_name);
	};
}

#endif