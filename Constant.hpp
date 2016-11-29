#ifndef __CONSTANT_HPP__
#define __CONSTANT_HPP__

static constexpr int RequestJsonMax = 1024;
static constexpr int ResponseJsonMax = 128;

static const char* ContentTypeJson = "application/json";



static const char*	 	CONFIG_FILE_PATH		= "/light.json";
static const char*		NAME_PRIFIX				= "HiLight";
static constexpr int	NAME_PRIFIX_LEN			= 7;
static constexpr int 	DefaultStartupColor 	= 0x00ffffff;

#endif
