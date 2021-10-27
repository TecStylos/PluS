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
	PluginHandlePtr CreatePluginHandle(const std::string& path)
	{
		PluginHandlePtr handle;
		#if defined PLUS_PLATFORM_WINDOWS
		handle =  std::make_shared<Win32PluginHandle>(path);
		#elif defined PLUS_PLATFORM_UNIX
		handle = std::make_shared<UnixPluginHandle>(path);
		#else
		#error Missing platform macro!
		#endif

		if (!(*handle))
			return nullptr;

		return handle;
	}
}
