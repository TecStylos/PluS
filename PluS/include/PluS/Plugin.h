#pragma once

#include <vector>
#include <map>
#include <set>

#include "Feature.h"
#include "FeatureIterator.h"

namespace PluS
{
	class _Plugin
	{
	public:
		/*
		* Constructor of _Plugin.
		*/
		_Plugin() = default;
		/*
		* Destructor of _Plugin.
		*/
		virtual ~_Plugin() = default;
	public:
		/*
		* Create a new feature.
		* 
		* @param fid Feature ID of the feature to create.
		* @returns Pointer to newly created feature.
		*/
		virtual FeaturePtr createFeature(FeatureID fid) = 0;
		/*
		* Destroy a feature created with createFeature.
		* 
		* @param feature Pointer to feature to be destroyed.
		*/
		virtual void destroyFeature(FeaturePtr feature) = 0;
		/*
		* Get the feature ID associated with the specified name.
		* 
		* @param name Name of the feature.
		* @returns Feature ID associated with the specified name.
		*/
		virtual FeatureID getFeatureID(const std::string& name) const = 0;
		/*
		* Get the name of the plugin.
		* 
		* @returns Name of the plugin.
		*/
		virtual const std::string& getName() const = 0;
		/*
		* Get the plugin ID of the plugin.
		* 
		* @returns Plugin ID of the plugin.
		*/
		virtual PluginID getID() const = 0;
	public:
		/*
		* Get an iterator iterating over all registered features.
		* 
		* @returns Iterator iterating over all registered features.
		*/
		virtual FeatureIterator begin() const = 0;
		/*
		* Get an iterator indicating end of all registered features.
		* 
		* @returns Iterator indicating end of all registered features.
		*/
		virtual FeatureIterator end() const = 0;
	protected:
		/*
		* Get the feature creator associated with the specified feature ID.
		* 
		* @param fid Feature ID to get the feature creator for.
		* @returns Feature creator associated with the specified feature ID.
		*/
		virtual FeatureCreator getFeatureCreator(FeatureID fid) = 0;
	};

	typedef _Plugin* PluginPtr;

	typedef bool(__stdcall* _PluginOnInitFunc)(PluginID);
	typedef _Plugin* (__stdcall* _PluginGetInstanceFunc)();
	typedef void(__stdcall* _PluginOnShutdownFunc)();

} // namespace PluS