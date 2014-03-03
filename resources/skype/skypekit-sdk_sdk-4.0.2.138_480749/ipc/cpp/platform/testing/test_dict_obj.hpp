#ifndef test_dict_obj_INCLUDED_HPP
#define test_dict_obj_INCLUDED_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "SidPlatform.hpp"

class DictObjTestCase : public CppUnit::TestCase
{

CPPUNIT_TEST_SUITE(DictObjTestCase);
CPPUNIT_TEST(createTest);
CPPUNIT_TEST(insertTest);
CPPUNIT_TEST(keysTest);
CPPUNIT_TEST(valueTest);
CPPUNIT_TEST(removeTest);
CPPUNIT_TEST_SUITE_END();

protected:

	void createTest(); 
        void insertTest();
        void keysTest();
        void valueTest();
        void removeTest();       
};



#endif 
