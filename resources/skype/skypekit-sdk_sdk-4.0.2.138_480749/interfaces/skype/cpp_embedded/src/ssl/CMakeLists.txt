# create cyassl library from sources found in root ipc directory

aux_source_directory(${SID_IPC_DIR}/ssl/cyassl/src CYASSL_SRCS)

aux_source_directory(${SID_IPC_DIR}/ssl/cyassl/ctaocrypt/src CTAOCRYPT_SRCS)

include_directories(${SID_IPC_DIR}/ssl/cyassl/include
                    ${SID_IPC_DIR}/ssl/cyassl/include/openssl
                    ${SID_IPC_DIR}/ssl/cyassl/ctaocrypt/include
                   ) 

add_definitions(-DNO_RABBIT -DNO_HC128 -DNO_DES -DNO_DSA -DNO_MD4 -DNO_FILESYSTEM -DHAVE_CONFIG_H)

add_library(skypekit-cyassl_lib STATIC ${CYASSL_SRCS} ${CTAOCRYPT_SRCS})

install(TARGETS skypekit-cyassl_lib DESTINATION ${CPPWRAPPER_INSTALL_DIR})

