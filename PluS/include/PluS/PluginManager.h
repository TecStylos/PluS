#pragma once

#include <string>
#include <map>

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
		PluginID loadPlugin(const std::string& path);
		void unloadPlugin(PluginID pluginID);
		PluginPtr getPlugin(PluginID pluginID);
		std::vector<UniqueID> findMatchingFeatures(const std::string& name) const;
		std::vector<PluginID> loadPluginDir(const std::string& path, bool recursive = false);
		template <class T>
		T* createFeature(UniqueID uid);
		void destroyFeature(FeaturePtr feature);
	private:
		PluginID registerPluginData(const PluginData& pd);
		PluginData deregisterPluginData(PluginID pid);
	private:
		PluginID m_nextPluginID = 1;
		std::map<PluginID, PluginData> m_plugins;
	};

	PluginID PluginManager::loadPlugin(const std::string& path)
	{
		PluginData pd;

		// Load the library file
		pd.handle = CreatePluginHandle(path); // LoadLibrary(path.c_str())
		if (!pd.handle) return 0; // TODO: Error handling

		// Load the onInit function
		pd.onInit = pd.handle->get<_PluginOnInitFunc>("_PluSInternalInit");
		if (!pd.onInit) return 0;

		// Load the getInstance function
		pd.getInstance = pd.handle->get<_PluginGetInstanceFunc>("_PluSInternalGetInstance");
		if (!pd.getInstance) return 0;

		// Load the onShutdown function
		pd.onShutdown = pd.handle->get<_PluginOnShutdownFunc>("_PluSInternalShutdown"); // GetProcAddress(handle, name)
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
			for (auto& featureName : plugin->getFeatureList())
			{
				if (featureName == name)
					matches.push_back(MakeUniqueID(plugin->getID(), plugin->getFeatureID(featureName)));
			}
		}

		return matches;
	}
	std::vector<PluginID> PluginManager::loadPluginDir(const std::string& path, bool recursive)
	{
		// TODO: Implement loading plugins from directory.

		std::vector<PluginID> pluginNames;

		return pluginNames;
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