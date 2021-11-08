#include "PluS/Platform/UnixPluginHandle.h"

namespace PluS
{
    UnixPluginHandle::UnixPluginHandle(const std::string& path)
        : PluginHandle(path)
    {
        m_handle = dlopen(path.c_str(), RTLD_NOW);
    }

    UnixPluginHandle::~UnixPluginHandle()
    {
        if (m_handle)
            dlclose(m_handle);
        m_handle = nullptr;
    }

    UnixPluginHandle::operator bool() const
    {
        return m_handle != nullptr;
    }

    void* UnixPluginHandle::getImpl(const std::string& name)
    {
        return dlsym(m_handle, name.c_str());
    }
}