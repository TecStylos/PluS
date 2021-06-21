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
		static PluginManager& get();
	public:
		/*
		* Load plugin from file.
		* 
		* @param path Path to the plugin file to load.
		* @returns Internal plugin ID. Can be used to retreive the underlying plugin or create features (part of UniqueID).
		*/
		PluginID loadPlugin(const std::string& path);
		/*
		* Unload loaded plugin.
		* 
		* @param pluginID Plugin ID of the plugin to unload. (Returned by loadPlugin)
		* @returns Number of initializations after the current call (See "PluginImplementation.h - Internal init function" for more details)
		*/
		uint64_t unloadPlugin(PluginID pluginID);
		/*
		* Get the underlying plugin.
		* 
		* @param pluginID Plugin ID of the plugin to retreive.
		* @returns Pointer to the underlying plugin.
		*/
		PluginPtr getPlugin(PluginID pluginID) const;
		/*
		* Get all features with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Iterator iterating over all registered features with the specified name.
		*/
		FeatureSearchIterator findFeatures(const std::string& name) const;
		/*
		* Get any feature with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Unique ID of a matching feature.
		*/
		UniqueID findFeature(const std::string& name) const;
		/*
		* Get the name of a specific feature.
		* 
		* @param uid Unique ID of the feature to get the name of.
		* @returns Name of the specified feature.
		*/
		const char* getFeatureName(UniqueID uid) const;
		/*
		* Load all plugins located in the specified path.
		* 
		* @param path Root directory to start searching.
		* @param extension The file extension of plugins to load.
		* @param recursive Specifies wether subdirectories should be crawled or not.
		* @returns Vector containing all newly loaded plugins.
		*/
		std::vector<PluginID> loadPluginDir(const std::string& path, const std::string& extension, bool recursive = false);
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
		void destroyFeature(FeaturePtr feature);
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

	inline PluginManager PluginManager::s_singleton = PluginManager();

	PluginManager& PluginManager::get()
	{
		return s_singleton;
	}

	PluginID PluginManager::loadPlugin(const std::string& path)
	{
		_PluginData pd;

		// Load the library file
		pd.handle = CreatePluginHandle(path);
		if (!pd.handle) return 0; // TODO: Error handling

		// Load the onInit function
		pd.onInit = pd.handle->get<_PluginOnInitFunc>("_PluSInternalInit");
		if (!pd.onInit) return 0;

		// Load the getInstance function
		pd.getInstance = pd.handle->get<_PluginGetInstanceFunc>("_PluSInternalGetInstance");
		if (!pd.getInstance) return 0;

		// Load the onShutdown function
		pd.onShutdown = pd.handle->get<_PluginOnShutdownFunc>("_PluSInternalShutdown");
		if (!pd.onShutdown) return 0;

		if (pd.onInit(m_nextPluginID))
			return 0;

		++m_nextPluginID;

		return registerPluginData(pd);
	}

	uint64_t PluginManager::unloadPlugin(PluginID pluginID)
	{
		auto pd = deregisterPluginData(pluginID);

		// Shut the plugin down
		return pd.onShutdown();
	}

	PluginPtr PluginManager::getPlugin(PluginID pluginID) const
	{
		// Find the plugin by name
		auto& it = m_plugins.find(pluginID);
		if (it == m_plugins.end()) // Plugin not found
			return nullptr;

		return it->second.getInstance(); // Return the instance
	}

	FeatureSearchIterator PluginManager::findFeatures(const std::string& name) const
	{
		return FeatureSearchIterator(m_plugins, m_plugins.begin(), name);
	}

	UniqueID PluginManager::findFeature(const std::string& name) const
	{
		auto it = findFeatures(name);
		if (it != it.end())
			return *it;
		return { 0 };
	}

	const char* PluginManager::getFeatureName(UniqueID uid) const
	{
		return getPlugin(uid.plugin)->getFeatureName(uid.feature);
	}

	std::vector<PluginID> PluginManager::loadPluginDir(const std::string& path, const std::string& extension, bool recursive)
	{
		// TODO: Implement loading plugins from directory.

		std::vector<PluginID> pluginIDs;

		auto extensionPath = std::filesystem::directory_entry(extension);

		for (auto& it = std::filesystem::recursive_directory_iterator(path);
			it != std::filesystem::recursive_directory_iterator();
			++it)
		{
			if (it->is_directory() &&
				!recursive)
			{
				it.disable_recursion_pending();
				continue;
			}

			if (it->is_regular_file() &&
				it->path().has_extension() &&
				it->path().extension() == extensionPath
				)
			{
				auto pid = loadPlugin(it->path().string());
				if (pid)
					pluginIDs.push_back(pid);
			}
		}

		return pluginIDs;
	}

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

	void PluginManager::destroyFeature(FeaturePtr feature)
	{
		getPlugin(feature->getUniqueID().plugin)->destroyFeature(feature);
	}

	PluginID PluginManager::registerPluginData(const _PluginData& pd)
	{
		// Add the new plugin to the loaded plugins
		PluginID pid = pd.getInstance()->getID();
		m_plugins.insert(std::make_pair(pid, pd));

		return pid;
	}

	_PluginData PluginManager::deregisterPluginData(PluginID pid)
	{
		auto& it = m_plugins.find(pid);
		if (it == m_plugins.end())
			return {};
		auto pd = it->second;

		m_plugins.erase(it);

		return pd;
	}
} // namespace PluS