#include "PluS/PluginFeatureIterator.h"

namespace PluS
{
	FeatureSearchIterator::FeatureSearchIterator(const _PluginDataMap & map, _PluginDataMap::const_iterator curr, const std::string & featureName)
		: m_map(map), m_iterator(curr), m_featureName(featureName)
	{
		if (m_iterator != m_map.end() && !isMatch(m_iterator))
			++(*this);
	}

	FeatureSearchIterator FeatureSearchIterator::begin() const
	{
		return FeatureSearchIterator(m_map, m_iterator, m_featureName);
	}

	FeatureSearchIterator FeatureSearchIterator::end() const
	{
		return FeatureSearchIterator(m_map, m_map.end(), m_featureName);
	}

	bool FeatureSearchIterator::operator!=(const FeatureSearchIterator& other) const
	{
		return m_iterator != other.m_iterator;
	}

	FeatureSearchIterator& FeatureSearchIterator::operator++()
	{
		while (++m_iterator != m_map.end())
		{
			if (isMatch(m_iterator))
				break;
		}
		return *this;
	}

	UniqueID FeatureSearchIterator::operator*() const
	{
		return MakeUniqueID(m_iterator->first, m_currFeatureID);
	}

	bool FeatureSearchIterator::isMatch(_PluginDataMap::const_iterator it)
	{
		m_currFeatureID = it->second.getInstance()->getFeatureID(m_featureName);
		return m_currFeatureID != 0;
	}
}