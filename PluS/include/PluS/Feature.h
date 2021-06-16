#pragma once

class Feature
{
public:
	Feature() = default;
	virtual ~Feature() = default;
};

typedef Feature* (*FeatureCreator)();