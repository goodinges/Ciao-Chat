#ifndef test_string_INCLUDED_HPP
#define test_string_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class StringTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(StringTestCase);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(assignTest);
CPPUNIT_TEST(operatorTest);
CPPUNIT_TEST(getBufTest);
CPPUNIT_TEST(appendTest);
CPPUNIT_TEST(getSizeTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest(); 
	void assignTest();
	void operatorTest();
	void getBufTest();
	void appendTest();
	void getSizeTest();
};


#endif //test_string_INCLUDED_HPP
