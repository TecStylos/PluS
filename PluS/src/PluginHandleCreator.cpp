#include "PluS/PluginHandleCreator.h"

namespace PluS
{
	PluginHandlePtr CreatePluginHandle(const std::string& path)
	{
		PluginHandlePtr handle;
		#if defined PLUS_PLATFORM_WINDOWS
		handle = std::make_shared<Win32PluginHandle>(path);
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
