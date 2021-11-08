#include "PluS/Platform/Win32PluginHandle.h"

namespace PluS
{
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