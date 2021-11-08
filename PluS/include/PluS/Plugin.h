#pragma once

#include <vector>
#include <map>
#include <set>

#include "PluginHandle.h"
#include "FeatureIterator.h"

#ifdef PLUS_PLATFORM_UNIX
#define __cdecl __attribute((__cdecl__))
#endif

#if defined PLUS_PLATFORM_WINDOWS
#define PLUS_PLATFORM_PLUGIN_EXTENSION ".dll"
#elif defined PLUS_PLATFORM_UNIX
#define PLUS_PLATFORM_PLUGIN_EXTENSION ".so"
#else
#error Missing platform macro!
#endif

namespace PluS
{
  const char* PluginExtension = PLUS_PLATFORM_PLUGIN_EXTENSION;

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
		* Create a new instance of a feature.
		* 
		* @param fid Feature ID of the instance to create.
		* @returns Pointer to newly created instace.
		*/
		virtual FeaturePtr createFeature(FeatureID fid) = 0;
		/*
		* Destroy a feature instance created with createFeature.
		* 
		* @param feature Pointer to feature instance to be destroyed.
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
		* Get the name of the specified feature.
		* 
		* @param fid Feature ID of the feature to get the name of.
		* @returns Name of the specified feature.
		*/
		virtual const char* getFeatureName(FeatureID fid) const = 0;
		/*
		* Get an iterator iterating over all names of the registered features.
		*
		* @returns Iterator iterating over all names of the registered features.
		*/
		virtual FeatureIterator getFeatureIterator() const = 0;
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
	};

	typedef _Plugin* PluginPtr;

	typedef uint64_t(__cdecl *_PluginGetRefCountFunc)();
	typedef uint64_t(__cdecl *_PluginOnInitFunc)(PluginID);
	typedef _Plugin* (__cdecl *_PluginGetInstanceFunc)();
	typedef uint64_t(__cdecl *_PluginOnShutdownFunc)();

	struct _PluginData
	{
		PluginHandlePtr handle = NULL;
		_PluginGetRefCountFunc getRefCount = nullptr;
		_PluginOnInitFunc onInit = nullptr;
		_PluginGetInstanceFunc getInstance = nullptr;
		_PluginOnShutdownFunc onShutdown = nullptr;
	};

	typedef std::map<PluginID, _PluginData> _PluginDataMap;

} // namespace PluS
