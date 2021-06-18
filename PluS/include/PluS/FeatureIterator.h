#pragma once

namespace PluS {
	typedef std::map<std::string, FeatureID> _FeatureMap;

	class FeatureIterator
	{
	public:
		FeatureIterator() = delete;
		FeatureIterator(_FeatureMap::const_iterator begin, _FeatureMap::const_iterator end, _FeatureMap::const_iterator curr)
			: m_begin(begin), m_end(end), m_iterator(curr)
		{}
	public:
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