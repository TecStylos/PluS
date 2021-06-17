#pragma once

#include <string>
#include <memory>

#include "Types.h"

namespace PluS {
	class Feature;

	typedef Feature* FeaturePtr;

	typedef FeaturePtr(*FeatureCreator)(UniqueID uid, bool isCheck);

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
	private:
		UniqueID m_uid;
	private:
		template <class CFeature, typename ...Args>
		friend FeaturePtr CreateFeature(UniqueID uid, Args...);
		template <class CFeature, typename ...Args>
		friend CFeature* CreateFeatureNoConvert(UniqueID uid, Args...);
	};

	/*
	* Create a new feature. (Should be used in function passed to RegisterFeatureCreator)
	* 
	* @param uid Unique ID of the feature. (Retreived through FeatureCreator parameter)
	* @param ... Feature specific parameters.
	* @returns Pointer to newly created feature.
	*/
	template <class CFeature, typename ...Args>
	FeaturePtr CreateFeature(UniqueID uid, Args...)
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
	CFeature* CreateFeatureNoConvert(UniqueID uid, Args...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		CFeature* feature = new CFeature(Args...);
		feature->m_uid;
		return feature;
	}

} // namespace PluS