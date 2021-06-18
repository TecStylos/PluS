#pragma once

#include "Plugin.h"

namespace PluS
{
	class Plugin : public _Plugin
	{
	public:
		Plugin() = delete;
		/*
		* Constructor of Plugin.
		* 
		* @param name Name of the plugin.
		* @param pid Plugin ID of the plugin.
		*/
		Plugin(const std::string& name, PluginID pid);
		~Plugin();
	public:
		virtual FeaturePtr createFeature(FeatureID fid) override;
		virtual void destroyFeature(FeaturePtr feature) override;
		virtual FeatureID getFeatureID(const std::string& name) const override;
		virtual FeatureIterator getFeatureIterator() const override;
		virtual const std::string& getName() const override;
		virtual PluginID getID() const override;
	protected:
		virtual FeatureCreator<Feature> getFeatureCreator(FeatureID fid) override;
	public:
		template <class CFeature>
		void registerFeatureCreator(FeatureCreator<CFeature> creator);
	private:
		void clearCreatedFeatures();
	private:
		PluginID m_pid;
		FeatureID m_nextFeatureID = 1;
		std::string m_name;
		std::map<FeatureID, FeatureCreator<Feature>> m_featureCreators;
		std::set<FeaturePtr> m_createdFeatures;
		std::map<std::string, FeatureID> m_features;
	};

	inline std::unique_ptr<Plugin> _pPlugin = nullptr;
	inline uint64_t _pluginRefCount = 0;

	/*
	* Get the global instance of the plugin.
	* 
	* @returns Global instance of the plugin.
	*/
	Plugin* getPlugin();

	// Everything in the 'PerPlugin' namespace must be defined by the plugin.
	namespace PerPlugin {
		/*
		* This variable stores the name of the plugin.
		*/
		extern const std::string pluginName;

		/*
		* This function gets called while initializing the plugin.
		*/
		extern void initPlugin();

		/*
		* This function gets called while shutting down the plugin.
		*/
		extern void shutdownPlugin();

	} // namespace PerPlugin

	Plugin::Plugin(const std::string& name, PluginID pid)
		: m_name(name), m_pid(pid)
	{}

	Plugin::~Plugin()
	{
		clearCreatedFeatures();
	}

	FeaturePtr Plugin::createFeature(FeatureID fid)
	{
		FeatureCreator<Feature> creator = getFeatureCreator(fid);
		if (!creator)
			return nullptr;

		FeaturePtr pFeature = creator(MakeUniqueID(getID(), fid));
		m_createdFeatures.insert(pFeature);
		return pFeature;
	}

	void Plugin::destroyFeature(FeaturePtr feature)
	{
		auto& it = m_createdFeatures.find(feature);
		if (it == m_createdFeatures.end())
			return;
		m_createdFeatures.erase(feature);
	}

	FeatureID Plugin::getFeatureID(const std::string& name) const
	{
		auto& it = m_features.find(name);
		if (it == m_features.end())
			return 0;
		return it->second;
	}

	FeatureIterator Plugin::getFeatureIterator() const
	{
		return FeatureIterator(
			m_features.begin(),
			m_features.end(),
			m_features.begin()
		);
	}

	const std::string& Plugin::getName() const
	{
		return m_name;
	}

	PluginID Plugin::getID() const
	{
		return m_pid;
	}

	FeatureIterator Plugin::begin() const
	{
		return FeatureIterator(m_features.begin(), m_features.end(), m_features.begin());
	}
	FeatureIterator Plugin::end() const
	{
		return FeatureIterator(m_features.begin(), m_features.end(), m_features.end());
	}

	FeatureCreator<Feature> Plugin::getFeatureCreator(FeatureID fid)
	{
		auto& it = m_featureCreators.find(fid);
		if (it == m_featureCreators.end())
			return nullptr;
		return it->second;
	}

	template <class CFeature>
	void Plugin::registerFeatureCreator(FeatureCreator<CFeature> creator)
	{
		// TODO: Store name
		FeatureID fid = m_nextFeatureID++;
		m_features.insert(std::make_pair(name, fid));
		m_featureCreators.insert(std::make_pair(fid, creator));
	}

	void Plugin::clearCreatedFeatures()
	{
		while (!m_createdFeatures.empty())
			destroyFeature(*m_createdFeatures.begin());
	}

	Plugin* getPlugin()
	{
		return _pPlugin.get();
	}

	extern "C" {
		#define PLUS_PLUGIN_EXPORT __declspec(dllexport)

		/*
		* This function gets called by the PluginManager for the plugin being loaded.
		* 
		* @param pid The plugin ID of the plugin
		* @returns True, if the plugin has already been initialized, otherwise false.
		*/
		PLUS_PLUGIN_EXPORT bool _PluSInternalInit(PluginID pid)
		{
			if (_pluginRefCount++)
				return true;

			_pPlugin.reset(new Plugin(PerPlugin::pluginName, pid));

			PerPlugin::initPlugin();

			return false;
		}

		/*
		* This function gets called by the PluginManager after the plugin has been initialized.
		*
		* @returns Pointer to a Plugin object providing information about the plugin.
		*/
		PLUS_PLUGIN_EXPORT _Plugin* _PluSInternalGetInstance()
		{
			return getPlugin();
		}

		/*
		* This function gets called by the PluginManager for the plugin being unloaded.
		*/
		PLUS_PLUGIN_EXPORT void _PluSInternalShutdown()
		{
			if (!_pluginRefCount)
				return;

			--_pluginRefCount;

			PerPlugin::shutdownPlugin();

			_pPlugin.reset();
		}

		#undef PLUS_PLUGIN_EXPORT
	} // extern "C"
} // namespace PluS