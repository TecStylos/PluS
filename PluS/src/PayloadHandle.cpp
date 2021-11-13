#include "PluS/PayloadHandle.h"

#if defined PLUS_PLATFORM_WINDOWS
#include "PluS/Platform/Win32PayloadHandle.h"
#elif defined PLUS_PLATFORM_UNIX
#error Unix currently unsupported!
#endif

#include <stdexcept>

namespace PluS
{
	PayloadHandle::~PayloadHandle()
	{
		if (m_isInjected)
			throw std::runtime_error("Implementation of payload handle did not detach the payload before destruction!");
	}

	PayloadHandleRef PayloadHandle::create(const std::string& payloadPath, ProcessID processID, PayloadID payloadID)
	{
		#if defined PLUS_PLATFORM_WINDOWS
		return std::make_shared<Win32PayloadHandle>(payloadPath, processID, payloadID);
		#elif defined PLUS_PLATFORM_UNIX
		#error Unix currently unsupported!
		#endif
	}

	bool PayloadHandle::inject()
	{
		if (m_isInjected)
			return true;

		if (!injectImpl())
			return false;

		m_isInjected = true;
		return true;
	}

	bool PayloadHandle::detach()
	{
		if (!m_isInjected)
			return true;

		if (!detachImpl())
			return false;

		m_isInjected = false;
		return true;
	}

	PayloadID PayloadHandle::getID() const
	{
		return m_payloadID;
	}

	PayloadHandle::operator bool() const
	{
		return m_isInjected;
	}

	PayloadHandle::PayloadHandle(const std::string& payloadPath, ProcessID processID, PayloadID payloadID)
		: m_payloadPath(payloadPath), m_processID(processID), m_payloadID(payloadID)
	{}
}