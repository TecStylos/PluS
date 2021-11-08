#include "PluS/Types.h"

namespace PluS
{
	UniqueID MakeUniqueID(PluginID pid, FeatureID fid)
	{
		UniqueID uid;
		uid.plugin = pid;
		uid.feature = fid;
		return uid;
	}
}