#define BUILD_PLUGIN
#include "PluS.h"

class TestFeature : public Feature
{
	void printName() { printf("TestFeature\n"); }
};

PluginPtr CreatePlugin()
{
	return new Plugin("TestPlugin");
}