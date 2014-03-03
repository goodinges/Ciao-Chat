
#include "test_int_list.hpp"
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(IntListTestCase);

const int array[10] = { 19, 28, 37, 46, 55, 64, 73, 82, 91, 100 };
const uint array_len = sizeof(array) / sizeof(uint); 

void IntListTestCase::createTest()
{
	Sid::List_int test1; 
	Sid::List_int test2(test1); 
	
	CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
	CPPUNIT_ASSERT_EQUAL(0, (int)test2.size());
}

void IntListTestCase::appendTest()
{
	Sid::List_int test1;
	for(int i = 0; i < array_len; i++) { 
                test1.append(array[i]); 
        }
	
	CPPUNIT_ASSERT_EQUAL(array_len, (uint)test1.size());
        
	for(int i = 0; i < array_len; i++) { 
               CPPUNIT_ASSERT_EQUAL(array[i], test1[i]); 
        }
}

void IntListTestCase::operatorTest()
{
	Sid::List_int test1;
        Sid::List_int test2;	
        
        for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
               test2.append(array[i]);
        }
	
// 	CPPUNIT_ASSERT(test1 == test2);
// 	CPPUNIT_ASSERT(!(test1 != test2));	
}

void IntListTestCase::assignTest()
{
	Sid::List_int test1;
	for(int i = 0; i < array_len; i++) { test1.append(array[i]); }
	Sid::List_int test2 = test1;
	
	CPPUNIT_ASSERT_EQUAL(test2.size(), test1.size());
	for(int i = 0; i < array_len; i++) { CPPUNIT_ASSERT_EQUAL(array[i], test1[i]); }
}

void IntListTestCase::containsTest()
{
	Sid::List_int test1;
	for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
        }
	
	CPPUNIT_ASSERT(test1.contains(array[0]));
	CPPUNIT_ASSERT(test1.contains(array[2]));
	CPPUNIT_ASSERT(!test1.contains(123));
}

bool field_equals_default(const void* p) { return reinterpret_cast<const Sid::List_int*>(p)->size()==0; }
void set_field_to_default(void* p) { reinterpret_cast<Sid::List_int*>(p)->clear(); }
void append(void* list, uint size, void*& elem) {
        Sid::List_int* l=reinterpret_cast<Sid::List_int*>(list);
        l->append(int());
        elem = (void*) &(((const Sid::List_int*)l)->operator[](size));
};
void* iterate(void* list, uint count) {
        Sid::List_int* l=reinterpret_cast<Sid::List_int*>(list);
        if (count >= (uint)l->size()) return 0;
        return (void*) &(((const Sid::List_int*)l)->operator[](count));
};

void IntListTestCase::pointerTest()
{
	Sid::List_int test1;
        
        CPPUNIT_ASSERT(field_equals_default((void*)&test1));
	
        for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
        }
               
        set_field_to_default((void*)&test1);         
        CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
        
        int* element;
        append((void*)&test1, 0, (void*&)element); 
        *(element) = 1234;  
        append((void*)&test1, 1, (void*&)element); 
        *(element) = 4321; 
                
        CPPUNIT_ASSERT_EQUAL(4321, test1[1]); 
        
        (void*&)element = iterate((void*)&test1, 0);
        
        CPPUNIT_ASSERT_EQUAL(1234, *element);       
}

void IntListTestCase::clearTest()
{
        Sid::List_int test1;
        for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
        }
                
        test1.clear();
        
        CPPUNIT_ASSERT(!test1.contains(array[2]));
        CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
}

void IntListTestCase::resizeTest()
{
        Sid::List_int test1;
        for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
        }
        
        test1.resize(0); 
        CPPUNIT_ASSERT(!test1.contains(array[2]));
        CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());    
            
        for(int i = 0; i < array_len; i++) { 
               test1.append(array[i]); 
        }   
        CPPUNIT_ASSERT_EQUAL(array_len, (uint)test1.size());
        for(int i = 0; i < array_len; i++) { 
               CPPUNIT_ASSERT_EQUAL(array[i], test1[i]); 
        }    
            
        test1.resize(array_len*2);     
        CPPUNIT_ASSERT_EQUAL(array_len*2, (uint)test1.size());
        for(int i = 0; i < array_len; i++) { 
               CPPUNIT_ASSERT_EQUAL(array[i], test1[i]); 
        }    
        for(int i = array_len; i < array_len*2; i++) { 
               CPPUNIT_ASSERT_EQUAL(0, test1[i]); 
        }         
}





