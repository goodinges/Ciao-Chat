
#if defined(_WIN32)
#include "skype-thread-impl.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////

SEMutexImpl::SEMutexImpl()
{
        InitializeCriticalSectionAndSpinCount(&criticalSection, 4000);
}

SEMutexImpl::~SEMutexImpl()
{
        DeleteCriticalSection(&criticalSection);
}

bool SEMutexImpl::AcquireImpl(void)
{
		EnterCriticalSection(&criticalSection);
        return true;
}

bool SEMutexImpl::TryAcquireImpl(void)
{
        return TryEnterCriticalSection(&criticalSection) == TRUE;
}

void SEMutexImpl::ReleaseImpl(void)
{
        LeaveCriticalSection(&criticalSection);
}

//////////////////////////////////////////////////////////////////////

SESyncEventImpl::SESyncEventImpl()
{
        _syncEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
}

SESyncEventImpl::~SESyncEventImpl()
{
        CloseHandle(_syncEvent);
}

bool SESyncEventImpl::WaitImpl(int milliseconds)
{
        switch (WaitForSingleObject(_syncEvent, milliseconds >= 0 ? milliseconds + 1 : INFINITE)) {
        case WAIT_TIMEOUT:
        case WAIT_OBJECT_0:
                return true;
        }

        return false;
}

void SESyncEventImpl::TriggerImpl(void)
{
        SetEvent(_syncEvent);
}

//////////////////////////////////////////////////////////////////////
SEThreadImpl::SEThreadHolder SEThreadImpl::threadHolder;

SEThreadImpl::~SEThreadImpl()
{
        if (_thread) CloseHandle(_thread);
}

void SEThreadImpl::StartImpl()
{
        _thread = CreateThread(NULL, 0, thread_func, this, 0, &thread_id);
}

void SEThreadImpl::StopImpl()
{
}

void SEThreadImpl::SleepImpl(unsigned int milliseconds)
{
        Sleep(DWORD(milliseconds));
}

bool SEThreadImpl::isRunningImpl() const
{
        if (_thread) {
                DWORD ec = 0;
                return GetExitCodeThread(_thread, &ec) && ec == STILL_ACTIVE;
        }

        return false;
}

DWORD WINAPI SEThreadImpl::thread_func(LPVOID pthis)
{
        SEThreadImpl* th = (SEThreadImpl*)pthis;
        threadHolder.SetThread(th);
        th->Run();

        return 0;
}

#endif









