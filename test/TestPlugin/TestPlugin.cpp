#define BUILD_PLUGIN
#include "PluS.h"

class TestFeature : public PluS::Feature
{
public:
	virtual const char* getName() const override { return "TestFeature"; }
};

const std::string PluS::PerPlugin::pluginName = "TestPlugin";

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureCreator("TestFeature", []{ return (FeaturePtr)new TestFeature(); });
}

void PluS::PerPlugin::shutdownPlugin()
{
}