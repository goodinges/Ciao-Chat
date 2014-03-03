#ifndef TEST_STRING_DICT_HPP_
#define TEST_STRING_DICT_HPP_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class TestStringDict : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(TestStringDict);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(insertTest);
CPPUNIT_TEST(keysTest);
CPPUNIT_TEST(valueTest);
CPPUNIT_TEST(emptyInsertTest);
CPPUNIT_TEST(removeTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest();
        void insertTest();
        void keysTest();
        void valueTest();
        void emptyInsertTest();
        void removeTest();
};

#endif /* TEST_STRING_DICT_HPP_ */
