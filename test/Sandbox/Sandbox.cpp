#define PLUS_PLATFORM_WINDOWS
#include "PluS.h"
#include "MathFeature.h"

#include <iostream>

int main()
{
	PluS::PluginManager pm;
	auto ids = pm.loadPluginDir("../TestPlugin/", ".dll", true);
	std::cout << "Loaded plugins:" << std::endl;
	for (auto& id : ids)
		std::cout << "  " << id << std::endl;

	auto plugin = pm.getPlugin(ids[0]);

	std::cout << "Features:" << std::endl;
	for (auto& name : *plugin)
		std::cout << "  " << name << std::endl;

	std::string fStr;
	std::cout << " >>> ";
	std::getline(std::cin, fStr);

	PluS::UniqueID uid = pm.findFeature(fStr);
	auto feature = pm.createFeature<MathFeature>(uid);
	std::cout << "Created feature '" << feature->getName() << "'" << std::endl;

	std::cout << "  Result: " << feature->calc(12.0f, 23.0f) << std::endl;

	pm.destroyFeature(feature);
	std::cout << "Destroyed feature '" << fStr << "'" << std::endl;

	pm.unloadPlugin(ids[0]);
}