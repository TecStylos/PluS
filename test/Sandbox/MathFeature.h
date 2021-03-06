#define BUILD_PLUGIN
#include "PluS.h"

class MathFeature : public PluS::Feature
{
public:
  using PluS::Feature::Feature;
  virtual float calc(float a, float b) const = 0;
};
