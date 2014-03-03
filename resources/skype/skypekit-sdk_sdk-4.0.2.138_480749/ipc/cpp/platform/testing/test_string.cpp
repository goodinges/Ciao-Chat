
#include "test_string.hpp"

#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(StringTestCase);
const char* temp = "test12345";

void StringTestCase::createTest()
{	
	Sid::String test1(temp); 
	Sid::String test2(test1); 
	
	CPPUNIT_ASSERT(strcmp((const char*)test1, temp)  == 0);
	CPPUNIT_ASSERT(strcmp((const char*)test1, (const char*)test2)  == 0);
}

void StringTestCase::assignTest()
{
	Sid::String test1 = temp; 
	Sid::String test2 = test1; 
	Sid::String test3 = "test12345"; 
	
	CPPUNIT_ASSERT(strcmp((const char*)test1, temp) == 0);
	CPPUNIT_ASSERT(strcmp((const char*)test1, (const char*)test2) == 0);
	CPPUNIT_ASSERT(strcmp((const char*)test1, (const char*)test3) == 0);	
}

void StringTestCase::operatorTest()
{
	Sid::String test1 = temp;
	Sid::String test2 = temp;
	
	CPPUNIT_ASSERT(test1 == test2);
	CPPUNIT_ASSERT(!(test1 != test2));
	CPPUNIT_ASSERT(strcmp((const char*)test1, temp)  == 0);
}

void StringTestCase::getBufTest()
{
	Sid::String test1 = temp; 
	Sid::String *test2 = &test1;
	const char* temp2 = (const char*)*test2; 
	
	CPPUNIT_ASSERT(strcmp(temp2, temp) == 0);
	CPPUNIT_ASSERT(strcmp((const char*)test1, temp) == 0);
}

void StringTestCase::appendTest()
{
	Sid::String test1;
	test1 += temp; 
	
	CPPUNIT_ASSERT(strcmp((const char*)test1, temp)  == 0);
}

void StringTestCase::getSizeTest()
{
	Sid::String test1 = temp; 
	CPPUNIT_ASSERT_EQUAL((int)test1.size(), (int)strlen(temp));
}