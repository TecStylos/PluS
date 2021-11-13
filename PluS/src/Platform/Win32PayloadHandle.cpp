#include "PluS/Platform/Win32PayloadHandle.h"

#include <Psapi.h>
#include <vector>

namespace PluS
{
	Win32PayloadHandle::Win32PayloadHandle(const std::string& payloadPath, PLUS_PROCESS_ID processID, PayloadID payloadID)
		: PayloadHandle(payloadPath, processID, payloadID)
	{}

	Win32PayloadHandle::~Win32PayloadHandle()
	{
		detach();
	}

	bool Win32PayloadHandle::call(const std::string& funcName, void* param) const
	{
		auto func = getPayloadFuncAddr(funcName);

		DWORD threadID;
		Win32HandleHelper hThread = CreateRemoteThread(*m_hProcTarget, NULL, 0, func, param, 0, &threadID);
		if (!*hThread)
			return false;

		DWORD retVal;
		WaitForSingleObject(*hThread, INFINITE);
		GetExitCodeThread(*hThread, (LPDWORD)&retVal);

		return true;
	}

	bool Win32PayloadHandle::injectImpl()
	{
		uint64_t payloadPathSize = m_payloadPath.size() + 1;

		m_hProcTarget = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_processID);
		if (!*m_hProcTarget)
			return false;

		void* nameMem = VirtualAllocEx(*m_hProcTarget, NULL, payloadPathSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!nameMem)
			return false;

		if (!WriteProcessMemory(*m_hProcTarget, nameMem, m_payloadPath.c_str(), payloadPathSize, 0))
			return false;

		HMODULE kernel32 = GetModuleHandle("kernel32");
		if (!kernel32)
			return false;

		auto funcLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32, "LoadLibraryA");

		Win32HandleHelper hThread = CreateRemoteThread(*m_hProcTarget, NULL, 0, funcLoadLibrary, nameMem, 0, nullptr);
		if (!*hThread)
			return false;

		WaitForSingleObject(*hThread, INFINITE);

		m_payloadBase = getPayloadBaseAddr();

		if (!m_payloadBase)
			return false;

		return true;
	}

	bool Win32PayloadHandle::detachImpl()
	{
		// TODO: Implement payload detach.
		return true;
	}

	HMODULE Win32PayloadHandle::getPayloadBaseAddr() const
	{
		bool success = true;
		std::vector<HMODULE> modules(16);
		DWORD nBytesNeeded = 0;

		if (!EnumProcessModules(*m_hProcTarget, modules.data(), modules.size() * sizeof(HMODULE), &nBytesNeeded))
			return nullptr;
		if (modules.size() * sizeof(HMODULE) < nBytesNeeded)
		{
			modules.resize(nBytesNeeded / sizeof(HMODULE));
			if (!EnumProcessModules(*m_hProcTarget, modules.data(), modules.size() * sizeof(HMODULE), &nBytesNeeded))
				return nullptr;
		}

		DWORD nModules = nBytesNeeded / sizeof(HMODULE);

		for (DWORD i = 0; i < nModules; ++i)
		{
			auto hModule = modules[i];
			char nameBuff[1024];
			if (!GetModuleFileNameEx(*m_hProcTarget, hModule, nameBuff, sizeof(nameBuff)))
				continue;

			if (nameBuff == m_payloadPath)
				return hModule;
		}

		return nullptr;
	}

	LPTHREAD_START_ROUTINE Win32PayloadHandle::getPayloadFuncAddr(const std::string& funcName) const
	{
		HMODULE hLoaded = LoadLibraryA(m_payloadPath.c_str());
		if (!hLoaded)
			return nullptr;

		void* lpFunc = GetProcAddress(hLoaded, funcName.c_str());
		uint64_t offset = (char*)lpFunc - (char*)hLoaded;
		return (LPTHREAD_START_ROUTINE)((uint64_t)m_payloadBase + offset);
	}
}