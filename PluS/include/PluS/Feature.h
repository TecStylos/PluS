#pragma once

#include <string>
#include <memory>

#include "Types.h"

namespace PluS {
	class Feature;

	typedef Feature* FeaturePtr;

	typedef FeaturePtr(*FeatureCreator)(UniqueID uid);

	class Feature
	{
	public:
		/*
		* Constructor of Feature.
		*/
		Feature() = default;
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
		template <class CFeature, typename ...Args>
		static FeaturePtr create(UniqueID uid, Args...);
		template <class CFeature, typename ...Args>
		static CFeature* createNoConvert(UniqueID uid, Args...);
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
	template <class CFeature, typename ...Args>
	FeaturePtr Feature::create(UniqueID uid, Args...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		FeaturePtr feature = new CFeature(Args...);
		feature->m_uid = uid;
		return feature;
	}

	/*
	* Create a new feature. (Should be used in function passed to RegisterFeatureCreator)
	*
	* @param uid Unique ID of the feature. (Retreived through FeatureCreator parameter)
	* @param ... Feature specific parameters.
	* @returns Pointer to newly created feature with type CFeature*.
	*/
	template <class CFeature, typename ...Args>
	CFeature* Feature::createNoConvert(UniqueID uid, Args...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		CFeature* feature = new CFeature(Args...);
		feature->m_uid;
		return feature;
	}

} // namespace PluS