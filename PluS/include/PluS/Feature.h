#pragma once

#include <string>
#include <memory>

namespace PluS {
	class Feature
	{
	public:
		Feature() = default;
		virtual ~Feature() = default;
	public:
		virtual const char* getName() const = 0;
	};

	typedef Feature* FeaturePtr;

	typedef FeaturePtr (*FeatureCreator)();

} // namespace PluS