#pragma once

#include <dlfcn.h>

#include "../PluginHandle.h"

namespace PluS {
  class UnixPluginHandle : public PluginHandle
  {
  public:
    UnixPluginHandle(const std::string& path);
    virtual ~UnixPluginHandle();
  public:
    virtual operator bool() const override;
  protected:
    virtual void* getImpl(const std::string& name) override;
  private:
    void* m_handle = nullptr;
  };

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
