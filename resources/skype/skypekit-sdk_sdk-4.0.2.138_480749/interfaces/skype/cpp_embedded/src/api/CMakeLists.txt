
#source
aux_source_directory(${CPPWRAPPER_DIR}/src/ipc SRCS)

#platform
list(APPEND SRCS ${SID_PLATFORM_DIR}/SidPlatform.cpp)

#generated api
list(APPEND SRCS ${CPPWRAPPER_DIR}/src/api/sidg_Skypeevent_dispatch.cpp
                 ${CPPWRAPPER_DIR}/src/api/sidg_msgs_Skype.cpp
                 ${CPPWRAPPER_DIR}/src/api/sidg_Skypeaction_call.cpp
                 ${CPPWRAPPER_DIR}/src/api/skype-embedded_2.cpp
                 ${CPPWRAPPER_DIR}/src/api/sidg_Skypeproperty_get_call.cpp
                 ${CPPWRAPPER_DIR}/src/api/sidg_Skyperefs.cpp
    )

#common ipc
list(APPEND SRCS ${SID_IPC_DIR}/SidProtocolClientInterface.hpp
                 ${SID_IPC_DIR}/SidSession.cpp
                 ${SID_IPC_DIR}/SidSession.hpp
                 ${SID_IPC_DIR}/SidClientSession.cpp
                 ${SID_IPC_DIR}/SidClientSession.hpp
                 ${SID_IPC_DIR}/SidCommandProcessor.cpp
                 ${SID_IPC_DIR}/SidCommandProcessor.hpp
                 ${SID_IPC_DIR}/SidField.hpp
                 ${SID_IPC_DIR}/SidProtocolBinCommon.cpp
                 ${SID_IPC_DIR}/SidProtocolBinCommon.hpp
                 ${SID_IPC_DIR}/SidProtocolBinClient.cpp
                 ${SID_IPC_DIR}/SidProtocolBinClient.hpp
                 ${SID_IPC_DIR}/SidConnection.cpp
                 ${SID_IPC_DIR}/SidConnection.hpp
                 ${SID_IPC_DIR}/SidTransportLog.cpp
                 ${SID_IPC_DIR}/SidTransportLog.hpp
                 ${SID_IPC_DIR}/SidClientConnection.hpp
                 ${SID_IPC_DIR}/SidClientConnection.cpp
                 ${SID_IPC_DIR}/SidTLSEncryption.cpp
                 ${SID_IPC_DIR}/SidTLSEncryption.hpp
                 ${SID_IPC_DIR}/SidDebugging.cpp
    )

include_directories(${CPPWRAPPER_DIR}/src/api
                    ${CPPWRAPPER_DIR}/src/ipc
                    ${CPPWRAPPER_DIR}/src/types
                    ${SID_IPC_DIR}
                    ${SID_PLATFORM_DIR}
                    ${CPPWRAPPER_DIR}/src/platform/threading
                   )

#ssl
add_definitions(-DSSL_LIB_CYASSL -DNO_FILESYSTEM)

include_directories(${SID_IPC_DIR}/ssl/cyassl/ctaocrypt/include
                    ${SID_IPC_DIR}/ssl/cyassl/include
                   )

# threading
if (WIN32)
    aux_source_directory(${CPPWRAPPER_DIR}/src/platform/threading/win/ THREAD_SRCS)

    include_directories(${CPPWRAPPER_DIR}/src/platform/threading/win/)
else()
    aux_source_directory(${CPPWRAPPER_DIR}/src/platform/threading/pthread/ THREAD_SRCS)

    include_directories(${CPPWRAPPER_DIR}/src/platform/threading/pthread/)
endif()

aux_source_directory(${CPPWRAPPER_DIR}/src/types TYPES_SRCS)

if (USE_OPENGL)
    add_definitions(-DSKYPEKIT_SURFACE_RENDERING)
    if (WIN32)
        list(APPEND Surface_Rendering_Srcs ${SID_IPC_DIR}/VideoBuffers/WinShm.cpp
                                           ${SID_IPC_DIR}/VideoBuffers/WinShm.hpp
            )
    else()
        list(APPEND Surface_Rendering_Srcs ${SID_IPC_DIR}/VideoBuffers/PosixShm.cpp
                                           ${SID_IPC_DIR}/VideoBuffers/PosixShm.hpp
                                           ${SID_IPC_DIR}/VideoBuffers/SysVShm.cpp
                                           ${SID_IPC_DIR}/VideoBuffers/SysVShm.hpp
        )
    endif()
endif()

add_library(skypekit-cppwrapper_2_lib STATIC ${SRCS} ${THREAD_SRCS} ${TYPES_SRCS} ${Surface_Rendering_Srcs})

install(TARGETS skypekit-cppwrapper_2_lib DESTINATION ${CPPWRAPPER_INSTALL_DIR})
