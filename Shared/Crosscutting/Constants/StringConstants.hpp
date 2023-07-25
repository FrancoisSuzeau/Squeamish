/******************************************************************************************************************************************/
// File : StringConstants.hpp
// Purpose : string constant for the application
/******************************************************************************************************************************************/
#ifndef STRINGCONSTANTS_H
#define STRINGCONSTANTS_H

#include <iostream>


namespace Constants {

	std::string const SHADERSPATH = "Shaders/";
	std::string const LOGPATH = "Logs/";
	std::string const LOGEXT = ".txt";
	std::string const SHADERVERTEXT = ".vert";
	std::string const SHADERFRAGEXT = ".frag";
	std::string const SHADERGEOEXT = ".geom";
	std::string const NONE = "NONE";
	std::string const APP = "[APP ";
	std::string const CLIENT = "[CLIENT ";
	std::string const EXTSERVICE = "[EXT SERVICE ";
	std::string const LOGPATTERN = "%^[%T] %n: %v%$";
	std::string const TRACEFILE = "trace";
	std::string const INFOFILE = "info";
	std::string const DEBUGFILE = "debug";
	std::string const WARNFILE = "warn";
	std::string const ERRFILE = "err";
	std::string const CRITFILE = "crit";
	std::string const IMGUIVERSION = "#version 400";
	std::string const JSONEXT = ".json";
	std::string const CONFIGFILE = "configs";
	std::string const USERPREFNODE = "userpreferences";
}

#endif