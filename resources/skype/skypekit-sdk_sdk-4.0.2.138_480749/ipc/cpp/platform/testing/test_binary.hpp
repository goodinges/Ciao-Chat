#ifndef test_binary_INCLUDED_HPP
#define test_binary_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class BinaryTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(BinaryTestCase);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(operatorTest);
CPPUNIT_TEST(setTest);
CPPUNIT_TEST(resizeTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest(); 
	void operatorTest();
	void setTest();
	void resizeTest();
};

#endif 
