#define BUILD_PLUGIN
#include "PluS.h"
#include "../Sandbox/MathFeature.h"

class AddFeature : public MathFeature
{
public:
	using MathFeature::MathFeature;
public:
	virtual const char* getName() const override { return "add"; }
	virtual float calc(float a, float b) const override { return a + b; }
};

class SubFeature : public MathFeature
{
public:
	using MathFeature::MathFeature;
public:
	virtual const char* getName() const override { return "sub"; }
	virtual float calc(float a, float b) const override { return a - b; }
};

class MulFeature : public MathFeature
{
public:
	using MathFeature::MathFeature;
public:
	virtual const char* getName() const override { return "mul"; }
	virtual float calc(float a, float b) const override { return a * b; }
};

class DivFeature : public MathFeature
{
public:
	using MathFeature::MathFeature;
public:
	virtual const char* getName() const override { return "div"; }
	virtual float calc(float a, float b) const override { return a / b; }
};

const std::string PluS::PerPlugin::pluginName = "MathPlugin";

void PluS::PerPlugin::initPlugin()
{
	getPlugin()->registerFeatureCreator([](UniqueID uid, bool isCheck){ return (FeaturePtr)new AddFeature(uid); });
	getPlugin()->registerFeatureCreator([](UniqueID uid, bool isCheck){ return (FeaturePtr)new SubFeature(uid); });
	getPlugin()->registerFeatureCreator([](UniqueID uid, bool isCheck){ return (FeaturePtr)new MulFeature(uid); });
	getPlugin()->registerFeatureCreator([](UniqueID uid, bool isCheck){ return (FeaturePtr)new DivFeature(uid); });
}

void PluS::PerPlugin::shutdownPlugin()
{
}