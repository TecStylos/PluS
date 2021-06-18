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
		FeatureIterator begin() const
		{
			return FeatureIterator(m_begin, m_end, m_begin);
		}
		FeatureIterator end() const
		{
			return FeatureIterator(m_begin, m_end, m_end);
		}
		bool operator!=(const FeatureIterator& other) const
		{
			return m_iterator != other.m_iterator;
		}
		FeatureIterator& operator++()
		{
			++m_iterator;
			return *this;
		}
		const std::string& operator*() const
		{
			return m_iterator->first;
		}
	private:
		_FeatureMap::const_iterator m_begin;
		_FeatureMap::const_iterator m_end;
		_FeatureMap::const_iterator m_iterator;
	};
}