#pragma once

#include <Windows.h>

#include "PluginHandle.h"

namespace PluS {
	class Win32PluginHandle : public PluginHandle
	{
	public:
		Win32PluginHandle(const std::string& path);
		~Win32PluginHandle();
	public:
		virtual operator bool() const override;
	protected:
		virtual void* getImpl(const std::string& name) override;
	private:
		HMODULE m_hModule = NULL;
	};

	Win32PluginHandle::Win32PluginHandle(const std::string& path)
		: PluginHandle(path)
	{
		m_hModule = LoadLibraryA(path.c_str());
	}

	Win32PluginHandle::~Win32PluginHandle()
	{
		if (m_hModule)
			FreeLibrary(m_hModule);
		m_hModule = nullptr;
	}

	Win32PluginHandle::operator bool() const
	{
		return m_hModule != NULL;
	}

	void* Win32PluginHandle::getImpl(const std::string& name)
	{
		return GetProcAddress(m_hModule, name.c_str());
	}
}