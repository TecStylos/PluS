#include "PluS.h"

#include <iostream>

int main()
{
	PluS::PluginManager pm;
	std::string pluginName = pm.loadPlugin("C:\\dev\\proj\\PluS\\out\\build\\x64-Debug\\test\\TestPlugin\\TestPlugin.dll");
	std::cout << "Loaded plugin: " << pluginName << std::endl;

	PluS::PluginPtr plugin = pm.getPlugin(pluginName);

	auto features = plugin->getFeatureList();
	std::cout << "Found features:" << std::endl;
	for (auto& f : features)
		std::cout << "  " << f << std::endl;

	PluS::FeaturePtr feature = plugin->createFeature(features[0]);
	std::cout << "Created feature '" << features[0] << "'" << std::endl;

	std::cout << "feature->getName() = " << feature->getName() << std::endl;

	plugin->destroyFeature(feature);
	std::cout << "Destroyed feature '" << features[0] << "'" << std::endl;

	pm.unloadPlugin(pluginName);
	std::cout << "Unloaded plugin: " << pluginName << std::endl;
}