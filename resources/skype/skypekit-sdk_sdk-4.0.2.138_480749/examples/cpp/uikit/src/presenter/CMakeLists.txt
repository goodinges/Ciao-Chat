include_directories(${SKT_CPP_DIR}/src/api
                    ${SKT_CPP_DIR}/src/ipc
                    ${SKT_CPP_DIR}/src/types
                    ${SKT_CPP_DIR}/src/platform/threading
                    ${SKT_DIR}/ipc/cpp
                    ${SKT_DIR}/ipc/cpp/platform/se
                    ${SKT_DIR}/ipc/cpp/ssl/cyassl/ctaocrypt/include
                    ${SKT_DIR}/ipc/cpp/ssl/cyassl/include
                   )

IF(WIN32)
    include_directories(${SKT_CPP_DIR}/src/platform/threading/win)
ELSE(WIN32)
    include_directories(${SKT_CPP_DIR}/src/platform/threading/pthread)
ENDIF(WIN32)

# Create a separated library for Presenters.

add_library(Mvp-presenter
            state/statemachine.cpp
            state/context.cpp
            state/state.cpp
            state/callstate.cpp
            state/chatstate.cpp
            state/convmanagerstate.cpp
            state/historystate.cpp
            state/idlestate.cpp
            state/loggedoutstate.cpp
            state/ftstate.cpp
            contactlistpresenter.cpp
            convmanagerpresenter.cpp
            historypresenter.cpp
            ftpresenter.cpp
            loginpresenter.cpp
            textchatpresenter.cpp
            callpresenter.cpp)

# If GTest and GMock is found
if (GTEST_FOUND)
    add_subdirectory(tests)
endif (GTEST_FOUND)
