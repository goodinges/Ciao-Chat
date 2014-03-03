#if !defined(_WIN32)
#include <pthread.h>

/** \cond INTERNAL */
struct SEMutexImpl {
        SEMutexImpl();
        ~SEMutexImpl();

        pthread_mutex_t Mtx;

        bool AcquireImpl(void);
        void ReleaseImpl(void);
        bool TryAcquireImpl(void);
        pthread_mutex_t * Get();
};

struct SESyncEventImpl {

        SESyncEventImpl();
        ~SESyncEventImpl();

        pthread_cond_t cond;
        pthread_mutex_t mutex;
        volatile bool wait;

        timespec makeTimespec(int milliseconds);

        bool WaitImpl(int milliseconds);
        void TriggerImpl(void);
};

struct SEThreadImpl {
        volatile bool Shutdown, Running, Joined;
        pthread_t thread_id;

        virtual void Run(void) = 0;
        static void *thread_func(void * const data);

        SEThreadImpl();
        virtual ~SEThreadImpl();
        void StartImpl();
        virtual void StopImpl();
        static void SleepImpl(unsigned int milliseconds);
        bool isRunningImpl() const {
                return Running;
        }
        static SEThreadImpl* CurrentImpl() {
                return threadHolder.GetThread();
        }

        struct SEThreadHolder {
                SEThreadHolder() {
                        pthread_key_create(&key, NULL);
                }
                ~SEThreadHolder() {
                        pthread_key_delete(key);
                }
                SEThreadImpl* GetThread() const {
                        return reinterpret_cast<SEThreadImpl*>(pthread_getspecific(key));
                }
                void SetThread(SEThreadImpl* thread) {
                        pthread_setspecific(key, thread);
                }
        private:
                pthread_key_t key;
        };

        static SEThreadHolder threadHolder;
};

/** \endcond */
#endif
