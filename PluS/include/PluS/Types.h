#pragma once

#include <string>
#include <map>

#include "Defines.h"

namespace PluS {
	typedef uint32_t PluginID;
	typedef uint32_t FeatureID;

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