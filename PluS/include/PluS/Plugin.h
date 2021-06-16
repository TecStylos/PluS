#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Feature.h"

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
typedef _Plugin*(__stdcall* PluginGetInstanceFunc)();
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
		// TODO: Implement getFeatureCreator
		auto& it = m_featureCreators.find(name);
		if (it == m_featureCreators.end())
			return nullptr;
		return it->second;
	}
	virtual std::vector<std::string> getFeatureList() const override
	{
		// TODO: Implemente getFeatureList
		std::vector<std::string> features;
		return features;
	}
	virtual const std::string& getName() const override
	{
		return m_name;
	}
private:
	std::string m_name;
	std::map<std::string, FeatureCreator> m_featureCreators;
};

inline std::unique_ptr<_Plugin> g_pPlugin = nullptr;

/*
* This function gets called while initializing the plugin.
* 
* @returns Subclass of Plugin implementing the required functionality.
*/
extern _Plugin* CreatePlugin();

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
extern "C" PLUGIN_EXPORT _Plugin* PluginGetInstance()
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