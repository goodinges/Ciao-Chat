
-include local.mk

FRAMEWORK ?= SE

CXX = g++

%$(OBJ_EXTENSION): %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $?

CXXFLAGS += -I../../

ifeq ($(FRAMEWORK),SE)
	CXXFLAGS += -g -I../se/ -I../../../../interfaces/skype/cpp_embedded/src/types/
	CXXFLAGS += -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/
	vpath %.cpp ../../../../interfaces/skype/cpp_embedded/src/types/
	TEST_SRCS += skype-ptrint-dict.cpp skype-binary.cpp skype-filename.cpp skype-filename-list.cpp skype-int-list.cpp skype-string.cpp skype-string-list.cpp skype-ptr-dict.cpp
	vpath %.cpp ../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread
	TEST_SRCS += skype-thread-pthread.cpp
	LDFLAGS += -lpthread
endif
ifeq ($(FRAMEWORK),POCO)
	CXXFLAGS += -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/
	vpath %.cpp ../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread
	TEST_SRCS += skype-thread-pthread.cpp
	
	CXXFLAGS += -I../poco/
	LDFLAGS += -lPocoFoundation
endif
ifeq ($(FRAMEWORK),QT)
	CXXFLAGS += -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/
	vpath %.cpp ../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread
	TEST_SRCS += skype-thread-pthread.cpp
	
	CXXFLAGS += -I../qt/ -I/usr/include/qt4/
	LDFLAGS += -lQtCore
endif

TEST_SRCS += test_dict_obj.cpp test_int_list.cpp test_thread.cpp test_string.cpp test_string_list.cpp test_binary.cpp test_string_dict.cpp
TEST_OBJS = $(patsubst %.cpp,%$(OBJ_EXTENSION).o, $(filter %.cpp,$(TEST_SRCS))) 
TESTS = runtests
LDFLAGS += -lcppunit -ldl

all: $(TESTS)

runtests: $(TEST_OBJS) main.o
	$(CXX) $(LDFLAGS) -o $@ $?	
	rm -f *.o
			
clean: 
	rm -f *.o $(TESTS) 			
