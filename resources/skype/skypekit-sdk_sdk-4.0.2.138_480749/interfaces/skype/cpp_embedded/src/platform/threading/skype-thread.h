
#ifndef _SETHREAD_H_
#define _SETHREAD_H_

#include "skype-thread-impl.h"

/// FastMutex wrapper
struct SEMutex : private SEMutexImpl {
public:
        SEMutex(void) {};
        ~SEMutex(void) {};
        inline bool Acquire(void) {
                return AcquireImpl();
        }
        inline void Release(void) {
                ReleaseImpl();
        }
        inline bool TryAcquire(void) {
                return TryAcquireImpl();
        }
};

struct SEMutexLock {
        SEMutex &mtx;
        SEMutexLock(SEMutex &ref) : mtx(ref) {
                mtx.Acquire();
        }
        ~SEMutexLock() {
                mtx.Release();
        }
private:
        const SEMutexLock & operator=(const SEMutexLock &);
};

/// Syncevent wrapper
struct SESyncEvent : private SESyncEventImpl {
        SEMutex internal_mutex;
public:
        SESyncEvent(void) {};
        ~SESyncEvent(void) {};
        inline bool Wait(int milliseconds = -1) {
                return WaitImpl(milliseconds);
        }
        inline void Trigger(void) {
                TriggerImpl();
        }
};

/// Thread wrapper
struct SEThread : private SEThreadImpl {
        virtual void Run(void)=0;
public:
        SEThread() {};
        virtual ~SEThread(void) {};
        inline void Start() {
                StartImpl();
        }
        inline virtual void Stop() {
                StopImpl();
        }
        inline static void Sleep(unsigned int milliseconds) {
                SleepImpl(milliseconds);
        }
        inline bool IsRunning() const {
                return isRunningImpl();
        }
        static SEThread* Current() {
                return static_cast<SEThread*>(CurrentImpl());
        }
};

#endif
