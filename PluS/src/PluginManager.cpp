#include "PluS/PluginManager.h"

namespace PluS
{
	PluginManager PluginManager::s_singleton = PluginManager();

	PluginManager& PluginManager::get()
	{
		return s_singleton;
	}

	PluginID PluginManager::loadPlugin(const std::string& path)
	{
		_PluginData pd;

		// Load the library file
		pd.handle = CreatePluginHandle(path);
		if (!pd.handle) return 0;

		// Load the onInit function
		pd.onInit = pd.handle->get<_PluginOnInitFunc>("_PluSInternalInit");
		if (!pd.onInit) return 0;

		// Load the getInstance function
		pd.getInstance = pd.handle->get<_PluginGetInstanceFunc>("_PluSInternalGetInstance");
		if (!pd.getInstance) return 0;

		pd.getRefCount = pd.handle->get<_PluginGetRefCountFunc>("_PluSInternalGetRefCount");
		if (!pd.getRefCount) return 0;

		// Load the onShutdown function
		pd.onShutdown = pd.handle->get<_PluginOnShutdownFunc>("_PluSInternalShutdown");
		if (!pd.onShutdown) return 0;

		if (pd.onInit(m_nextPluginID++))
			return 0;

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
		auto it = m_plugins.find(pluginID);
		if (it == m_plugins.end()) // Plugin not found
			return nullptr;

		return it->second.getInstance(); // Return the instance
	}

	uint64_t PluginManager::getPluginRefCount(PluginID pluginID) const
	{
		auto it = m_plugins.find(pluginID);
		if (it == m_plugins.end()) // Plugin not found
			return 0;

		return it->second.getRefCount(); // Return the number of references
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
		std::vector<PluginID> pluginIDs;

		auto extensionPath = std::filesystem::directory_entry(extension);

		for (auto it = std::filesystem::recursive_directory_iterator(path);
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
		auto it = m_plugins.find(pid);
		if (it == m_plugins.end())
			return {};
		auto pd = it->second;

		m_plugins.erase(it);

		return pd;
	}
}