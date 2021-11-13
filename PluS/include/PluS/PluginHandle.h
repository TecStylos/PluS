#pragma once

#include <memory>
#include <string>

#include "Defines.h"

namespace PluS {

	class PluginHandle;

	typedef std::shared_ptr<PluginHandle> PluginHandleRef;

	class PluginHandle
	{
	public:
		/*
		* Constructor of PluginHandle.
		* 
		* @param path Path of the plugin to load.
		*/
		PLUS_API PluginHandle(const std::string& path);
		PluginHandle(const PluginHandle&) = delete;
		PluginHandle(PluginHandle&&) = delete;
		PluginHandle& operator=(PluginHandle&) = delete;
		/*
		* Destructor of PluginHandle.
		*/
		PLUS_API virtual ~PluginHandle() = default;
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
		PLUS_API virtual operator bool() const = 0;
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
	  return (Type)(getImpl(name));
	}

} // namespace PluS
