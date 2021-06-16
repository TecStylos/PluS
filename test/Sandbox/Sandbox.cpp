#include "PluS.h"
#include "MathFeature.h"

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

	std::string fStr;
	std::getline(std::cin, fStr);

	MathFeature* feature = (MathFeature*)plugin->createFeature(fStr);
	std::cout << "Created feature '" << feature->getName() << "'" << std::endl;

	std::cout << "  Result: " << feature->calc(12.0f, 23.0f) << std::endl;

	plugin->destroyFeature(feature);
	std::cout << "Destroyed feature '" << features[0] << "'" << std::endl;

	pm.unloadPlugin(pluginName);
	std::cout << "Unloaded plugin: " << pluginName << std::endl;
}