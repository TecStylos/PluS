#pragma once

#include <cstdint>
#include <string>
#include <map>

#include "Defines.h"

namespace PluS {
	#if defined PLUS_PLATFORM_WINDOWS
	typedef unsigned long ProcessID;
	#elif defined PLUS_PLATFORM_UNIX
	typedef int ProcessID;
	#endif

	typedef uint32_t PluginID;
	typedef uint32_t FeatureID;

	typedef uint32_t PayloadID;

	typedef std::map<std::string, FeatureID> _FeatureMap;

	union UniqueID
	{
		uint64_t full;
		struct
		{
			PluginID plugin;
			FeatureID feature;
		};

		operator bool()
		{
			return full != 0;
		}
	};

	PLUS_API UniqueID MakeUniqueID(PluginID pid, FeatureID fid);
} // namespace PluS