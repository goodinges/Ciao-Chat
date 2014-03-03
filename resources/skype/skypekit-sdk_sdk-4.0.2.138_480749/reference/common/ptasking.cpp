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

#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ptasking.hpp"

bool pMutex::Acquire(void /* sint milliseconds */)
{
	if (pthread_mutex_lock(&Mtx))
		FATAL("pthread_mutex_lock() failed (%s)", strerror(errno));

	return 1;
}

void pMutex::Release(void)
{
	pthread_mutex_unlock(&Mtx);
}

pSyncEvent::pSyncEvent(void)
{
	if (pthread_cond_init(&cond, NULL))
		FATAL("pthread_cond_init() failed (%s)", strerror(errno));
}

pSyncEvent::~pSyncEvent(void)
{
	pthread_cond_destroy(&cond);
}

bool pSyncEvent::Wait(int milliseconds)
{
	pMutexLock rm(internal_mutex);
	return Wait(internal_mutex, milliseconds);
}

bool pSyncEvent::Wait(pMutex &mutex, int milliseconds)
{

	int rc;

	if (milliseconds < 0) {
		rc = pthread_cond_wait(&cond, mutex.Get());
	} else {
		do {
			timespec ts = makeTimespec(milliseconds);
			rc = pthread_cond_timedwait(&cond, mutex.Get(), &ts);
		} while (rc == EINTR);
	}

	if (rc == ETIMEDOUT)
		return 0;
	if (rc)
		FATAL("pthread_cond_wait()/pthread_cond_timedwait() failed (%s)", strerror(errno));
	return 1;
}

void pSyncEvent::Trigger(void)
{
	Trigger(internal_mutex);
}

void pSyncEvent::Trigger(pMutex &mutex)
{
	pMutexLock rm(mutex);
	pthread_cond_signal(&cond);
}

timespec pSyncEvent::makeTimespec(int milliseconds)
{
	timeval tv;
	gettimeofday(&tv, NULL);
	timespec ts;
	ts.tv_sec = tv.tv_sec + milliseconds / 1000;
	ts.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000 * 1000;
	if (ts.tv_nsec >= 1000 * 1000 * 1000) {
		ts.tv_nsec -= 1000 * 1000 * 1000;
		ts.tv_sec++;
	}
	return ts;
}

pThread::pThread(bool detached, int stackSize) :
	Shutdown(0),
	Running(0),
	Detached(detached),
	StackSize(stackSize)
{
}

pThread::~pThread(void)
{
	Stop();
}

void *pThread::thread_func(void * const data)
{
	pThread * const th = (pThread *) data;
	if (th->Detached) {
#ifndef ANDROID
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
#endif
	}
	th->Running = 1;
	th->Run();
	th->Running = 0;
	pthread_exit((void*) data);
	return NULL;
}

void pThread::Start(const char *name)
{
	Shutdown = 0;
	Running = 1;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (StackSize > 0) {
		pthread_attr_setstacksize(&attr, StackSize);
	}

	if (pthread_create(&thread_id, &attr, thread_func, this))
		FATAL("pthread_create() failed (%s)", strerror(errno));

	pthread_attr_destroy(&attr);
	if (Detached) {
		pthread_detach(thread_id);
	}
}

void pThread::Stop(void)
{
	Shutdown = 1;
	if (Running) {
#ifndef ANDROID
		if (Detached) {
			pthread_cancel(thread_id);
		}
#endif
	}
	if (!Detached) {
		pthread_join(thread_id, NULL);
	}
	Running = 0;
}

void pThread::Sleep(unsigned int milliseconds)
{
	timespec ts;
	ts.tv_sec = (time_t) (milliseconds / 1000);
	ts.tv_nsec = (time_t) ((milliseconds % 1000) * 1000 * 1000);
	nanosleep(&ts, NULL);
}
