#pragma once

#include "Plugin.h"

#include "FeatureFactory.h"

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
		PLUS_API Plugin(const std::string& name, PluginID pid);
		PLUS_API ~Plugin();
	public:
		virtual FeaturePtr createFeature(FeatureID fid) override;
		virtual void destroyFeature(FeaturePtr feature) override;
		virtual FeatureID getFeatureID(const std::string& name) const override;
		virtual const char* getFeatureName(FeatureID fid) const override;
		virtual FeatureIterator getFeatureIterator() const override;
		virtual const std::string& getName() const override;
		virtual PluginID getID() const override;
	protected:
		FeatureCreator getFeatureCreator(FeatureID fid);
	public:
		PLUS_API void registerFeatureFactory(FeatureFactory factory);
	private:
		void clearCreatedFeatures();
	private:
		std::string m_name;
		PluginID m_pid;
		FeatureID m_nextFeatureID = 1;
		std::map<FeatureID, FeatureFactory> m_featureCreators;
		std::set<FeaturePtr> m_createdFeatures;
		_FeatureMap m_features;
	};

	// Everything in the 'PerPlugin' namespace must be defined by the plugin.
	namespace PerPlugin {
		/*
		* This function gets called while initializing the plugin.
		*/
		extern void initPlugin();

		/*
		* This function gets called while shutting down the plugin.
		*/
		extern void shutdownPlugin();

		/*
		* This variable stores the name of the plugin.
		*/
		extern const std::string pluginName;

		extern std::unique_ptr<Plugin> pPlugin;
		extern uint64_t pluginRefCount;

#define PLUS_PERPLUGIN_DEFINE_EXTERNALS(PLUGIN_NAME) \
const std::string PluS::PerPlugin::pluginName = PLUGIN_NAME;\
\
std::unique_ptr<PluS::Plugin> PluS::PerPlugin::pPlugin = nullptr;\
uint64_t PluS::PerPlugin::pluginRefCount = 0;

	} // namespace PerPlugin

	#if defined PLUS_BUILD_PLUGIN
	extern "C" {
#if defined PLUS_PLATFORM_WINDOWS
#define PLUS_PLUGIN_EXPORT __declspec(dllexport)
#elif defined PLUS_PLATFORM_UNIX
#define PLUS_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif
		/*
		* This function returns the number of references on this plugin.
		*
		* @returns Number of references.
		*/
		PLUS_PLUGIN_EXPORT uint64_t _PluSInternalGetRefCount()
		{
			return PerPlugin::pluginRefCount;
		}

		/*
		* This function gets called by the PluginManager for the plugin being loaded.
		* 
		* @param pid The plugin ID of the plugin
		* @returns Number of initializations before the current call. (Zero if it has not been initialized yet, otherwise non-zero)
		*/
		PLUS_PLUGIN_EXPORT uint64_t _PluSInternalInit(PluginID pid)
		{
			uint64_t prevCount = _PluSInternalGetRefCount();

			if (!PerPlugin::pluginRefCount++)
			{
				PerPlugin::pPlugin.reset(new Plugin(PerPlugin::pluginName, pid));
				PerPlugin::initPlugin();
			}

			return prevCount;
		}

		/*
		* This function gets called by the PluginManager after the plugin has been initialized.
		*
		* @returns Pointer to a Plugin object providing information about the plugin.
		*/
		PLUS_PLUGIN_EXPORT _Plugin* _PluSInternalGetInstance()
		{
			return PerPlugin::pPlugin.get();
		}

		/*
		* This function gets called by the PluginManager for the plugin being unloaded.
		* 
		* @returns Number initializations after the current call. (Zero if it it now uninitialized, otherwise non-zero)
		*/
		PLUS_PLUGIN_EXPORT uint64_t _PluSInternalShutdown()
		{
			if (!PerPlugin::pluginRefCount)
				return 0;

			--PerPlugin::pluginRefCount;

			PerPlugin::shutdownPlugin();

			PerPlugin::pPlugin.reset();

			return _PluSInternalGetRefCount();
		}

		#undef PLUS_PLUGIN_EXPORT
	} // extern "C"
	#endif // PLUS_BUILD_PLUGIN
} // namespace PluS
