#pragma once

#include "Types.h"

namespace PluS {

	class FeatureIterator
	{
	public:
		FeatureIterator() = delete;
		FeatureIterator(_FeatureMap::const_iterator begin, _FeatureMap::const_iterator end, _FeatureMap::const_iterator curr)
			: m_begin(begin), m_end(end), m_iterator(curr)
		{}
	public:
		PLUS_API FeatureIterator begin() const;
		PLUS_API FeatureIterator end() const;
		PLUS_API bool operator!=(const FeatureIterator& other) const;
		PLUS_API FeatureIterator& operator++();
		PLUS_API const std::string& operator*() const;
	private:
		_FeatureMap::const_iterator m_begin;
		_FeatureMap::const_iterator m_end;
		_FeatureMap::const_iterator m_iterator;
	};
}