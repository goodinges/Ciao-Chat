# Create a separated library for Models.

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

add_library(Mvp-model
            skypecore.cpp
            skypeaccount.cpp
            skypecontact.cpp
            skypecontactgroup.cpp
            skypeconversation.cpp
            skypemessage.cpp
            skypeparticipant.cpp
            skypesms.cpp
            skypetransfer.cpp
            skypevideo.cpp
            contactlistservice.cpp
            convmanagerservice.cpp
            historyservice.cpp
            loginservice.cpp
            textchatservice.cpp
            callservice.cpp
            ftservice.cpp
            util.cpp
            )

# If GTest and GMock is found
if (GTEST_FOUND)
  add_subdirectory(tests)
endif (GTEST_FOUND)
