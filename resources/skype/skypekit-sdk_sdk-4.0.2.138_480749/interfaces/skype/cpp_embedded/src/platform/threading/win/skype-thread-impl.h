#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>

struct SEMutexImpl {
public:
        SEMutexImpl();
        ~SEMutexImpl();
        bool AcquireImpl(void);
        void ReleaseImpl(void);
        bool TryAcquireImpl(void);
private:
        CRITICAL_SECTION criticalSection;
};

struct SESyncEventImpl {
public:
        SESyncEventImpl();
        ~SESyncEventImpl();
        bool WaitImpl(int milliseconds);
        void TriggerImpl(void);
private:
        HANDLE _syncEvent;
};

struct SEThreadImpl {
private:
        DWORD thread_id;
        HANDLE _thread;
public:
        SEThreadImpl() : thread_id(0), _thread(0) {}
        virtual ~SEThreadImpl();
        virtual void Run(void) = 0;
        static DWORD WINAPI thread_func( LPVOID pthis);
        void StartImpl();
        virtual void StopImpl();
        static void SleepImpl(unsigned int milliseconds);
        bool isRunningImpl() const;

        static SEThreadImpl* CurrentImpl() {
                return threadHolder.GetThread();
        }

        struct SEThreadHolder {
                SEThreadHolder() {
                        tls_index = TlsAlloc();
                }
                ~SEThreadHolder() {
                        TlsFree(tls_index);
                }
                SEThreadImpl* GetThread() const {
                        return reinterpret_cast<SEThreadImpl*>(TlsGetValue(tls_index));
                }
                void SetThread(SEThreadImpl* thread) {
                        TlsSetValue(tls_index, thread);
                }
private:
                DWORD tls_index;
        };

        static SEThreadHolder threadHolder;
};

#endif

