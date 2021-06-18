#pragma once

#include <type_traits>

#include "Feature.h"

namespace PluS {

	struct FeatureFactory
	{
		FeatureCreator creator;
		const char*(*getName)();
	public:
		template <class CFeature>
		static FeatureFactory create();
		template <class CFeature>
		static FeatureFactory create(FeatureCreator creator);
	};

	template <class CFeature>
	FeatureFactory FeatureFactory::create()
	{
		return create<CFeature>(
			[](UniqueID uid)
			{
				return Feature::create<CFeature>(uid);
			}
		);
	}

	template <class CFeature>
	FeatureFactory FeatureFactory::create(FeatureCreator creator)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");

		FeatureFactory ff;
		ff.creator = creator;
		ff.getName = &CFeature::_StaticGetName;

		return ff;
	}
} // namespace PluS