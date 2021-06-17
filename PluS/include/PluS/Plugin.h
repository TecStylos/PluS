#pragma once

#include <vector>
#include <map>
#include <set>

#include "Feature.h"

namespace PluS
{
	class _Plugin
	{
	public:
		_Plugin() = default;
		virtual ~_Plugin() = default;
	public:
		virtual FeaturePtr createFeature(FeatureID fid) = 0;
		virtual void destroyFeature(FeaturePtr feature) = 0;
		virtual std::vector<std::string> getFeatureList() const = 0;
		virtual FeatureID getFeatureID(const std::string& name) const = 0;
		virtual const std::string& getName() const = 0;
		virtual PluginID getID() const = 0;
	protected:
		virtual FeatureCreator getFeatureCreator(FeatureID fid) = 0;
	};

	typedef _Plugin* PluginPtr;

	typedef bool(__stdcall* _PluginOnInitFunc)(PluginID);
	typedef _Plugin* (__stdcall* _PluginGetInstanceFunc)();
	typedef void(__stdcall* _PluginOnShutdownFunc)();

} // namespace PluS