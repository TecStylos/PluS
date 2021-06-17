#define PLUS_BUILD_PLUGIN
#include "PluS.h"

const std::string PluS::PerPlugin::pluginName = "TemplatePlugin";

class TemplateFeature : PluS::Feature
{
	using Feature::Feature;
public:
	virtual const char* getName() const override { return "TemplateFeature"; }
};

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureCreator([](UniqueID uid, bool isCheck) { return CreateFeature<TemplateFeature>(uid); })
}

void PluS::PerPlugin::shutdownPlugin()
{
	;
}