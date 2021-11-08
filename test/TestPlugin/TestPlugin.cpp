#define PLUS_BUILD_PLUGIN
#include "PluS.h"
#include "../Sandbox/MathFeature.h"

class AddFeature : public MathFeature
{
public:
  using MathFeature::MathFeature;
  PLUS_FEATURE_GET_NAME("add");
	virtual float calc(float a, float b) const override { return a + b; }
};

class SubFeature : public MathFeature
{
public:
  using MathFeature::MathFeature;
	PLUS_FEATURE_GET_NAME("sub");
	virtual float calc(float a, float b) const override { return a - b; }
};

class MulFeature : public MathFeature
{
public:
    using MathFeature::MathFeature;
	PLUS_FEATURE_GET_NAME("mul");
	virtual float calc(float a, float b) const override { return a * b; }
};

class DivFeature : public MathFeature
{
public:
    using MathFeature::MathFeature;
	PLUS_FEATURE_GET_NAME("div");
	virtual float calc(float a, float b) const override { return a / b; }
};

PLUS_PERPLUGIN_DEFINE_EXTERNALS("MathPlugin");

void PluS::PerPlugin::initPlugin()
{
	pPlugin->registerFeatureFactory(FeatureFactory::create<AddFeature>());
	pPlugin->registerFeatureFactory(FeatureFactory::create<SubFeature>());
	pPlugin->registerFeatureFactory(FeatureFactory::create<MulFeature>());
	pPlugin->registerFeatureFactory(FeatureFactory::create<DivFeature>());
}

void PluS::PerPlugin::shutdownPlugin()
{
}
