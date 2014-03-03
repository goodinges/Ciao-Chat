
#CXX=/opt/cs08q3/bin/arm-none-linux-gnueabi-g++
CXX=g++
NDK?=/opt/android-ndk-r4b/

CFLAGS+=-DTRANSPORT_TEST

SRC_LIST_SHM = \
	transport_test.cpp \
	../ShmTransport.cpp \
	../ShmIpcSysV.cpp

SRC_LIST_SOCK = \
	transport_test.cpp \
	../SocketTransport.cpp \
	../UnixSocket.cpp
	

test_sock: $(SRC_LIST_SOCK)
	$(CXX) -static -g -Os -DDEBUG $(CFLAGS) -o test_sock  $^ -I../../ -I../ -lrt -lpthread

test_shm: $(SRC_LIST_SHM)
	$(CXX) -static -g -Os -DDEBUG $(CFLAGS)  -o test_shm  $^ -I../../ -I../ -lrt -lpthread


android:
	$(NDK)ndk-build APP_BUILD_SCRIPT=Android.mk V=1 CFLAGS+=$(CFLAGS)

android-clean:
	$(NDK)ndk-build APP_BUILD_SCRIPT=Android.mk clean  V=1 CFLAGS+=$(CFLAGS)

clean:
	rm -f test