#ifndef test_string_thread_INCLUDED_HPP
#define test_string_thread_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class ThreadingTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(ThreadingTestCase);
CPPUNIT_TEST(threadTest);
CPPUNIT_TEST(eventTest);
CPPUNIT_TEST(threadsTest);
CPPUNIT_TEST(mutexTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void threadTest(); 
	void eventTest();
	void threadsTest();  
	void mutexTest();  
};


#endif 
