include(FindThreads)

add_executable(loginmodel-test test-loginservice.cpp)

target_link_libraries(loginmodel-test
                        PocoFoundation
                        PocoData
                        Mvp-model
                        Mvp-presenter
                        ${GOOGLEMOCK_LIBRARIES}
                        libgtest.a
                        ${skt_libs}
                        ${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(loginmodel-test ""  test-loginservice.cpp)

add_executable(contactlistmodel-test test-contactlistservice.cpp)

target_link_libraries(contactlistmodel-test
                        PocoFoundation
                        PocoData
                        Mvp-model
                        ${GOOGLEMOCK_LIBRARIES}
                        libgtest.a
                        ${skt_libs}
                        ${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(contactlistmodel-test "" test-contactlistservice.cpp)
