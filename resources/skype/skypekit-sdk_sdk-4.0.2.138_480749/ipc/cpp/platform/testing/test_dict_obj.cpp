
#include "test_dict_obj.hpp"
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(DictObjTestCase);

class Object
{
public:
        Object(unsigned int oid, void* root) : object_id(oid) {}
        int object_id;
}; 

Object array[10] =
{
        Object(1,0),
        Object(2,0),
        Object(3,0),
        Object(4,0),
        Object(5,0),
        Object(6,0),
        Object(7,0),
        Object(8,0),
        Object(9,0),
        Object(10,0)
};

const uint array_len = sizeof(array) / sizeof(Object);

void DictObjTestCase::createTest()
{
	Sid::Dict_ptr test1; 
	Sid::Dict_ptr test2(test1); 
	
	CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
	CPPUNIT_ASSERT_EQUAL(0, (int)test2.size());
}

void DictObjTestCase::insertTest()
{
        Sid::Dict_ptr test1;
        for(int i = 0; i < array_len; i++) { 
                test1.insert(array[i].object_id, (void*)&array[i]); 
        } 
        
        CPPUNIT_ASSERT_EQUAL((int)test1.size(), (int)array_len);
}

void DictObjTestCase::keysTest()
{
        Sid::Dict_ptr test1;
        for(int i = 0; i < array_len; i++) { 
                test1.insert(array[i].object_id, (void*)&array[i]); 
        } 
        
        Sid::List_int keys = test1.keys();
        for(int i = 0; i < array_len; i++) { 
                bool found = false; 
                for(int j = 0; j < keys.size(); j++) {  
                        if (array[i].object_id == keys[j]) {
                                found = true; 
                                break;
                        }
                } 
                CPPUNIT_ASSERT(found);                
        }        
}

void DictObjTestCase::valueTest()
{
        Sid::Dict_ptr test1;
        for(int i = 0; i < array_len; i++) { 
                test1.insert(array[i].object_id, (void*)&array[i]); 
        } 
        
        Sid::List_int keys = test1.keys();
        for(int j = 0; j < keys.size(); j++) {
                int key = keys[j];
                Object* obj = (Object*)test1.value(key);
                
                CPPUNIT_ASSERT_EQUAL(obj->object_id, key);
        }     
}

void DictObjTestCase::removeTest()
{
        Sid::Dict_ptr test1;
        for(int i = 0; i < array_len; i++) { 
                test1.insert(array[i].object_id, (void*)&array[i]); 
        } 
        
        for(int j = array_len-1; j >= 0; j--) {        
                test1.remove(array[j].object_id);
                Sid::List_int keys = test1.keys();
                
                CPPUNIT_ASSERT_EQUAL((int)test1.size(), j);  
                CPPUNIT_ASSERT(test1.value(array[j].object_id) == 0); 
                CPPUNIT_ASSERT_EQUAL((int)keys.size(), j);                 
          
                bool found = false; 
                for(int k = 0; k < keys.size(); k++) {
                        if (array[j].object_id == keys[k]) {
                                found = true; 
                                break;
                        }                      
                }
                
                CPPUNIT_ASSERT(!found);
        }        
}




