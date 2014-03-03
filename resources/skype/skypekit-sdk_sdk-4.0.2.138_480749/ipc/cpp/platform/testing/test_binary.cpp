
#include "test_binary.hpp"

#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(BinaryTestCase);

const unsigned char array[129] =
{
    	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    	0x41, 0x41, 0x41, 0x41, 0x0a, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42
};

const unsigned char temp[4] = {0x54, 0x45, 0x53, 0x54}; 
const unsigned char temp2[4] = {0x41, 0x41, 0x41, 0x0a};

void BinaryTestCase::createTest()
{
	Sid::Binary test1((char*)array, sizeof(array));
	Sid::Binary test2(test1);
	Sid::Binary test3; 		 	
                                
	CPPUNIT_ASSERT(!memcmp(test1.data(), (const void *)array, sizeof(array))); 
	CPPUNIT_ASSERT(!memcmp(test1.data(), test2.data(), test2.size()));
        
        CPPUNIT_ASSERT_EQUAL(sizeof(array), test1.size());
        CPPUNIT_ASSERT_EQUAL(sizeof(array), test2.size());
        CPPUNIT_ASSERT_EQUAL(0, (int)test3.size());  
}

void BinaryTestCase::operatorTest()
{	 
	Sid::Binary test1((char*)array, sizeof(array));
	Sid::Binary test2((char*)array, sizeof(array));
        Sid::Binary test4((char*)temp, sizeof(temp));
	Sid::Binary test3 = test1; 
        
	CPPUNIT_ASSERT(test1 == test2);
        CPPUNIT_ASSERT(test1 != test4);
        CPPUNIT_ASSERT(test1 == test3);
        	
	CPPUNIT_ASSERT(!memcmp(test1.data(), test3.data(), test3.size()));      
}

void BinaryTestCase::setTest()
{       
        Sid::Binary test1; 
        
        test1.set((unsigned char *)array, sizeof(array)); 
        CPPUNIT_ASSERT(!memcmp(test1.data(), (const void *)array, sizeof(array))); 
        CPPUNIT_ASSERT_EQUAL(sizeof(array), test1.size());
        
        test1.set((unsigned char*)temp, sizeof(temp)); 
        CPPUNIT_ASSERT(!memcmp(test1.data(), (const void *)temp, sizeof(temp))); 
        CPPUNIT_ASSERT_EQUAL(sizeof(temp), test1.size());
               
        test1.set((unsigned char *)array, sizeof(array)); 
        CPPUNIT_ASSERT(!memcmp(test1.data(), (const void *)array, sizeof(array))); 
        CPPUNIT_ASSERT_EQUAL(sizeof(array), test1.size());   
            
        test1.set_at_offset(sizeof(array) / 2, (const void*)temp, sizeof(temp)); 
        CPPUNIT_ASSERT_EQUAL(sizeof(array), test1.size());
        CPPUNIT_ASSERT(!memcmp(test1.data() + (sizeof(array) / 2), (const void *)temp, sizeof(temp)));     
        CPPUNIT_ASSERT(!memcmp(test1.data(), (const void *)array, sizeof(array) / 2)); 
        CPPUNIT_ASSERT(!memcmp(test1.data() + (sizeof(array) / 2) + sizeof(temp), 
                (const void *)(array + (sizeof(array) / 2) + sizeof(temp)), sizeof(array) / 2) - sizeof(temp)); 
                
        Sid::Binary test2;     
        test2.set((unsigned char *)array, sizeof(array));   
        test2.set_at_offset(sizeof(array), (const void*)temp, sizeof(temp));  
        CPPUNIT_ASSERT_EQUAL(sizeof(array) + sizeof(temp), test2.size());
        CPPUNIT_ASSERT(!memcmp(test2.data() + sizeof(array), (const void *)temp, sizeof(temp))); 
}

void BinaryTestCase::resizeTest()
{
	Sid::Binary test1; 
	test1.resize(sizeof(array)); 
	
	CPPUNIT_ASSERT_EQUAL(sizeof(array), test1.size());
}


