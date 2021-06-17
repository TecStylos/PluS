#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>

#include "Feature.h"

namespace PluS {
	class _Plugin
	{
	public:
		_Plugin() = default;
		virtual ~_Plugin() = default;
	public:
		virtual FeaturePtr createFeature(const std::string& name) = 0;
		virtual void destroyFeature(FeaturePtr feature) = 0;
		virtual std::vector<std::string> getFeatureList() const = 0;
		virtual const std::string& getName() const = 0;
	protected:
		virtual FeatureCreator getFeatureCreator(const std::string& name) = 0;
	};

	typedef uint32_t PluginID;
	typedef uint32_t FeatureID;

	typedef _Plugin* PluginPtr;

	typedef void(__stdcall* PluginOnInitFunc)();
	typedef _Plugin* (__stdcall* PluginGetInstanceFunc)();
	typedef void(__stdcall* PluginOnShutdownFunc)();

	#ifdef BUILD_PLUGIN

	#define PLUGIN_EXPORT __declspec(dllexport)

	class Plugin : public _Plugin
	{
	public:
		Plugin() = delete;
		Plugin(const std::string& name)
			: m_name(name)
		{}
		~Plugin()
		{
			clearCreatedFeatures();
		}
	public:
		virtual FeaturePtr createFeature(const std::string& name) override
		{
			FeatureCreator creator = getFeatureCreator(name);
			if (!creator)
				return nullptr;

			FeaturePtr pFeature = creator();
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
			for (auto& f : m_featureCreators)
				features.push_back(f.first);
			return features;
		}
		virtual const std::string& getName() const override
		{
			return m_name;
		}
	protected:
		virtual FeatureCreator getFeatureCreator(const std::string& name) override
		{
			auto& it = m_featureCreators.find(name);
			if (it == m_featureCreators.end())
				return nullptr;
			return it->second;
		}
	public:
		void registerFeatureCreator(const std::string& name, FeatureCreator creator)
		{
			m_featureCreators.insert(std::make_pair(name, creator));
		}
	private:
		void clearCreatedFeatures()
		{
			while (!m_createdFeatures.empty())
				destroyFeature(*m_createdFeatures.begin());
		}
	private:
		std::string m_name;
		std::map<std::string, FeatureCreator> m_featureCreators;
		std::set<FeaturePtr> m_createdFeatures;
	};

	inline std::unique_ptr<Plugin> g_pPlugin;

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
		/*
		* This function gets called by the PluginManager for the plugin being loaded.
		*/
		PLUGIN_EXPORT void PluSInternalInit()
		{
			g_pPlugin.reset(new Plugin(PerPlugin::pluginName));

			PerPlugin::initPlugin();
		}

		/*
		* This function gets called by the PluginManager after the plugin has been initialized.
		*
		*	 @returns Pointer to a Plugin object providing information about the plugin.
		*/
		PLUGIN_EXPORT _Plugin* PluSInternalGetInstance()
		{
			return getPlugin();
		}

		/*
		* This function gets called by the PluginManager for the plugin being unloaded.
		*/
		PLUGIN_EXPORT void PluSInternalShutdown()
		{
			PerPlugin::shutdownPlugin();

			g_pPlugin.reset();
		}
	} // extern "C"

	#endif // BUILD_PLUGIN

} // namespace PluS