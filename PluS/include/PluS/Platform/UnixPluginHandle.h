#pragma once

#include <dlfcn.h>

#include "../PluginHandle.h"

namespace PluS {
    class UnixPluginHandle : public PluginHandle
    {
    public:
        PLUS_API UnixPluginHandle(const std::string& path);
        PLUS_API virtual ~UnixPluginHandle();
    public:
        PLUS_API virtual operator bool() const override;
    protected:
        virtual void* getImpl(const std::string& name) override;
    private:
        void* m_handle = nullptr;
    };
}
