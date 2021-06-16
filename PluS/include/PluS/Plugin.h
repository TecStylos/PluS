#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Feature.h"

namespace PluS {

	typedef Feature* FeaturePtr; // TODO: Custom Pointer

	class _Plugin
	{
	public:
		_Plugin() = default;
		virtual ~_Plugin() = default;
	public:
		virtual FeaturePtr createFeature(const std::string& name) = 0;
		virtual void destroyFeature(FeaturePtr feature) = 0;
		virtual FeatureCreator getFeatureCreator(const std::string& name) = 0;
		virtual std::vector<std::string> getFeatureList() const = 0;
		virtual const std::string& getName() const = 0;
	};

	typedef _Plugin* PluginPtr; // TODO: Custom Pointer

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
		~Plugin() = default;
	public:
		virtual FeaturePtr createFeature(const std::string& name) override
		{
			FeatureCreator creator = getFeatureCreator(name);
			if (creator)
				return creator();
			return nullptr;
		}
		virtual void destroyFeature(FeaturePtr feature) override
		{
			delete feature;
		}
		virtual FeatureCreator getFeatureCreator(const std::string& name) override
		{
			auto& it = m_featureCreators.find(name);
			if (it == m_featureCreators.end())
				return nullptr;
			return it->second;
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
	public:
		void registerFeatureCreator(const std::string& name, FeatureCreator creator)
		{
			m_featureCreators.insert(std::make_pair(name, creator));
		}
	private:
		std::string m_name;
		std::map<std::string, FeatureCreator> m_featureCreators;
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
		PLUGIN_EXPORT PluginPtr PluSInternalGetInstance()
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