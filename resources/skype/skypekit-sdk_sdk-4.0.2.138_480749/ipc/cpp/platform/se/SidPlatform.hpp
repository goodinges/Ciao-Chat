#ifndef Sid_SidPlatform_INCLUDED_HPP
#define Sid_SidPlatform_INCLUDED_HPP

#include "SidCommonConfig.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <skype-ref-list.h>
#include <skype-string.h>
#include <skype-binary.h>
#include <skype-filename.h>
#include <skype-filename-list.h>
#include <skype-int-list.h>
#include <skype-uint64-list.h>
#include <skype-thread.h>
#include <skype-ptr-vector.h>
#include <skype-obj-dict.h>
#include <skype-int-dict.h>
#include <skype-ptrint-dict.h>
#include "skype-ptr-dict.h"

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef unsigned char uchar;

#define MAX_UINT ((uint)-1)

#ifdef _WIN32
#define snprintf _snprintf
#endif

namespace Sid
{
        typedef unsigned long long uint64;

        typedef SEString              String;
        typedef SEFilename            Filename;
        typedef SEBinary              Binary;
        
        typedef SEIntList             List_int;
        typedef SEIntList             List_uint;
        typedef SEUInt64List          List_uint64;
        typedef SEStringList          List_String;
        typedef SEFilenameList        List_Filename;
        typedef SERefList             List_ref;
        
        typedef SEObjDict             Dict_SEObj;
        typedef SEIntDict             Dict_int;
        typedef SEPtrIntDict          Dict_ptr; 
        typedef SEPtrDict             Dict_ptr_str;
        
        class Thread : public ::SEThread {
        public:
                Thread(const char* nm = 0) : SEThread(), name(nm) {}
                const char* get_name()  { return (const char*)name; }
                void start(const char* nm = 0) { if (nm) name = nm; SEThread::Start(); }
                void stop() { SEThread::Stop(); }
                static void sleep(uint milliseconds) { ::SEThread::Sleep(milliseconds); }
                SEString name; 
                static Thread* current() { return static_cast<Thread*>(SEThread::Current()); }
        };
        
        class Mutex : public ::SEMutex {
        public:
                typedef ::SEMutexLock ScopedLock;
                Mutex(const String& = "") {}
                void lock()     { ::SEMutex::Acquire(); }     
                void unlock()   { ::SEMutex::Release(); }
                bool try_lock() { return ::SEMutex::TryAcquire(); }
        };
        
        class Event : public ::SESyncEvent {
        public:
                void wait()   { Wait(); }
                void notify() { Trigger(); }
        };
        
        namespace Platform
        {        
                void list_remove(List_int* list, int i);
                void list_remove(List_String* list, int i);   
                void list_remove(List_Filename* list, int i);  
                void list_remove(List_ref* list, int i); 
        }
        struct Field;
        struct ModProp2Field {
                Field* contains(uint, uint , int&) { return 0; }
                void add(uint, uint , Field*, int)  {}
        };  
} //namespace

#endif //Sid_SidPlatform_INCLUDED_HPP











