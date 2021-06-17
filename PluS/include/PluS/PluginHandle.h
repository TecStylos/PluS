#pragma once

#include <memory>
#include <string>

namespace PluS {

	class PluginHandle;

	typedef std::shared_ptr<PluginHandle> PluginHandlePtr;

	class PluginHandle
	{
	public:
		/*
		* Constructor of PluginHandle.
		* 
		* @param path Path of the plugin to load.
		*/
		PluginHandle(const std::string& path)
			: m_path(path)
		{}
		PluginHandle(const PluginHandle&) = delete;
		PluginHandle(PluginHandle&&) = delete;
		PluginHandle& operator=(PluginHandle&) = delete;
		/*
		* Destructor of PluginHandle.
		*/
		virtual ~PluginHandle() = default;
	public:
		/*
		* Get an object (function, variable) from the plugin.
		* 
		* @param name Name of the object to retreive.
		* @returns Object found in the plugin with type Type.
		*/
		template <typename Type>
		Type get(const std::string& name);
	public:
		/*
		* Get wether the underlying plugin is loaded or not.
		* 
		* @returns True, if the underlying plugin is loaded, otherwise false.
		*/
		virtual operator bool() const = 0;
	protected:
		/*
		* Implementation of get method.
		* 
		* @param name Name of the object to retreive.
		* @returns Object found in the plugin.
		*/
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