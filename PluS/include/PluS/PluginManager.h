#pragma once

#include <string>
#include <map>
#include <filesystem>

#include "PluginHandle.h"
#include "PluginHandleCreator.h"
#include "PluginFeatureIterator.h"

namespace PluS {
	class PluginManager
	{
	private:
		PluginManager() = default;
		PluginManager(const PluginManager&) = delete;
		PluginManager(PluginManager&&) = delete;
		PluginManager& operator=(const PluginManager&) = delete;
	public:
		/*
		* Return the singleton instance of PluginManager.
		* 
		* @returns PluginManager instance.
		*/
		PLUS_API static PluginManager& get();
	public:
		/*
		* Load plugin from file.
		* 
		* @param path Path to the plugin file to load.
		* @returns Internal plugin ID. Can be used to retreive the underlying plugin or create features (part of UniqueID).
		*/
		PLUS_API PluginID loadPlugin(const std::string& path);
		/*
		* Unload loaded plugin.
		* 
		* @param pluginID Plugin ID of the plugin to unload. (Returned by loadPlugin)
		* @returns Number of initializations after the current call (See "PluginImplementation.h - Internal init function" for more details)
		*/
		PLUS_API uint64_t unloadPlugin(PluginID pluginID);
		/*
		* Get the underlying plugin.
		* 
		* @param pluginID Plugin ID of the plugin to retreive.
		* @returns Pointer to the underlying plugin.
		*/
		PLUS_API PluginPtr getPlugin(PluginID pluginID) const;
		/*
		* Get the number of references to a specific plugin.
		* 
		* @param pluginID Plugin ID of the plugin to get the ref count from.
		* @returns Number of references to the specified plugin.
		*/
		PLUS_API uint64_t getPluginRefCount(PluginID pluginID) const;
		/*
		* Get all features with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Iterator iterating over all registered features with the specified name.
		*/
		PLUS_API FeatureSearchIterator findFeatures(const std::string& name) const;
		/*
		* Get any feature with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Unique ID of a matching feature.
		*/
		PLUS_API UniqueID findFeature(const std::string& name) const;
		/*
		* Get the name of a specific feature.
		* 
		* @param uid Unique ID of the feature to get the name of.
		* @returns Name of the specified feature.
		*/
		PLUS_API const char* getFeatureName(UniqueID uid) const;
		/*
		* Load all plugins located in the specified path.
		* 
		* @param path Root directory to start searching.
		* @param extension The file extension of plugins to load.
		* @param recursive Specifies wether subdirectories should be crawled or not.
		* @returns Vector containing all newly loaded plugins.
		*/
		PLUS_API std::vector<PluginID> loadPluginDir(const std::string& path, const std::string& extension, bool recursive = false);
		/*
		* Create a new feature.
		* 
		* @param uid Unique ID of the feature to create.
		* @returns Pointer to newly created feature of type T. (T must derive from Feature)
		*/
		template <class CFeature>
		CFeature* createFeature(UniqueID uid);
		/*
		* Destroy a feature created with createFeature.
		* 
		* @param feature Pointer to feature to be destroyed.
		*/
		PLUS_API void destroyFeature(FeaturePtr feature);
	private:
		/*
		* Register new PluginData.
		* 
		* @param pd Plugin data to register.
		* @returns Plugin ID of the newly registered plugin data.
		*/
		PluginID registerPluginData(const _PluginData& pd);
		/*
		* Deregister PluginData registered with registerPluginData.
		* 
		* @param pid Plugin ID of the plugin to be deregistered.
		* @returns Copy of the plugin data getting deregistered.
		*/
		_PluginData deregisterPluginData(PluginID pid);
	private:
		PluginID m_nextPluginID = 1;
		_PluginDataMap m_plugins;
		static PluginManager s_singleton;
	};

	template <class CFeature>
	CFeature* PluginManager::createFeature(UniqueID uid)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must inherit from Feature!");

		FeaturePtr feature = getPlugin(uid.plugin)->createFeature(uid.feature);

		CFeature* derived = dynamic_cast<CFeature*>(feature);
		if (derived == nullptr)
			getPlugin(uid.plugin)->destroyFeature(feature);

		return derived;
	}
} // namespace PluS
