#pragma once

#include <cstdarg>
#include <string>
#include <memory>
#include <cstdarg>

#include "Types.h"

namespace PluS {
	class Feature;

	typedef Feature* FeaturePtr;

	typedef FeaturePtr(*FeatureCreator)(UniqueID uid);

	class Feature
	{
	public:
	  Feature() = delete;
		/*
		* Constructor of Feature.
		*/
	  Feature(va_list args) : m_uid(va_arg(args, UniqueID)) {}
		/*
		* Destructor of Feature.
		*/
		virtual ~Feature() = default;
	public:
		/*
		* Get the name of the feature.
		* 
		* @returns Name of the feature.
		*/
		virtual const char* getName() const = 0;
		/*
		* Get the unique ID of the feature.
		* 
		* @returns Unique ID of the feature.
		*/
		virtual UniqueID getUniqueID() const { return m_uid; };
	public:
		template <class CFeature>
		static FeaturePtr create(UniqueID uid, ...);
	private:
		UniqueID m_uid;
	};

	#define PLUS_FEATURE_GET_NAME(name) \
	virtual const char* getName() const override { return _StaticGetName(); } \
	static const char* _StaticGetName() { return name; }

	/*
	* Create a new feature. (Should be used in function passed to RegisterFeatureCreator)
	* 
	* @param uid Unique ID of the feature. (Retreived through FeatureCreator parameter)
	* @param ... Feature specific parameters.
	* @returns Pointer to newly created feature.
	*/
	template <class CFeature>
	FeaturePtr Feature::create(UniqueID uid, ...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		va_list args;
		va_start(args, uid);
		FeaturePtr feature = new CFeature(args);
		va_end(args);
		feature->m_uid = uid;
		return feature;
	}

} // namespace PluS
