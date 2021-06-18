#define PLUS_BUILD_PLUGIN
#include "PluS.h"
#include "../Sandbox/MathFeature.h"

class AddFeature : public MathFeature
{
public:
	PLUS_FEATURE_GET_NAME("add");
	virtual float calc(float a, float b) const override { return a + b; }
};

class SubFeature : public MathFeature
{
public:
	PLUS_FEATURE_GET_NAME("sub");
	virtual float calc(float a, float b) const override { return a - b; }
};

class MulFeature : public MathFeature
{
public:
	PLUS_FEATURE_GET_NAME("mul");
	virtual float calc(float a, float b) const override { return a * b; }
};

class DivFeature : public MathFeature
{
public:
	PLUS_FEATURE_GET_NAME("div");
	virtual float calc(float a, float b) const override { return a / b; }
};

const std::string PluS::PerPlugin::pluginName = "MathPlugin";

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureFactory(FeatureFactory::create<AddFeature>());
	getPlugin()->registerFeatureFactory(FeatureFactory::create<SubFeature>());
	getPlugin()->registerFeatureFactory(FeatureFactory::create<MulFeature>());
	getPlugin()->registerFeatureFactory(FeatureFactory::create<DivFeature>());
}

void PluS::PerPlugin::shutdownPlugin()
{
}