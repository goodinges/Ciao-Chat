#ifndef test_string_list_INCLUDED_HPP
#define test_string_list_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class StringListTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(StringListTestCase);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(appendTest);
CPPUNIT_TEST(operatorTest);
CPPUNIT_TEST(assignTest);
CPPUNIT_TEST(appendEmptyStringTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest(); 
	void operatorTest();
	void assignTest();
	void appendTest();
	void appendEmptyStringTest();
};

#endif 
