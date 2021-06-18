#pragma once

#include "PluS/Feature.h"
#include "PluS/Plugin.h"

#ifdef PLUS_BUILD_PLUGIN
#include "PluS/FeatureFactory.h"
#include "PluS/PluginImplementation.h"
#else
#include "PluS/PluginManager.h"
#endif