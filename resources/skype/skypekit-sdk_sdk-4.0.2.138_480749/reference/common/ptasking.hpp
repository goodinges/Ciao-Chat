/*
 * Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained
 * in, relating to, or arising from the internet telephony software of Skype
 * Limited (including its affiliates, "Skype"), including without limitation
 * this source code, Skype API and related material of such software
 * proprietary to Skype and/or its licensors ("IP Rights") are and shall
 * remain the exclusive property of Skype and/or its licensors. The recipient
 * hereby acknowledges and agrees that any unauthorized use of the IP Rights
 * is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and ownership
 * thereof.
 */

#ifndef _PTASKING_H_
#define _PTASKING_H_

#include <pthread.h>

#include "debugging.hpp"

/// pthreads Mutex wrapper
struct pMutex
{
	pthread_mutex_t Mtx;

public:
	pMutex(void)
	{
		// Pthread init
		pthread_mutexattr_t attr;
		if (pthread_mutexattr_init(&attr))
			FATAL("pthread_mutexattr_init() failed");
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&Mtx, &attr);
		pthread_mutexattr_destroy(&attr);
	}
	~pMutex(void)
	{
		pthread_mutex_destroy(&Mtx);
	}
	pthread_mutex_t * Get()
	{
		return &Mtx;
	}
	bool Acquire(void);
	void Release(void);
};

/// auto scoping/descoping Mutex pointer
struct pMutexLock
{
	inline pMutexLock(pMutex & mtx) :
		mutex(mtx)
	{
		pthread_mutex_lock(mutex.Get());
	}
	inline ~pMutexLock()
	{
		pthread_mutex_unlock(mutex.Get());
	}
	pMutex & mutex;
private:
	pMutexLock(const pMutexLock&);
	const pMutexLock& operator=(const pMutexLock&);
};

/// auto scoping/descoping Mutex lock
struct MutexLock
{
	inline MutexLock(pthread_mutex_t& mutex) :
		mutex(mutex)
	{
		pthread_mutex_lock(&mutex);
	}
	inline ~MutexLock()
	{
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_t& mutex;
private:
	MutexLock(const MutexLock&);
	const MutexLock& operator=(const MutexLock&);
};

/// pthreads syncevent wrapper
class pSyncEvent
{
	pMutex internal_mutex;
	pthread_cond_t cond;
	timespec makeTimespec(int milliseconds);
public:
	pSyncEvent(void);
	~pSyncEvent(void);
	bool Wait(int milliseconds = -1);
	// wait and unlock given mutex handle - mutex handle has to be already locked
	bool Wait(pMutex &wait_mutex, int milliseconds = -1);
	void Trigger(void);
	void Trigger(pMutex &mutex);
};

/// pthreads thread wrapper
class pThread
{
	pthread_t thread_id;
	static void *thread_func(void * const data);
	virtual void Run(void)=0;

public:
	volatile bool Shutdown, Running, Detached;
	int StackSize;
	pThread(bool detached = true, int stackSize = -1);
	virtual ~pThread(void);
	void Start(const char *name=NULL);
	virtual void Stop(void);
	void Sleep(unsigned int milliseconds);
};

#endif
