#pragma once

#include "PluginHandle.h"

#if defined PLUS_PLATFORM_WINDOWS
#include "Platform/Win32PluginHandle.h"
#elif defined PLUS_PLATFORM_UNIX
#include "Platform/UnixPluginHandle.h"
#endif

namespace PluS {
	/*
	* Create a new plugin handle.
	* 
	* @param path Path to the plugin to load.
	* @returns Pointer to the newly created plugin handle.
	*/
	PLUS_API PluginHandlePtr CreatePluginHandle(const std::string& path);
}
