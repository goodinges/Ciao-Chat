
if (USE_OPENGL)
    add_definitions(-DSKYPEKIT_SURFACE_RENDERING)
    add_definitions(-DGL_WINDOW)
    include_directories(${SID_IPC_DIR}/VideoBuffers)
    
    list (APPEND SKYPEKIT_LIBRARIES ${OPENGL_LIBRARIES})
    if (GLUT_LIBRARIES)
        list (APPEND SKYPEKIT_LIBRARIES ${GLUT_LIBRARIES})
   endif()
    if (UNIX AND NOT APPLE)
        list (APPEND SKYPEKIT_LIBRARIES "rt")
    endif()

    if (WIN32)
        message(STATUS "Win shared memeory Enabled")
        aux_source_directory(${SID_IPC_DIR}/VideoBuffers/WinShm.* CLIENT_SRCS)
    else()
        message(STATUS "POSIX shared memeory Enabled")
        add_definitions(-DVIDEO_TRANSPORT_POSIX)
        aux_source_directory(${SID_IPC_DIR}/VideoBuffers/PosixShm.* CLIENT_SRCS)
    endif()
    
endif()

include_directories(${CPPWRAPPER_DIR}/src/api
                    ${CPPWRAPPER_DIR}/src/ipc
                    ${CPPWRAPPER_DIR}/src/types
                    ${SID_IPC_DIR}
                    ${SID_PLATFORM_DIR}
                    ${CPPWRAPPER_DIR}/src/platform/threading
                   )

if (ACTION MATCHES "gmake")
    add_definitions(-fno-use-cxa-atexit)
endif()

# threading
if (WIN32)
    include_directories(${CPPWRAPPER_DIR}/src/platform/threading/win)
else()
    include_directories(${CPPWRAPPER_DIR}/src/platform/threading/pthread/)
    if (NOT(TARGET_OS MATCHES "android"))
        list (APPEND SKYPEKIT_LIBRARIES pthread)
    endif()
endif()

if (WIN32)
    list (APPEND SKYPEKIT_LIBRARIES Ws2_32)
endif()

#this has to happen before attaching skypekit_cyassl_lib since skypekit_cppwrapper has link dependancy on skypekit_cyassl_lib
list (APPEND SKYPEKIT_LIBRARIES skypekit-cppwrapper_2_lib)

#ssl-transport
add_definitions(-DSSL_LIB_CYASSL -DNO_FILESYSTEM)

list (APPEND SKYPEKIT_LIBRARIES skypekit-cyassl_lib)

include_directories(${SID_IPC_DIR}/ssl/cyassl/ctaocrypt/include/
                    ${SID_IPC_DIR}/include/
                    ${SID_IPC_DIR}/ssl/cyassl/include)

aux_source_directory(${CPPWRAPPER_DIR}/src/client/ CLIENT_SRCS)

if (USE_OPENGL)
    include_directories(${SID_DIR}/reference/openglvideorenderer
    ${SID_IPC_DIR}/VideoBuffers)
    list(APPEND SKYPEKIT_LIBRARIES reference_opengl_lib)
endif()

if (TARGET_OS MATCHES "android")
    list (APPEND SKYPEKIT_LIBRARIES gcc)
endif()

add_executable(skypekitclient ${CLIENT_SRCS})

target_link_libraries(skypekitclient ${SKYPEKIT_LIBRARIES})

install(TARGETS skypekitclient DESTINATION ${CPPWRAPPER_INSTALL_DIR})
