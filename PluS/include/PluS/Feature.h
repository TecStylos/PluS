#pragma once

#include <string>
#include <memory>

#include "Types.h"

namespace PluS {
	class Feature
	{
	public:
		Feature() = delete;
		Feature(UniqueID uid)
			: m_uid(uid)
		{}
		virtual ~Feature() = default;
	public:
		virtual const char* getName() const = 0;
		virtual UniqueID getUniqueID() const { return m_uid; };
	private:
		UniqueID m_uid;
	};

	typedef Feature* FeaturePtr;

	typedef FeaturePtr (*FeatureCreator)(UniqueID uid, bool isCheck);

} // namespace PluS