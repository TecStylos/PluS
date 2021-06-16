#pragma once

#include <string>
#include <map>

#include "Plugin.h"

#include <Windows.h>

namespace PluS {
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
		std::string loadPlugin(const std::string& path)
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

			// Add the new plugin to the loaded plugins
			std::string name = pd.getInstance()->getName();
			m_plugins.insert(std::make_pair(name, pd));

			return name;
		}
		void unloadPlugin(const std::string& name)
		{
			// Find the plugin by name
			auto& it = m_plugins.find(name);
			if (it == m_plugins.end()) // Plugin not found
				return;

			// Get the instance
			auto& pd = it->second;

			// Shut the plugin down
			pd.onShutdown();

			// Release the library
			FreeLibrary(pd.hModule);

			// Remove the plugin from the loaded plugins
			m_plugins.erase(it);
		}
		PluginPtr getPlugin(const std::string& name)
		{
			// Find the plugin by name
			auto& it = m_plugins.find(name);
			if (it == m_plugins.end()) // Plugin not found
				return nullptr;

			return it->second.plugin; // Return the instance
		}
		std::vector<std::string> loadPluginDir(const std::string& path, bool recursive = false)
		{
			// TODO: Implement loading plugins from directory.

			std::vector<std::string> pluginNames;

			return pluginNames;
		}
	private:
		std::map<std::string, PluginData> m_plugins;
	};
} // namespace PluS