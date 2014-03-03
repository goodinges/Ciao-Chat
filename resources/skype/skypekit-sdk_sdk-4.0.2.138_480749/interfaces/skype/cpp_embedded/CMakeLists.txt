cmake_minimum_required(VERSION 2.8)

project(skypekitclient)

# we include OpenGl support files for older versions of cmake.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)

set(USE_OPENGL FALSE)
if (NO_VIDEO)
   message(STATUS "Video calling not supported")
else()
    find_package(OpenGL)

    if (OPENGL_FOUND)
        if (NOT WIN32)
            find_package(GLUT)
            if (GLUT_FOUND)
                message(STATUS "OpenGL and GLUT found, video calling supported.")
                set(USE_OPENGL TRUE)
            else()
                message(WARNING "GLUT - OpenGL Utility Toolkit - not found, video calling not supported.")
            endif()
        else(NOT WIN32)
            message(STATUS "OpenGL found, video calling supported.")
            set(USE_OPENGL TRUE)
        endif()
    else()
        message(WARNING "OpenGL not found, video calling not supported.")
    endif()
endif()

if (WIN32)
    add_definitions(-D_WIN32 -DWIN32 -D__WIN32 -D_CRT_SECURE_NO_DEPRECATE)

    if ((ACTION MATCHES "vs2005") OR (ACTION MATCHES "vs2008")) 
        add_definitions("/wd4100" "/W2") #turn off excess "unused parameter" warnings
    endif()
  
    if (ACTION MATCHES "gmake")
        add_definitions ( -Wshadow -Wconversion )
    endif() 
endif()

set(CMAKE_OSX_ARCHITECTURES i386)

set(CPPWRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPPWRAPPER_INSTALL_DIR ${CPPWRAPPER_DIR}/build/)

set(SID_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
set(SID_IPC_DIR ${SID_DIR}/ipc/cpp/)
set(SID_PLATFORM_DIR ${SID_IPC_DIR}/platform/se)


# build skypekit version of the cyassl library
ADD_SUBDIRECTORY(src/ssl)

# build main cpp interface library
ADD_SUBDIRECTORY(src/api)

if (USE_OPENGL)
    # build reusable opengl library
    ADD_SUBDIRECTORY(src/platform/opengl)
endif()

# build command line skypekitclient
ADD_SUBDIRECTORY(src/client)
