#pragma once

namespace PluS {
	typedef uint32_t PluginID;
	typedef uint32_t FeatureID;

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