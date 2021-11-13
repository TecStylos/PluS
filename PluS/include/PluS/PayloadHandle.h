#pragma once

#include <memory>
#include <map>
#include <string>

#include "Defines.h"
#include "Types.h"

namespace PluS
{
	class PayloadHandle;

	typedef std::shared_ptr<PayloadHandle> PayloadHandleRef;

	class PayloadHandle
	{
	public:
		virtual ~PayloadHandle();
	public:
		static PayloadHandleRef create(const std::string& payloadPath, ProcessID processID, PayloadID payloadID);
	public:
		PLUS_API bool inject();
		PLUS_API bool detach();
		PLUS_API PayloadID getID() const;
		operator bool() const;
	public:
		virtual bool call(const std::string& funcName, const void* param, uint64_t paramSize) = 0;
	protected:
		virtual bool injectImpl() = 0;
		virtual bool detachImpl() = 0;
	protected:
		PayloadHandle(const std::string& payloadPath, ProcessID processID, PayloadID payloadID);
	protected:
		PayloadID m_payloadID;
		std::string m_payloadPath;
		ProcessID m_processID;
		bool m_isInjected = false;
	};

	typedef std::map<PayloadID, PayloadHandleRef> _PayloadMap;
}