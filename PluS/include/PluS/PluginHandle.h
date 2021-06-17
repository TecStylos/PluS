#pragma once

#include <memory>
#include <string>

namespace PluS {

	class PluginHandle;

	typedef std::shared_ptr<PluginHandle> PluginHandlePtr;

	class PluginHandle
	{
	public:
		PluginHandle(const std::string& path)
			: m_path(path)
		{}
		PluginHandle(const PluginHandle&) = delete;
		PluginHandle(PluginHandle&&) = delete;
		PluginHandle& operator=(PluginHandle&) = delete;
		virtual ~PluginHandle() = default;
	public:
		template <typename Type>
		Type get(const std::string& name);
	public:
		virtual operator bool() const = 0;
	protected:
		virtual void* getImpl(const std::string& name) = 0;
	private:
		std::string m_path;
	};

	template <typename Type>
	Type PluginHandle::get(const std::string& name)
	{
		return static_cast<Type>(getImpl(name));
	}

} // namespace PluS