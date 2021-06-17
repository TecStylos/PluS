#include "PluS.h"
#include "MathFeature.h"

#include <iostream>

int main()
{
	PluS::PluginManager pm;
	PluS::PluginID pluginID = pm.loadPlugin("C:\\dev\\proj\\PluS\\out\\build\\x64-Debug\\test\\TestPlugin\\TestPlugin.dll");
	std::cout << "Loaded plugin!" << std::endl;

	auto plugin = pm.getPlugin(pluginID);

	std::string fStr;
	std::cout << ">>";
	std::getline(std::cin, fStr);

	PluS::UniqueID uid = pm.findMatchingFeatures(fStr)[0];
	auto feature = pm.createFeature<MathFeature>(uid);
	std::cout << "Created feature '" << feature->getName() << "'" << std::endl;

	std::cout << "  Result: " << feature->calc(12.0f, 23.0f) << std::endl;

	pm.destroyFeature(feature);
	std::cout << "Destroyed feature '" << fStr << "'" << std::endl;

	pm.unloadPlugin(pluginID);
}