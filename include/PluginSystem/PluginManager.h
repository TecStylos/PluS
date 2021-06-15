#pragma once

#include <string>
#include <map>

#include "Plugin.h"

#include <Windows.h>

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
		pd.hModule = LoadLibrary(path.c_str());

		if (!pd.hModule) return "ERROR"; // TODO: Error handling
		pd.onInit = (PluginOnInitFunc)GetProcAddress(pd.hModule, "PluginOnInit");
		if (!pd.onInit) return "ERROR: PluginOnInit not found!";

		pd.getInstance = (PluginGetInstanceFunc)GetProcAddress(pd.hModule, "PluginGetInstance");
		if (!pd.getInstance) return "ERROR: PluginGetInstance not found!";

		pd.onShutdown = (PluginOnShutdownFunc)GetProcAddress(pd.hModule, "PluginOnShutdown");
		if (!pd.onShutdown) return "ERROR: PluginOnShutdown not found!";

		pd.onInit();
		pd.plugin = pd.getInstance();

		std::string name = pd.getInstance()->getName();
		m_plugins.insert(std::make_pair(name, pd));
		return name;
	}
	PluginPtr getPlugin(const std::string& name)
	{
		auto& it = m_plugins.find(name);
		if (it == m_plugins.end())
			return nullptr;

		return it->second.plugin;
	}
	void unloadPlugin(const std::string& name)
	{
		auto& it = m_plugins.find(name);
		if (it == m_plugins.end())
			return;

		auto& pd = it->second;
		pd.onShutdown();

		FreeLibrary(pd.hModule);

		m_plugins.erase(it);
	}
private:
	std::map<std::string, PluginData> m_plugins;
};