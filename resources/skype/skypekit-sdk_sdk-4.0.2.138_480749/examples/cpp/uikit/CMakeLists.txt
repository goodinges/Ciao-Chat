# cpack is better with 2.8.3 (for key fixes to fixup_bundle)
cmake_minimum_required(VERSION 2.8.3)

PROJECT(skypeuikit)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)

FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtXml)
IF(QT4_FOUND)
    # configure include path, libraries, and much more
    INCLUDE(${QT_USE_FILE})
ELSE(QT4_FOUND)
	MESSAGE(WARNING "Qt4 libraries not found - building model and presenter libraries only.")
ENDIF(QT4_FOUND)

FIND_PACKAGE(GTest)
FIND_PACKAGE(Googlemock)
IF(GTEST_FOUND)
    ENABLE_TESTING(TRUE)

    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${GOOGLEMOCK_INCLUDE_DIR})
ELSE(GTEST_FOUND)
    MESSAGE(WARNING "Cannot find Google Mocks, unit testing disabled")
ENDIF(GTEST_FOUND)

FIND_PACKAGE(Poco COMPONENTS Foundation XML Data REQUIRED)

FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    ADD_CUSTOM_TARGET(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
)
ENDIF(DOXYGEN_FOUND)

SET(SKT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
SET(SKT_CPP_DIR ${SKT_DIR}/interfaces/skype/cpp_embedded)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src
                    ${Poco_INCLUDE_DIRS})

SET(is-skt32 true)

IF(is-skt32)
	SET(skt_libs 
		skypekit-cppwrapper_2_lib
		skypekit-cyassl_lib)
    IF(WIN32)
        SET(skt_libs ${skt_libs} Ws2_32)
    ELSE(WIN32)
        SET(skt_libs ${skt_libs} pthread)
    ENDIF(WIN32)
ELSE(is-skt32)
	SET(skt_libs 
		skypekit-cppwrapper_2_lib)
ENDIF(is-skt32)

LINK_DIRECTORIES(${SKT_CPP_DIR}/build
        ${PROJECT_BINARY_DIR}
        ${Poco_LIBRARY_DIR})

ADD_SUBDIRECTORY(src)

IF(QT4_FOUND)
    ADD_SUBDIRECTORY(qtsimpleclient)
ENDIF(QT4_FOUND)
