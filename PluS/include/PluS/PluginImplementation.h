#pragma once

#include "Plugin.h"

namespace PluS
{
	class Plugin : public _Plugin
	{
	public:
		Plugin() = delete;
		Plugin(const std::string& name, PluginID pid)
			: m_name(name), m_pid(pid)
		{}
		~Plugin()
		{
			clearCreatedFeatures();
		}
	public:
		virtual FeaturePtr createFeature(FeatureID fid) override
		{
			FeatureCreator creator = getFeatureCreator(fid);
			if (!creator)
				return nullptr;

			FeaturePtr pFeature = creator(MakeUniqueID(getID(), fid), false);
			m_createdFeatures.insert(pFeature);
			return pFeature;
		}
		virtual void destroyFeature(FeaturePtr feature) override
		{
			auto& it = m_createdFeatures.find(feature);
			if (it == m_createdFeatures.end())
				return;
			m_createdFeatures.erase(feature);
		}
		virtual std::vector<std::string> getFeatureList() const override
		{
			std::vector<std::string> features;
			features.reserve(m_features.size());
			for (auto& it : m_features)
				features.push_back(it.first);
			return features;
		}
		virtual FeatureID getFeatureID(const std::string& name) const override
		{
			auto& it = m_features.find(name);
			if (it == m_features.end())
				return 0;
			return it->second;
		}
		virtual const std::string& getName() const override
		{
			return m_name;
		}
		virtual PluginID getID() const override
		{
			return m_pid;
		}
	protected:
		virtual FeatureCreator getFeatureCreator(FeatureID fid) override
		{
			auto& it = m_featureCreators.find(fid);
			if (it == m_featureCreators.end())
				return nullptr;
			return it->second;
		}
	public:
		void registerFeatureCreator(FeatureCreator creator)
		{
			// TODO: Store name
			FeatureID fid = m_nextFeatureID++;
			std::string name;
			{
				FeaturePtr feature = creator({ 0 }, true);
				name = feature->getName();
				delete feature;
			}
			m_features.insert(std::make_pair(name, fid));
			m_featureCreators.insert(std::make_pair(fid, creator));
		}
	private:
		void clearCreatedFeatures()
		{
			while (!m_createdFeatures.empty())
				destroyFeature(*m_createdFeatures.begin());
		}
	private:
		PluginID m_pid;
		FeatureID m_nextFeatureID = 1;
		std::string m_name;
		std::map<FeatureID, FeatureCreator> m_featureCreators;
		std::set<FeaturePtr> m_createdFeatures;
		std::map<std::string, FeatureID> m_features;
	};

	inline std::unique_ptr<Plugin> g_pPlugin = nullptr;

	Plugin* getPlugin()
	{
		return g_pPlugin.get();
	}

	// Everything in the 'PerPlugin' namespace must be defined by the plugin.
	namespace PerPlugin {
		/*
		* This variable stores the name of the plugin.
		*/
		extern const std::string pluginName;

		extern void initPlugin();

		extern void shutdownPlugin();

	} // namespace PerPlugin

	extern "C" {
		#define PLUS_PLUGIN_EXPORT __declspec(dllexport)

		/*
		* This function gets called by the PluginManager for the plugin being loaded.
		*/
		PLUS_PLUGIN_EXPORT bool _PluSInternalInit(PluginID pid)
		{
			if (g_pPlugin)
				return true;

			g_pPlugin.reset(new Plugin(PerPlugin::pluginName, pid));

			PerPlugin::initPlugin();

			return false;
		}

		/*
		* This function gets called by the PluginManager after the plugin has been initialized.
		*
		*	 @returns Pointer to a Plugin object providing information about the plugin.
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
			if (!g_pPlugin)
				return;

			PerPlugin::shutdownPlugin();

			g_pPlugin.reset();
		}

		#undef PLUS_PLUGIN_EXPORT
	} // extern "C"
} // namespace PluS