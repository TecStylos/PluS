#define PLUS_PLATFORM_WINDOWS
#include "PluS.h"
#include "MathFeature.h"

#include <iostream>

int main()
{
	auto& pm = PluS::PluginManager::get();
	auto ids = pm.loadPluginDir("../TestPlugin/", ".dll", true);
	std::cout << "Loaded plugins:" << std::endl;
	for (auto& id : ids)
		std::cout << "  " << id << std::endl;

	auto plugin = pm.getPlugin(ids[0]);

	std::cout << "Plugin specific features:" << std::endl;
	for (auto& name : plugin->getFeatureIterator())
		std::cout << "  " << name << std::endl;

	std::cout << "Global features with name 'add':" << std::endl;
	for (auto uid : pm.findFeatures("add"))
		printf("  %llu (P:%llu - F:%llu) :> %s\n", (uint64_t)uid.full, (uint64_t)uid.plugin, (uint64_t)uid.feature, pm.getFeatureName(uid));

	std::string fStr;
	std::cout << " >>> ";
	std::getline(std::cin, fStr);

	PluS::UniqueID uid = pm.findFeature(fStr);
	if (uid)
	{
		auto feature = pm.createFeature<MathFeature>(uid);
		std::cout << "Created feature '" << feature->getName() << "'" << std::endl;

		std::cout << "  Result: " << feature->calc(12.0f, 23.0f) << std::endl;

		pm.destroyFeature(feature);
		std::cout << "Destroyed feature '" << fStr << "'" << std::endl;
	}
	else
	{
		std::cout << "Unable to find feature '" << fStr << "'!" << std::endl;
	}

	std::cout << "Number of references to plugin with ID=" << ids[0] << ": " << pm.getPluginRefCount(ids[0]) << std::endl;

	pm.unloadPlugin(ids[0]);
	std::cout << "Unloaded plugin with ID=" << ids[0] << std::endl;
	std::cout << "Number of references to plugin with ID=" << ids[0] << ": " << pm.getPluginRefCount(ids[0]) << std::endl;
}