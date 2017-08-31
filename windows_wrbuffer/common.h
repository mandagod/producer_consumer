#pragma once

#include <windows.h>

class CriticalSection {
public:
	// following based on the WIN32 API
	CriticalSection() { ::InitializeCriticalSection(&m_critSection); }
	~CriticalSection() { ::DeleteCriticalSection(&m_critSection); }
	void lock() { ::EnterCriticalSection(&m_critSection); }
	void unlock() { ::LeaveCriticalSection(&m_critSection); }
private:
	// forbidden copy and assign
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);
private:
	CRITICAL_SECTION m_critSection;
};

template<class SyncObject>
class GenericLocker {
public:
	explicit GenericLocker(const SyncObject& so)
		: m_refObj(const_cast<SyncObject&>(so)) {
		m_refObj.lock();
	}
	GenericLocker() { m_refObj.unlock(); }
private:
	GenericLocker(const GenericLocker<SyncObject>&);
	GenericLocker& operator=(const GenericLocker<SyncObject>&);
private:
	SyncObject& m_refObj;
};