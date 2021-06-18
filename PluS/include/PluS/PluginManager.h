#pragma once

#include <string>
#include <map>
#include <filesystem>

#include "Plugin.h"
#include "PluginHandle.h"
#include "PluginHandleCreator.h"

namespace PluS {
	class PluginManager
	{
	private:
		struct PluginData
		{
			PluginHandlePtr handle;
			_PluginOnInitFunc onInit = nullptr;
			_PluginGetInstanceFunc getInstance = nullptr;
			_PluginOnShutdownFunc onShutdown = nullptr;
		};
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
		*/
		void unloadPlugin(PluginID pluginID);
		/*
		* Get the underlying plugin.
		* 
		* @param pluginID Plugin ID of the plugin to retreive.
		* @returns Pointer to the underlying plugin.
		*/
		PluginPtr getPlugin(PluginID pluginID);
		/*
		* Get all features with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Vector holding the unique IDs of all matching features.
		*/
		std::vector<UniqueID> findMatchingFeatures(const std::string& name) const;
		/*
		* Get any feature with the specified name.
		* 
		* @param name Feature name to search for.
		* @returns Unique ID of a matching feature.
		*/
		UniqueID findFeature(const std::string& name) const;
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
		template <class T>
		T* createFeature(UniqueID uid);
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
		PluginID registerPluginData(const PluginData& pd);
		/*
		* Deregister PluginData registered with registerPluginData.
		* 
		* @param pid Plugin ID of the plugin to be deregistered.
		* @returns Copy of the plugin data getting deregistered.
		*/
		PluginData deregisterPluginData(PluginID pid);
	private:
		PluginID m_nextPluginID = 1;
		std::map<PluginID, PluginData> m_plugins;
	};

	PluginID PluginManager::loadPlugin(const std::string& path)
	{
		PluginData pd;

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

	void PluginManager::unloadPlugin(PluginID pluginID)
	{
		auto pd = deregisterPluginData(pluginID);

		// Shut the plugin down
		pd.onShutdown();

		// FreeLibrary(pd.handle);
	}

	PluginPtr PluginManager::getPlugin(PluginID pluginID)
	{
		// Find the plugin by name
		auto& it = m_plugins.find(pluginID);
		if (it == m_plugins.end()) // Plugin not found
			return nullptr;

		return it->second.getInstance(); // Return the instance
	}

	std::vector<UniqueID> PluginManager::findMatchingFeatures(const std::string& name) const
	{
		// TODO: Implement findMatchingFeatures
		std::vector<UniqueID> matches;

		for (auto& pluginPair : m_plugins)
		{
			auto plugin = pluginPair.second.getInstance();
			auto fid = plugin->getFeatureID(name);
			if (fid)
				matches.push_back(MakeUniqueID(plugin->getID(), fid));
		}

		return matches;
	}

	UniqueID PluginManager::findFeature(const std::string& name) const
	{
		for (auto& pluginPair : m_plugins)
		{
			auto plugin = pluginPair.second.getInstance();
			auto fid = plugin->getFeatureID(name);
			if (fid)
				return MakeUniqueID(plugin->getID(), fid);
		}
		return { 0 };
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

	template <class T>
	T* PluginManager::createFeature(UniqueID uid)
	{
		static_assert(std::is_base_of<Feature, T>::value, "T must inherit from Feature!");

		FeaturePtr feature = getPlugin(uid.plugin)->createFeature(uid.feature);

		T* derived = dynamic_cast<T*>(feature);
		if (derived == nullptr)
			getPlugin(uid.plugin)->destroyFeature(feature);

		return derived;
	}

	void PluginManager::destroyFeature(FeaturePtr feature)
	{
		getPlugin(feature->getUniqueID().plugin)->destroyFeature(feature);
	}

	PluginID PluginManager::registerPluginData(const PluginData& pd)
	{
		// Add the new plugin to the loaded plugins
		PluginID pid = pd.getInstance()->getID();
		m_plugins.insert(std::make_pair(pid, pd));

		return pid;
	}

	PluginManager::PluginData PluginManager::deregisterPluginData(PluginID pid)
	{
		auto& it = m_plugins.find(pid);
		if (it == m_plugins.end())
			return {};
		auto pd = it->second;

		m_plugins.erase(it);

		return pd;
	}
} // namespace PluS