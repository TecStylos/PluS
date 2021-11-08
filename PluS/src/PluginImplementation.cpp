#include "PluS/PluginImplementation.h"

namespace PluS
{
	Plugin::Plugin(const std::string& name, PluginID pid)
		: m_name(name), m_pid(pid)
	{}

	Plugin::~Plugin()
	{
		clearCreatedFeatures();
	}

	FeaturePtr Plugin::createFeature(FeatureID fid)
	{
		FeatureCreator creator = getFeatureCreator(fid);
		if (!creator)
			return nullptr;

		FeaturePtr pFeature = creator(MakeUniqueID(getID(), fid));
		m_createdFeatures.insert(pFeature);
		return pFeature;
	}

	void Plugin::destroyFeature(FeaturePtr feature)
	{
		auto it = m_createdFeatures.find(feature);
		if (it == m_createdFeatures.end())
			return;
		m_createdFeatures.erase(feature);
	}

	FeatureID Plugin::getFeatureID(const std::string& name) const
	{
		auto it = m_features.find(name);
		if (it == m_features.end())
			return 0;
		return it->second;
	}

	const char* Plugin::getFeatureName(FeatureID fid) const
	{
		auto it = m_featureCreators.find(fid);
		if (it == m_featureCreators.end())
			return nullptr;
		return it->second.getName();
	}

	FeatureIterator Plugin::getFeatureIterator() const
	{
		return FeatureIterator(
			m_features.begin(),
			m_features.end(),
			m_features.begin()
		);
	}

	const std::string& Plugin::getName() const
	{
		return m_name;
	}

	PluginID Plugin::getID() const
	{
		return m_pid;
	}

	FeatureCreator Plugin::getFeatureCreator(FeatureID fid)
	{
		auto it = m_featureCreators.find(fid);
		if (it == m_featureCreators.end())
			return nullptr;
		return it->second.creator;
	}

	void Plugin::registerFeatureFactory(FeatureFactory factory)
	{
		// TODO: Store name
		FeatureID fid = m_nextFeatureID++;
		m_features.insert(std::make_pair(factory.getName(), fid));
		m_featureCreators.insert(std::make_pair(fid, factory));
	}

	void Plugin::clearCreatedFeatures()
	{
		while (!m_createdFeatures.empty())
			destroyFeature(*m_createdFeatures.begin());
	}
}