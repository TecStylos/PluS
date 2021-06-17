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
		Feature() = default;
		virtual ~Feature() = default;
	public:
		virtual const char* getName() const = 0;
		virtual UniqueID getUniqueID() const { return m_uid; };
	private:
		UniqueID m_uid;
	private:
		template <class CFeature, typename ...Args>
		friend FeaturePtr CreateFeature(UniqueID uid, Args...);
		template <class CFeature, typename ...Args>
		friend CFeature* CreateFeatureNoConvert(UniqueID uid, Args...);
	};

	template <class CFeature, typename ...Args>
	FeaturePtr CreateFeature(UniqueID uid, Args...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		FeaturePtr feature = new CFeature(Args...);
		feature->m_uid = uid;
		return feature;
	}

	template <class CFeature, typename ...Args>
	CFeature* CreateFeatureNoConvert(UniqueID uid, Args...)
	{
		static_assert(std::is_base_of<Feature, CFeature>::value, "CFeature must derive from Feature!");
		CFeature* feature = new CFeature(Args...);
		feature->m_uid;
		return feature;
	}

} // namespace PluS