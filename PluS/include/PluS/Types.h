#pragma once

#include <string>
#include <map>

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
	};

	UniqueID MakeUniqueID(PluginID pid, FeatureID fid)
	{
		UniqueID uid;
		uid.plugin = pid;
		uid.feature = fid;
		return uid;
	}
} // namespace PluS