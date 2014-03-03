
set(SRCS
    skype-binary.cpp
    skype-filename.cpp
    skype-filename-list.cpp
    skype-int-dict.cpp
    skype-int-list.cpp
    skype-obj-dict.cpp
    skype-ptr-dict.cpp
    skype-ptrint-dict.cpp
    skype-ptr-vector.cpp
    skype-ref-list.cpp
    skype-string.cpp
    skype-string-dict.cpp
    skype-string-list.cpp
    ../platform/threading/pthread/skype-thread-pthread.cpp
)

foreach( file ${SRCS} )
    list(APPEND SSRC ${CMAKE_CURRENT_SOURCE_DIR}/${file} )
endforeach( file )

set(SID_SRC ${SID_SRC} ${SSRC} PARENT_SCOPE)
