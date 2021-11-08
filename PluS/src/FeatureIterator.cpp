#include "PluS/FeatureIterator.h"

namespace PluS
{
	FeatureIterator FeatureIterator::begin() const
	{
		return FeatureIterator(m_begin, m_end, m_begin);
	}
	FeatureIterator FeatureIterator::end() const
	{
		return FeatureIterator(m_begin, m_end, m_end);
	}
	bool FeatureIterator::operator!=(const FeatureIterator& other) const
	{
		return m_iterator != other.m_iterator;
	}
	FeatureIterator& FeatureIterator::operator++()
	{
		++m_iterator;
		return *this;
	}
	const std::string& FeatureIterator::operator*() const
	{
		return m_iterator->first;
	}
}