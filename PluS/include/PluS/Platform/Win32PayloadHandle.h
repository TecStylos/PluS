#pragma once

#include <Windows.h>

#include "../PayloadHandle.h"

#include "PluS/Platform/Win32HandleHelper.h"

namespace PluS
{
	class Win32PayloadHandle : public PayloadHandle
	{
	public:
		Win32PayloadHandle(const std::string& payloadPath, PLUS_PROCESS_ID processID, PayloadID payloadID);
		~Win32PayloadHandle() = default;
	public:
		virtual bool call(const std::string& funcName, void* param) const override;
	protected:
		virtual bool injectImpl() override;
		virtual bool detachImpl() override;
	private:
		HMODULE getPayloadBaseAddr() const;
		LPTHREAD_START_ROUTINE getPayloadFuncAddr(const std::string& funcName) const;
	private:
		HMODULE m_payloadBase = nullptr;
		Win32HandleHelper m_hProcTarget = 0;
	};
}