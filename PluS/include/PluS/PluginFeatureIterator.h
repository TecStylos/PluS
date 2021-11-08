#pragma once

#include "Plugin.h"

namespace PluS {
	class FeatureSearchIterator
	{
	public:
		FeatureSearchIterator() = delete;
		PLUS_API FeatureSearchIterator(const _PluginDataMap& map, _PluginDataMap::const_iterator curr, const std::string& featureName);
	public:
		PLUS_API FeatureSearchIterator begin() const;
		PLUS_API FeatureSearchIterator end() const;
		PLUS_API bool operator!=(const FeatureSearchIterator& other) const;
		PLUS_API FeatureSearchIterator& operator++();
		PLUS_API UniqueID operator*() const;
	private:
		bool isMatch(_PluginDataMap::const_iterator it);
	private:
		const _PluginDataMap& m_map;
		_PluginDataMap::const_iterator m_iterator;
		std::string m_featureName;
		FeatureID m_currFeatureID = 0;
	};
}