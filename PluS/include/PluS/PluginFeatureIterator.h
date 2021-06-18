#pragma once

#include "Plugin.h"

namespace PluS {
	class PluginFeatureIterator
	{
	public:
		PluginFeatureIterator() = delete;
		PluginFeatureIterator(_PluginDataMap::const_iterator begin, _PluginDataMap::const_iterator end, _PluginDataMap::const_iterator curr, const std::string& featureName)
			: m_begin(begin), m_end(end), m_iterator(curr), m_featureName(featureName)
		{
			if (m_iterator != m_end && !isMatch(m_iterator))
				++(*this);
		}
	public:
		PluginFeatureIterator begin() const
		{
			return PluginFeatureIterator(m_begin, m_end, m_iterator, m_featureName);
		}
		PluginFeatureIterator end() const
		{
			return PluginFeatureIterator(m_begin, m_end, m_end, m_featureName);
		}
		bool operator!=(const PluginFeatureIterator& other) const
		{
			return m_iterator != other.m_iterator;
		}
		PluginFeatureIterator& operator++()
		{
			while (++m_iterator != m_end)
			{
				if (isMatch(m_iterator))
					break;
			}
			return *this;
		}
		UniqueID operator*() const
		{
			return MakeUniqueID(m_iterator->first, m_currFeatureID);
		}
	private:
		bool isMatch(_PluginDataMap::const_iterator it)
		{
			m_currFeatureID = it->second.getInstance()->getFeatureID(m_featureName);
			return m_currFeatureID != 0;
		}
	private:
		_PluginDataMap::const_iterator m_begin;
		_PluginDataMap::const_iterator m_end;
		_PluginDataMap::const_iterator m_iterator;
		std::string m_featureName;
		FeatureID m_currFeatureID = 0;
	};
}