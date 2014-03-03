
#if !defined(_WIN32)
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "skype-thread-impl.h"

#ifndef THREAD_STACK_SIZE
#if defined(SYMBIAN)
        #define THREAD_STACK_SIZE 32*1024
#else
        #define THREAD_STACK_SIZE 125*1024
#endif
#endif

void FATAL(const char *err)
{
        perror(err);
        abort();
}

///////////////////////////////////////////////////////////////////////////////

SEMutexImpl::SEMutexImpl(void)
{
        // Pthread init
        pthread_mutexattr_t attr;
        if (pthread_mutexattr_init(&attr))
                FATAL("pthread_mutexattr_init() failed");
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&Mtx, &attr);
        pthread_mutexattr_destroy(&attr);
}
SEMutexImpl::~SEMutexImpl(void)
{
        pthread_mutex_destroy(&Mtx);
}
pthread_mutex_t * SEMutexImpl::Get()
{
        return &Mtx;
}

bool SEMutexImpl::AcquireImpl(void /* sint milliseconds */)
{
        if (pthread_mutex_lock(&Mtx))
                FATAL("pthread_mutex_lock() failed");

        return true;
}

void SEMutexImpl::ReleaseImpl(void)
{
        pthread_mutex_unlock(&Mtx);
}

bool SEMutexImpl::TryAcquireImpl(void)
{
        if (pthread_mutex_trylock(&Mtx))
                return false;

        return true;
}

///////////////////////////////////////////////////////////////////////////////

SESyncEventImpl::SESyncEventImpl(void) : wait(true)
{
        if (pthread_cond_init(&cond, NULL))
                FATAL("pthread_cond_init() failed");

        if (pthread_mutex_init(&mutex, NULL))
                FATAL("pthread_mutex_init() failed");
}

SESyncEventImpl::~SESyncEventImpl(void)
{
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
}

bool SESyncEventImpl::WaitImpl(int milliseconds)
{
        int rc = 0;
        if (pthread_mutex_lock(&mutex) != 0)
                FATAL("pthread_mutex_lock() failed");

        while (wait) {
                if (milliseconds < 0) {
                        if (pthread_cond_wait(&cond, &mutex)) {
                                pthread_mutex_unlock(&mutex);
                                FATAL("pthread_cond_wait() failed");
                        }
                } else {
                        timespec ts = makeTimespec(milliseconds);
                        if ((rc = pthread_cond_timedwait(&cond, &mutex, &ts))) {
                                if (rc == ETIMEDOUT) break;
                                pthread_mutex_unlock(&mutex);
                                FATAL("pthread_cond_timedwait() failed");
                        }
                }
        }

        if (rc == 0)
                wait = true;
        pthread_mutex_unlock(&mutex);
        return rc == 0;
}

void SESyncEventImpl::TriggerImpl(void)
{
        if (pthread_mutex_lock(&mutex) != 0)
                FATAL("pthread_mutex_lock() failed");

        wait = false;

        if (pthread_cond_broadcast(&cond)) {
                pthread_mutex_unlock(&mutex);
                FATAL("pthread_cond_broadcast() failed");
        }

        pthread_mutex_unlock(&mutex);
}

timespec SESyncEventImpl::makeTimespec(int milliseconds)
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

///////////////////////////////////////////////////////////////////////////////
SEThreadImpl::SEThreadHolder SEThreadImpl::threadHolder;

SEThreadImpl::SEThreadImpl() :
        Shutdown(false),
        Running(false), 
        Joined(true), 
        thread_id(0)
{
}

SEThreadImpl::~SEThreadImpl(void)
{
        StopImpl();
}

void *SEThreadImpl::thread_func(void * const data)
{
        SEThreadImpl * const th = (SEThreadImpl *) data;
        threadHolder.SetThread(th);

        th->Running = true;
        th->Run();
        th->Running = false;
        return NULL;
}

void SEThreadImpl::StartImpl()
{
        Shutdown = false;
        Running = true;
        Joined = false;

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);

        if (pthread_create(&thread_id, &attr, thread_func, this))
                FATAL("pthread_create() failed");

        pthread_attr_destroy(&attr);
}

void SEThreadImpl::StopImpl(void)
{
        if (!Joined) {
                if (thread_id) {  
                        pthread_join(thread_id, NULL); 
                        Joined = true;
                }
        }
        
        Shutdown = true;
        Running = false;
}

void SEThreadImpl::SleepImpl(unsigned int milliseconds)
{
        timespec ts;
        ts.tv_sec = (time_t) (milliseconds / 1000);
        ts.tv_nsec = (time_t) ((milliseconds % 1000) * 1000 * 1000);
        nanosleep(&ts, NULL);
}

#endif
