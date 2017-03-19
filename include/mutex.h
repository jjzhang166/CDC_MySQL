
#ifndef _MUTEX_H_
#define _MUTEX_H_

#if defined (_MSC_VER)
#include <windows.h>

#else	//linux
#include <pthread.h>
#include <stdlib.h>      // for abort()
#endif

#include <cassert>

#ifdef _MSC_VER
	typedef CRITICAL_SECTION MutexType;
#else	
	typedef pthread_mutex_t MutexType;
#endif

class Mutex 
{
public:
#ifdef _MSC_VER

	Mutex()             { InitializeCriticalSection(&_mutex); }
	~Mutex()            { DeleteCriticalSection(&_mutex); }
	void Lock()         { EnterCriticalSection(&_mutex); }
	void Unlock()       { LeaveCriticalSection(&_mutex); }

#else

#define SAFE_PTHREAD(fncall)  do {    \
	if (fncall(&_mutex) != 0) abort();  \
	  } while (0)

	Mutex()             
	{
		if (pthread_mutex_init(&_mutex, NULL) != 0) abort();
	}
	~Mutex()            { SAFE_PTHREAD(pthread_mutex_destroy); }
	void Lock()         { SAFE_PTHREAD(pthread_mutex_lock); }
	void Unlock()       { SAFE_PTHREAD(pthread_mutex_unlock); }
#undef SAFE_PTHREAD

#endif

private:
	MutexType _mutex;

	Mutex(Mutex* /*ignored*/) {}
	Mutex(const Mutex&);
	void operator=(const Mutex&);
};	


// MutexLock(mu) acquires mu when constructed and releases it when destroyed.
class MutexLock 
{
public:
	explicit MutexLock(Mutex *mu) : _pMutex(mu), _isUnlocked(false) 
	{
		assert(0 != _pMutex);
		_pMutex->Lock(); 
	}

	~MutexLock() { this->Unlock(); }

	void Unlock() 
	{
		if (!_isUnlocked)
		{
			_pMutex->Unlock();
			_isUnlocked = true;
		}
	}

private:		
	Mutex* const _pMutex;
	bool _isUnlocked;
	MutexLock(const MutexLock&);
	void operator=(const MutexLock&);
};

#endif  // #define _MUTEX_H_ 
