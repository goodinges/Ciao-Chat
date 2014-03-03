
#include "test_string_dict.hpp"
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestStringDict);

class Object
{
public:
        Object(Sid::String id, void* root) : object_id(id) {}
        Sid::String object_id;
};

Object obj_array[10] =
{
        Object("o1",0),
        Object("o2",0),
        Object("o3",0),
        Object("o4",0),
        Object("o5",0),
        Object("o6",0),
        Object("o7",0),
        Object("o8",0),
        Object("o9",0),
        Object("o10",0)
};

const uint array_len = sizeof(obj_array) / sizeof(Object);

void TestStringDict::createTest()
{
	Sid::Dict_ptr_str test1;
	Sid::Dict_ptr_str test2(test1);

	CPPUNIT_ASSERT_EQUAL(0, (int)test1.size());
	CPPUNIT_ASSERT_EQUAL(0, (int)test2.size());
}

void TestStringDict::insertTest()
{
        Sid::Dict_ptr_str test1;
        for(int i = 0; i < array_len; i++) {
                test1.insert(obj_array[i].object_id, (void*)&obj_array[i]);
        }

        CPPUNIT_ASSERT_EQUAL((int)test1.size(), (int)array_len);
}

void TestStringDict::keysTest()
{
        Sid::Dict_ptr_str test1;
        for(int i = 0; i < array_len; i++) {
                test1.insert(obj_array[i].object_id, (void*)&obj_array[i]);
        }

        Sid::List_String keys = test1.keys();
        for(int i = 0; i < array_len; i++) {
                bool found = false;
                for(int j = 0; j < keys.size(); j++) {
                        if (obj_array[i].object_id == keys[j]) {
                                found = true;
                                break;
                        }
                }
                CPPUNIT_ASSERT(found);
        }
}

void TestStringDict::valueTest()
{
        Sid::Dict_ptr_str test1;
        for(int i = 0; i < array_len; i++) {
                test1.insert(obj_array[i].object_id, (void*)&obj_array[i]);
        }

        Sid::List_String keys = test1.keys();
        for(int j = 0; j < keys.size(); j++) {
                Sid::String key = keys[j];
                Object* obj = (Object*)test1.value(key);

                CPPUNIT_ASSERT(obj->object_id == key);
        }
}

void TestStringDict::emptyInsertTest()
{
        Sid::Dict_ptr_str test1;
        Sid::String empty;
        test1.insert(empty, 0);

        CPPUNIT_ASSERT_EQUAL((int)test1.size(), 0);
}

void TestStringDict::removeTest()
{
//         Sid::Dict_ptr_str test1;
//         for(int i = 0; i < array_len; i++) {
//                 test1.insert(obj_array[i].object_id, (void*)&obj_array[i]);
//         }
// 
//         for(int j = array_len-1; j >= 0; j--) {
//                 test1.remove(obj_array[j].object_id);
//                 Sid::List_String keys = test1.keys();
// 
//                 CPPUNIT_ASSERT_EQUAL((int)test1.size(), j);
//                 CPPUNIT_ASSERT(test1.value(obj_array[j].object_id) == 0);
//                 CPPUNIT_ASSERT_EQUAL((int)keys.size(), j);
// 
//                 bool found = false;
//                 for(int k = 0; k < keys.size(); k++) {
//                         if (obj_array[j].object_id == keys[k]) {
//                                 found = true;
//                                 break;
//                         }
//                 }
// 
//                 CPPUNIT_ASSERT(!found);
//         }
}




