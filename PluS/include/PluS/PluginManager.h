#pragma once

#include <string>
#include <map>

#include "Plugin.h"

#include <Windows.h>

namespace PluS {
	union UniqueID
	{
		uint64_t full;
		struct
		{
			PluginID plugin;
			FeatureID feature;
		};
	};

	class PluginManager
	{
	private:
		struct PluginData
		{
			HMODULE hModule = nullptr;
			PluginPtr plugin = nullptr;
			PluginOnInitFunc onInit = nullptr;
			PluginGetInstanceFunc getInstance = nullptr;
			PluginOnShutdownFunc onShutdown = nullptr;
		};
	public:
		std::string loadPlugin(const std::string& path);
		void unloadPlugin(const std::string& name);
		void unloadPlugin(PluginID pluginID);
		PluginPtr getPlugin(const std::string& name);
		PluginPtr getPlugin(PluginID pluginID);
		PluginID getPluginID(const std::string& name);
		std::vector<UniqueID> findMatchingFeatures(const std::string& name);
		std::vector<std::string> loadPluginDir(const std::string& path, bool recursive = false);
	private:
		void registerPluginData(const PluginData& pd);
		PluginData deregisterPluginData(PluginID pid);
	private:
		PluginID m_nextPluginID = 1;
		std::map<std::string, PluginID> m_pluginIDMap;
		std::map<PluginID, PluginData> m_plugins;
	};

	std::string PluginManager::loadPlugin(const std::string& path)
	{
		PluginData pd;

		// Load the library file
		pd.hModule = LoadLibrary(path.c_str());
		if (!pd.hModule) return "ERROR: Unable to load plugin from file!"; // TODO: Error handling

		// Load the onInit function
		pd.onInit = (PluginOnInitFunc)GetProcAddress(pd.hModule, "PluSInternalInit");
		if (!pd.onInit) return "ERROR: PluginOnInit not found!";

		// Load the getInstance function
		pd.getInstance = (PluginGetInstanceFunc)GetProcAddress(pd.hModule, "PluSInternalGetInstance");
		if (!pd.getInstance) return "ERROR: PluginGetInstance not found!";

		// Load the onShutdown function
		pd.onShutdown = (PluginOnShutdownFunc)GetProcAddress(pd.hModule, "PluSInternalShutdown");
		if (!pd.onShutdown) return "ERROR: PluginOnShutdown not found!";

		// Initialize the plugin
		pd.onInit();

		// Get the plugin instance
		pd.plugin = pd.getInstance();

		registerPluginData(pd);

		return pd.getInstance()->getName();
	}
	void PluginManager::unloadPlugin(const std::string& name)
	{
		unloadPlugin(getPluginID(name));
	}
	void PluginManager::unloadPlugin(PluginID pluginID)
	{
		auto pd = deregisterPluginData(pluginID);

		// Shut the plugin down
		pd.onShutdown();

		// Release the library
		FreeLibrary(pd.hModule);
	}
	PluginPtr PluginManager::getPlugin(const std::string& name)
	{
		return getPlugin(getPluginID(name));
	}
	PluginPtr PluginManager::getPlugin(PluginID pluginID)
	{
		// Find the plugin by name
		auto& it = m_plugins.find(pluginID);
		if (it == m_plugins.end()) // Plugin not found
			return nullptr;

		return it->second.plugin; // Return the instance
	}
	PluginID PluginManager::getPluginID(const std::string& name)
	{
		auto& it = m_pluginIDMap.find(name);
		if (it == m_pluginIDMap.end())
			return 0;
		return it->second;
	}
	std::vector<UniqueID> PluginManager::findMatchingFeatures(const std::string& name)
	{
		// TODO: Implement findMatchingFeatures
		std::vector<UniqueID> matches;

		return matches;
	}
	std::vector<std::string> PluginManager::loadPluginDir(const std::string& path, bool recursive)
	{
		// TODO: Implement loading plugins from directory.

		std::vector<std::string> pluginNames;

		return pluginNames;
	}
	void PluginManager::registerPluginData(const PluginData& pd)
	{
		// Add the new plugin to the loaded plugins
		auto& name = pd.getInstance()->getName();
		PluginID pid = m_nextPluginID++;
		m_pluginIDMap.insert(std::make_pair(name, pid));
		m_plugins.insert(std::make_pair(pid, pd));
	}
	PluginManager::PluginData PluginManager::deregisterPluginData(PluginID pid)
	{
		auto& it = m_plugins.find(pid);
		if (it == m_plugins.end())
			return {};
		auto pd = it->second;

		m_plugins.erase(it);
		m_pluginIDMap.erase(pd.getInstance()->getName());

		return pd;
	}
} // namespace PluS