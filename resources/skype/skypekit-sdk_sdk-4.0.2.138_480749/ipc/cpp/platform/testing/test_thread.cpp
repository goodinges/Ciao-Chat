
#include "test_thread.hpp"
#include <string.h>
#include <unistd.h>

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadingTestCase);

class TestThread : public Sid::Thread
{
public:
	TestThread(const char* tname = 0) : Sid::Thread(tname), 
		ran(false) {
		}
	
	int WaitForCompletion(int timeout)
	{
		for(int i = 0; i < timeout; i++)
		{
			if(!IsRunning()) return 1;
			Sid::Thread::sleep(500);
		}	
		
		return 0;
	}
	
	bool Ran() 
	{
		return ran; 
	}
	
protected:
	bool ran;
private:	
	virtual void Run() 
	{
                for(int i = 0; i < 3; i++)
                {
                        Sid::Thread::sleep(1000);
                }
                        
		ran = true; 	
	}		
};

class TestEventThread : public TestThread
{
public:
	TestEventThread(const char* tname = 0) : TestThread(tname) 
        {                   
	}
	
	void Continue()
	{       
		event.notify();     
                Sid::Thread::sleep(50);                      
	}	
         
private:
	Sid::Event event; 
	void Run() 
	{                   
		event.wait();
                ran = true;               	                            
	}
			
};

class TestMutexThread : public TestThread
{
public:
	TestMutexThread(Sid::Mutex* m, const char* tname = 0) : 
		TestThread(tname), 
		mutex(m) {
		}
                
private:
	Sid::Mutex* mutex; 
	void Run() 
	{
		mutex->lock();
                ran = true;              
		mutex->unlock(); 
	}
			
};

void ThreadingTestCase::threadTest()
{
	TestThread thread1; 

	CPPUNIT_ASSERT(!thread1.IsRunning());
	
	thread1.start("test::thread"); 
	
	CPPUNIT_ASSERT(strcmp("test::thread", thread1.get_name()) == 0);
        
        Sid::Thread::sleep(1000);        
	CPPUNIT_ASSERT(thread1.IsRunning());
	
        CPPUNIT_ASSERT(thread1.WaitForCompletion(20));
	CPPUNIT_ASSERT(thread1.Ran());
	CPPUNIT_ASSERT(!thread1.IsRunning());
}

void ThreadingTestCase::eventTest()
{
	TestEventThread thread1; 
	thread1.start("test::thread");
         
        Sid::Thread::sleep(1000);       
        CPPUNIT_ASSERT(thread1.IsRunning()); 
        
        Sid::Thread::sleep(1000);       
        CPPUNIT_ASSERT(thread1.IsRunning()); 
	
	thread1.Continue(); 
	
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(thread1.Ran());
}

void ThreadingTestCase::mutexTest()
{
	Sid::Mutex mutex; 
	TestMutexThread thread1(&mutex); 
	
	mutex.lock(); 
	thread1.start("test::thread"); 
	
        Sid::Thread::sleep(1000); 	
	CPPUNIT_ASSERT(thread1.IsRunning()); 
        
        Sid::Thread::sleep(1000);       
        CPPUNIT_ASSERT(thread1.IsRunning());      
        
	mutex.unlock(); 

	CPPUNIT_ASSERT(thread1.WaitForCompletion(10));
	CPPUNIT_ASSERT(thread1.Ran());
}

void ThreadingTestCase::threadsTest()
{
	TestEventThread thread1("thread1"); 
	TestEventThread thread2("thread2"); 
	TestEventThread thread3("thread3"); 
	TestEventThread thread4("thread4"); 
        
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(!thread2.IsRunning());
	CPPUNIT_ASSERT(!thread3.IsRunning());
	CPPUNIT_ASSERT(!thread4.IsRunning());
		            
	thread1.start(); 
	
	CPPUNIT_ASSERT(thread1.IsRunning());
	CPPUNIT_ASSERT(!thread2.IsRunning());
	CPPUNIT_ASSERT(!thread3.IsRunning());
	CPPUNIT_ASSERT(!thread4.IsRunning());
	     
	thread2.start(); 
	
	CPPUNIT_ASSERT(thread1.IsRunning());
	CPPUNIT_ASSERT(thread2.IsRunning());
	CPPUNIT_ASSERT(!thread3.IsRunning());
	CPPUNIT_ASSERT(!thread4.IsRunning());
	   
	thread3.start(); 
	
	CPPUNIT_ASSERT(thread1.IsRunning());
	CPPUNIT_ASSERT(thread2.IsRunning());
	CPPUNIT_ASSERT(thread3.IsRunning());
	CPPUNIT_ASSERT(!thread4.IsRunning());
     
	thread4.start(); 
	
	CPPUNIT_ASSERT(thread1.IsRunning());
	CPPUNIT_ASSERT(thread2.IsRunning());
	CPPUNIT_ASSERT(thread3.IsRunning());
	CPPUNIT_ASSERT(thread4.IsRunning());	
	
        Sid::Thread::sleep(1000);
                     
	thread1.Continue(); 
        
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(thread2.IsRunning());
	CPPUNIT_ASSERT(thread3.IsRunning());
	CPPUNIT_ASSERT(thread4.IsRunning());	
	
	thread2.Continue(); 
	
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(!thread2.IsRunning());
	CPPUNIT_ASSERT(thread3.IsRunning());
	CPPUNIT_ASSERT(thread4.IsRunning());
		
	thread3.Continue(); 
	
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(!thread2.IsRunning());
	CPPUNIT_ASSERT(!thread3.IsRunning());
	CPPUNIT_ASSERT(thread4.IsRunning());	
		
	thread4.Continue(); 
	
	CPPUNIT_ASSERT(!thread1.IsRunning());
	CPPUNIT_ASSERT(!thread2.IsRunning());
	CPPUNIT_ASSERT(!thread3.IsRunning());
	CPPUNIT_ASSERT(!thread4.IsRunning());	
		
	CPPUNIT_ASSERT(thread1.Ran());
	CPPUNIT_ASSERT(thread2.Ran());
	CPPUNIT_ASSERT(thread3.Ran());
	CPPUNIT_ASSERT(thread4.Ran());	
	
	CPPUNIT_ASSERT(strcmp("thread1", thread1.get_name()) == 0);
	CPPUNIT_ASSERT(strcmp("thread2", thread2.get_name()) == 0);
	CPPUNIT_ASSERT(strcmp("thread3", thread3.get_name()) == 0);
	CPPUNIT_ASSERT(strcmp("thread4", thread4.get_name()) == 0);
}
