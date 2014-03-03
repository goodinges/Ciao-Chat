LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	../../ \
	../

LOCAL_CFLAGS := -DDEBUG -DTRANSPORT_TEST
LOCAL_MODULE    := test_shm
LOCAL_LDLIBS    := -llog

LOCAL_SRC_FILES = \
	transport_test.cpp \
	../ShmTransport.cpp \
	../ShmIpcPosix.cpp

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	../../ \
	../

LOCAL_CFLAGS := -DDEBUG -DTRANSPORT_TEST
LOCAL_MODULE    := test_sock
LOCAL_LDLIBS    := -llog

LOCAL_SRC_FILES = \
	transport_test.cpp \
	../SocketTransport.cpp \
	../UnixSocket.cpp

include $(BUILD_EXECUTABLE)



include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	../../ \
	../

LOCAL_CFLAGS := -DDEBUG
LOCAL_MODULE    := dir_walker

LOCAL_SRC_FILES = \
	dir_walker.cpp

include $(BUILD_EXECUTABLE)
