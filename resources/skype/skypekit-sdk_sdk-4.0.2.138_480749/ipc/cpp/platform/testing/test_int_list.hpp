#ifndef test_int_list_INCLUDED_HPP
#define test_int_list_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class IntListTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(IntListTestCase);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(appendTest);
CPPUNIT_TEST(operatorTest);
CPPUNIT_TEST(assignTest);
CPPUNIT_TEST(containsTest);
CPPUNIT_TEST(clearTest);
CPPUNIT_TEST(pointerTest);
CPPUNIT_TEST(resizeTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest(); 
	void operatorTest();
	void assignTest();
	void appendTest();
	void containsTest();
        void clearTest();	
        void pointerTest(); 
        void resizeTest(); 
};



#endif 
