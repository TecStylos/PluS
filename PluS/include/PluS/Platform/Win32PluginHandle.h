#pragma once

#define NOMINMAX
#include <Windows.h>

#include "../PluginHandle.h"

namespace PluS {
	class Win32PluginHandle : public PluginHandle
	{
	public:
		/*
		* Constructor of Win32PluginHandle.
		*/
		PLUS_API Win32PluginHandle(const std::string& path);
		/*
		* Destructor of Win32PluginHandle
		*/
		PLUS_API virtual ~Win32PluginHandle();
	public:
		PLUS_API virtual operator bool() const override;
	protected:
		virtual void* getImpl(const std::string& name) override;
	private:
		HMODULE m_hModule = NULL;
	};
}