
#include "test_string_list.hpp"
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(StringListTestCase);

const Sid::String array[5] = { "1", "2", "3", "4", "5" };
const uint array_len = sizeof(array) / sizeof(Sid::String); 

void StringListTestCase::createTest()
{
	Sid::List_String test1; 
	Sid::List_String test2(test1); 
	
	CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
	CPPUNIT_ASSERT_EQUAL(0, (int)test2.size());
}

void StringListTestCase::appendTest()
{
	Sid::List_String test1;
	for(int i = 0; i < array_len; i++) { test1.append(array[i]); }
	
	CPPUNIT_ASSERT_EQUAL(array_len, test1.size());
	for(int i = 0; i < array_len; i++) { CPPUNIT_ASSERT_EQUAL(array[i], test1[i]); }
}

void StringListTestCase::appendEmptyStringTest()
{
	Sid::List_String test1;
	Sid::String string;
	CPPUNIT_ASSERT(string.isNull());

	test1.append(string);

	CPPUNIT_ASSERT(test1.size() == 1);
	CPPUNIT_ASSERT(!test1[0].isNull());
	CPPUNIT_ASSERT(test1[0] == "");
}

void StringListTestCase::operatorTest()
{
	Sid::List_String test1;
	for(int i = 0; i < array_len; i++) { test1.append(array[i]); }
	Sid::List_String test2;
	for(int i = 0; i < array_len; i++) { test2.append(array[i]); }	
	
// 	CPPUNIT_ASSERT(test1 == test2);
// 	CPPUNIT_ASSERT(!(test1 != test2));
		
	for(int i = 0; i < array_len; i++) { CPPUNIT_ASSERT_EQUAL(test2[i], test1[i]); }		
}

void StringListTestCase::assignTest()
{
	Sid::List_String test1;
	for(int i = 0; i < array_len; i++) { test1.append(array[i]); }
	Sid::List_String test2 = test1;
	
	CPPUNIT_ASSERT_EQUAL(test2.size(), test1.size());
	for(int i = 0; i < array_len; i++) { CPPUNIT_ASSERT_EQUAL(test2[i], test1[i]); }
}
