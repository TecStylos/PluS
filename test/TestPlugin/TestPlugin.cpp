#define BUILD_PLUGIN
#include "PluS.h"
#include "../Sandbox/MathFeature.h"

class AddFeature : public MathFeature
{
public:
	virtual const char* getName() const override { return "add"; }
	virtual float calc(float a, float b) const override { return a + b; }
};

class SubFeature : public MathFeature
{
public:
	virtual const char* getName() const override { return "sub"; }
	virtual float calc(float a, float b) const override { return a - b; }
};

class MulFeature : public MathFeature
{
public:
	virtual const char* getName() const override { return "mul"; }
	virtual float calc(float a, float b) const override { return a * b; }
};

class DivFeature : public MathFeature
{
public:
	virtual const char* getName() const override { return "div"; }
	virtual float calc(float a, float b) const override { return a / b; }
};

const std::string PluS::PerPlugin::pluginName = "MathPlugin";

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureCreator("add", []{ return (FeaturePtr)new AddFeature(); });
	getPlugin()->registerFeatureCreator("sub", []{ return (FeaturePtr)new SubFeature(); });
	getPlugin()->registerFeatureCreator("mul", []{ return (FeaturePtr)new MulFeature(); });
	getPlugin()->registerFeatureCreator("div", []{ return (FeaturePtr)new DivFeature(); });
}

void PluS::PerPlugin::shutdownPlugin()
{
}