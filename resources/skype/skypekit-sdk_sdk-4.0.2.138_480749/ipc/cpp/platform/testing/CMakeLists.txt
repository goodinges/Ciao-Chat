
set(TARGET sid_types_test)
set(SRCS main.cpp 
    test_binary.cpp
    test_dict_obj.cpp
    test_int_list.cpp
    test_string.cpp
    test_string_list.cpp
    test_thread.cpp
    )

find_package(CppUnit REQUIRED)

set(sid_include ${SID_DIR}/interfaces/skype/cpp_embedded/src)

include_directories(
    ${CPPUNIT_INCLUDE_DIR}
    ${SID_DIR}/ipc/cpp/platform/se
    ${sid_include}/types
    ${sid_include}/platform/threading
    ${sid_include}/platform/threading/pthread
    ${sid_include}/platform/transport
)

add_executable(${TARGET} ${SRCS})

# TODO
# pthread is linux-only
target_link_libraries(${TARGET} ${CPPUNIT_LIBRARY} sidtypes pthread)

if(WIN32)
	set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/Zc:wchar_t- /vmg")
endif(WIN32)

#GTEST_ADD_TESTS(${TARGET} ""  ${SRCS})
