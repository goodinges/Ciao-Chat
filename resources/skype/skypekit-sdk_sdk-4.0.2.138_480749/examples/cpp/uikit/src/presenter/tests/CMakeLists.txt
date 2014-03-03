include(FindThreads)

add_executable(loginpresenter-test test-loginpresenter.cpp )

target_link_libraries(loginpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(loginpresenter-test ""  test-loginpresenter.cpp)

add_executable(contactlistpresenter-test test-contactlistpresenter.cpp )

target_link_libraries(contactlistpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(contactlistpresenter-test "" test-contactlistpresenter.cpp)

add_executable(chatpresenter-test test-textchatpresenter.cpp )

target_link_libraries(chatpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(chatpresenter-test "" test-textchatpresenter.cpp)

add_executable(callpresenter-test test-callpresenter.cpp )

target_link_libraries(callpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(callpresenter-test "" test-callpresenter.cpp)

add_executable(convmanagerpresenter-test test-convmanagerpresenter.cpp )

target_link_libraries(convmanagerpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(convmanagerpresenter-test ""  test-convmanagerpresenter.cpp)

add_executable(historypresenter-test test-historypresenter.cpp )

target_link_libraries(historypresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(historypresenter-test ""  test-historypresenter.cpp)

add_executable(ftpresenter-test test-ftpresenter.cpp )

target_link_libraries(ftpresenter-test
	Mvp-presenter
	Mvp-model
	${GOOGLEMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	PocoFoundation
	PocoData	
	${skt_libs}
	${QT_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})

gtest_add_tests(ftpresenter-test ""  test-ftpresenter.cpp)

