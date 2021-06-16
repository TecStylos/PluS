#pragma once

#include <string>

namespace PluS {
	class Feature
	{
	public:
		Feature() = default;
		virtual ~Feature() = default;
	public:
		virtual const char* getName() const = 0;
	};

	typedef Feature* (*FeatureCreator)();

} // namespace PluS