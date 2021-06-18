#define PLUS_BUILD_PLUGIN
#include "PluS.h"

const std::string PluS::PerPlugin::pluginName = "TemplatePlugin";

class TemplateFeature : PluS::Feature
{
public:
	PLUS_FEATURE_GET_NAME("TemplateFeature");
};

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureFactory(FeatureFactory::create<TemplateFeature>());
}

void PluS::PerPlugin::shutdownPlugin()
{
	;
}