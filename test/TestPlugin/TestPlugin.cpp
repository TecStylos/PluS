#define BUILD_PLUGIN
#include "PluginSystem.h"

class TestFeature : public Feature
{
	void printName() { printf("TestFeature\n"); }
};

class TestPlugin : public Plugin
{
public:
	std::vector<std::string> getFeatures() const override
	{
		return { "TestFeature" };
	}
	FeaturePtr createFeature(const std::string& name) override
	{
		if (name == "TestFeature")
			return new TestFeature();
		return nullptr;
	}
	void destroyFeature(FeaturePtr feature) override
	{
		delete feature;
	}
	const char* getName() const override
	{
		return "TestPlugin";
	}
};

Plugin* CreatePlugin()
{
	return new TestPlugin();
}