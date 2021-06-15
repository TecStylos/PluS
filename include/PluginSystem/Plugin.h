#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Feature.h"

typedef Feature* FeaturePtr; // TODO: Custom Pointer

class Plugin
{
public:
	Plugin() = default;
	virtual ~Plugin() = default;
public:
	virtual std::vector<std::string> getFeatures() const = 0;
	virtual FeaturePtr createFeature(const std::string& name) = 0;
	virtual void destroyFeature(FeaturePtr feature) = 0;
	virtual const char* getName() const = 0;
};

typedef Plugin* PluginPtr; // TODO: Custom Pointer

typedef void(__stdcall* PluginOnInitFunc)();
typedef Plugin*(__stdcall* PluginGetInstanceFunc)();
typedef void(__stdcall* PluginOnShutdownFunc)();

#ifdef BUILD_PLUGIN

#define PLUGIN_EXPORT __declspec(dllexport)

inline std::unique_ptr<Plugin> g_pPlugin = nullptr;

/*
* This function gets called while initializing the plugin.
* 
* @returns Subclass of Plugin implementing the required functionality.
*/
extern Plugin* CreatePlugin();

/*
* This function gets called by the PluginManager for the plugin being loaded.
*/
extern "C" PLUGIN_EXPORT void PluginOnInit()
{
	g_pPlugin.reset(CreatePlugin());
}

/*
* This function gets called by the PluginManager after the plugin has been initialized.
* 
* @returns Pointer to a Plugin object providing information about the plugin.
*/
extern "C" PLUGIN_EXPORT Plugin* PluginGetInstance()
{
	return g_pPlugin.get();
}

/*
* This function gets called by the PluginManager for the plugin being unloaded.
*/
extern "C" PLUGIN_EXPORT void PluginOnShutdown()
{
	g_pPlugin.reset();
}

#endif // BUILD_PLUGIN