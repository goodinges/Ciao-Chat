#include "skype-clientsession.h"
#include "SidPlatform.hpp"
#include <assert.h>
#include "skype-object.h"
#include "SidDebugging.hpp"

SEClientSession::SEClientSession(int num_threads, Field* descriptors)
:       Sid::ClientSession(num_threads,descriptors),
        m_get_size(0),
        m_response_size(0)
{
        m_queue_capacity                = m_max_threads+1;
        m_response_queue                = (ResponseQueueElem*) malloc(sizeof(ResponseQueueElem)*m_queue_capacity);
        m_get_queue                     = (Sid::CommandInitiator**) malloc(sizeof(Sid::CommandInitiator*)*m_queue_capacity);
        m_main_command_initiator        = new Sid::CommandInitiator(this, this);
        assert(m_response_queue && m_get_queue && m_main_command_initiator);
        m_buffering = 0;
        m_fork_failed = 0;
        __m_logging_out = MAX_UINT;
        m_dispatch_all = 0;
}

SEClientSession::~SEClientSession()
{
        free(m_response_queue);
        free(m_get_queue);
        delete m_main_command_initiator;
}

bool SEClientSession::read_property_value(Sid::CommandInitiator* thread, Sid::Protocol::ClientInterface* protocol, uint moduleid, uint propertyid, uchar kind, void* value)
{
        Sid::Protocol::Status status;
        Sid::Field* field = get_property_desc(moduleid, propertyid);
        if (!field) {
                status = protocol->sk_value(kind, thread);
        } else {
                Sid::Mutex::ScopedLock sl(m_propcache_lock);
                status = protocol->rd_value(kind, thread, value, field-get_descriptors());
        }      
         
        return status == Sid::Protocol::OK; 
}

Sid::Protocol::Status SEClientSession::read_properties(Sid::CommandInitiator* cmdini)
{
        Sid::Protocol::Status status;
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint __moduleid, __objectid, __propertyid;
        uchar __kind = '0', kind = '0'; 
        bool has_missing = false;
        while ((status = protocol->rd_property(cmdini, __moduleid, __objectid, __propertyid, __kind)) == Sid::Protocol::OK) {        
                void* property_ptr = (__moduleid>0 && __objectid>0) ? get_cached_prop(__moduleid, __objectid, __propertyid, kind) : 0; 
                //SID_DBGPRINTF("GETRESP %d %d %d %c %p\n",__moduleid, __objectid, __propertyid, __kind, property_ptr);
                if (!property_ptr) {
                        //SID_DBGPRINTF("GETRESP %d missing property\n",status);
                        if (protocol->resync_decode(cmdini) != Sid::Protocol::OK) terminate_requests();
                        return Sid::Protocol::ERR_DECODE;
                }
                if (__kind != 'N') {
                        if (!(kind == __kind || (kind == Sid::Field::KIND_BOOLEAN && (__kind == Sid::Field::KIND_TRUE || __kind == Sid::Field::KIND_FALSE)))) {
//                                SID_DBGPRINTF("GETRESP %d type mismatch\n",status);
                                if (protocol->resync_decode(cmdini) != Sid::Protocol::OK) terminate_requests();
                                return Sid::Protocol::ERR_DECODE;
                        }
                        if (!read_property_value(cmdini, protocol, __moduleid, __propertyid, __kind, property_ptr)) {
//                                SID_DBGPRINTF("GETRESP %d fail reading value\n",status);
                                if (protocol->resync_decode(cmdini) != Sid::Protocol::OK) terminate_requests();
                                return Sid::Protocol::ERR_DECODE;
                        }
                        add_prop_cached(__moduleid, __objectid, __propertyid);                           
                } else {
                        del_prop_cached(__moduleid, __objectid, __propertyid);  
                        has_missing = true;
//                        SID_DBGPRINTF("GETRESP %d missing value\n",status);
                }
        }
        //SID_DBGPRINTF("GETRESP %d\n",status);
        if (has_missing) return Sid::Protocol::COMPLETED;
        return status == Sid::Protocol::COMPLETED ? Sid::Protocol::OK : status;
}

Sid::Protocol::Status SEClientSession::get_property(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, uint propertyid) 
{        if (!is_prop_cached(moduleid, objectid, propertyid)) {
                //SID_DBGPRINTF("GET %d %d %d\n",moduleid, objectid, propertyid);
                Sid::Protocol::ClientInterface* protocol = m_protocol;                  
                if (protocol->begin_get_request(cmdini, moduleid, objectid, propertyid) != Sid::Protocol::OK) 
                        return Sid::Protocol::ERR_ENCODE;

                if (!wait_get(cmdini)) 
                        return Sid::Protocol::ERR_DECODE;
                                                        
                return read_properties(cmdini);         
        }
                                
        return Sid::Protocol::OK;
}

Sid::Protocol::Status SEClientSession::get_property(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, uint propertyid, const char* preencoded, uint preencoded_len) 
{        if (!is_prop_cached(moduleid, objectid, propertyid)) {
                //SID_DBGPRINTF("GET %d %d %d\n",moduleid, objectid, propertyid);
                Sid::Protocol::ClientInterface* protocol = m_protocol;                  
                if (protocol->begin_get_request(cmdini, preencoded_len, preencoded, objectid) != Sid::Protocol::OK) 
                        return Sid::Protocol::ERR_ENCODE;

                if (!wait_get(cmdini)) 
                        return Sid::Protocol::ERR_DECODE;
                                                        
                return read_properties(cmdini);         
        }
                              
        return Sid::Protocol::OK;
}

Sid::Protocol::Status SEClientSession::get_properties(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, Sid::List_uint properties)
{
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        Sid::List_uint properties2;
        Sid::List_uint objlist;
        objlist.append(objectid);
                
        for (uint i = 0; i < (uint)properties.size(); i++) {
                if (!is_prop_cached(moduleid, objectid, properties[i])) {    
                        properties2.append(properties[i]);                                       
                }
        }
        
        if (!properties2.size()) 
                return Sid::Protocol::OK;        
        
        if (protocol->begin_get_request(cmdini) != Sid::Protocol::OK)
                return Sid::Protocol::ERR_ENCODE;       
        
        if (protocol->add_get_properties(cmdini, moduleid, objlist, properties2) != Sid::Protocol::OK)
                return Sid::Protocol::ERR_ENCODE;                      
                 
        if (!wait_get(cmdini)) 
                return Sid::Protocol::ERR_DECODE;
        
        return read_properties(cmdini);
}

Sid::Protocol::Status SEClientSession::get_properties(Sid::CommandInitiator* cmdini, uint moduleid, Sid::List_ref objects, Sid::List_uint properties)
{
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        Sid::List_uint objids;
        for (uint i = 0; i < (uint)objects.size(); i++) {        
                objids.append(objects[i].get_oid());                                       
        }   
             
        if (!(properties.size() && objids.size()))
                return Sid::Protocol::OK;      
        
        if (protocol->begin_get_request(cmdini) != Sid::Protocol::OK)
                return Sid::Protocol::ERR_ENCODE;       
        
        if (protocol->add_get_properties(cmdini, moduleid, objids, properties) != Sid::Protocol::OK)
                return Sid::Protocol::ERR_ENCODE;                      
                 
        if (!wait_get(cmdini))
                return Sid::Protocol::ERR_DECODE;       
        
        return read_properties(cmdini);
}

Sid::Protocol::Status SEClientSession::property_onchange(Sid::CommandInitiator* cmdini) 
{
        Sid::Protocol::Status status;
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint __moduleid, __objectid, __propertyid;
        uchar __kind, kind;
        void* property_ptr = 0;
        SEReference r;

        //SID_DBGPRINTF("%p polling onchange fork %d buffering = %d\n",cmdini,m_fork_failed,m_buffering); 
        while ((status = protocol->rd_property(cmdini, __moduleid, __objectid, __propertyid, __kind)) == Sid::Protocol::OK) {        
                Sid::Field* field = get_property_desc(__moduleid, __propertyid);
                r = SEReference(__objectid, __moduleid); 
                if (!field || (!m_dispatch_all && !r.present(0))) { // unknown property
                        if (protocol->sk_value(__kind, cmdini) != Sid::Protocol::OK) return Sid::Protocol::ERR_DECODE;
                } else {
                        property_ptr = get_cached_prop(__moduleid, __objectid, __propertyid, kind); 
                        if (!property_ptr)
                                return Sid::Protocol::ERR_DECODE;
                        if (__kind != 'N') {
                                if (!(kind == __kind || (kind == Sid::Field::KIND_BOOLEAN && (__kind == Sid::Field::KIND_TRUE || __kind == Sid::Field::KIND_FALSE))))
                                         return Sid::Protocol::ERR_DECODE;
                                Sid::Mutex::ScopedLock sl(m_propcache_lock);
                                if (protocol->rd_value(__kind, cmdini, property_ptr, field-get_descriptors()) != Sid::Protocol::OK) return Sid::Protocol::ERR_DECODE;
                                add_prop_cached(__moduleid, __objectid, __propertyid);  
                        } else {
                                del_prop_cached(__moduleid, __objectid, __propertyid);  
                        }                        
                }      
                 
        }
        if (property_ptr && (!__m_logging_out || __moduleid == __m_logging_out)) {
                //simplified: currently there is 1 propchange per message
                SEObject* o = r.can_dispatch();
                if (o) {
			//SID_DBGPRINTF("%p dispatch onchange %d:%d.%d\n",cmdini, __moduleid,  __objectid, __propertyid );
			o->OnChange(__propertyid);
		}
        }             
        //SID_DBGPRINTF("%p polling onchange finished with status %d\n",cmdini,status);  
        return status == Sid::Protocol::COMPLETED ? Sid::Protocol::OK : status;
}

bool SEClientSession::wait_get(Sid::CommandInitiator* requesting_thread)
{
        bool buffering;
        m_mutex.lock();
        if (is_stopped()) { m_mutex.unlock(); return false; } 
        // check if already triggered...
        //SID_DBGPRINTF("%p waiting for get %d, fork %d buffering = %d\n",requesting_thread,m_get_size,m_fork_failed,m_buffering); 
        if (m_get_size < 0) {
                m_get_size++;
                m_protocol->end_get_property(requesting_thread);
                m_mutex.unlock();
                //SID_DBGPRINTF("%p get with status OK\n",requesting_thread);
                return true;
        }
        assert(requesting_thread);
        buffering = (m_fork_failed == 1);
	if (buffering) { m_fork_failed = 0; }
        if (m_get_size == m_queue_capacity)
             resize_queues(m_queue_capacity+1);
        m_get_queue[m_get_size++]=requesting_thread;
        m_protocol->end_get_property(requesting_thread);
        m_mutex.unlock();
        //SID_DBGPRINTF("%p get enter_buffering_mode, buf %d\n",requesting_thread,buffering);
        bool ok = enter_buffering_mode(requesting_thread, buffering);
        //SID_DBGPRINTF("%p get with status %d\n",requesting_thread,ok);
        return ok; 
}

bool SEClientSession::call_mn(Sid::CommandInitiator*& thread,int sz,const char* hdr,int msg,...) {
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint rid;
        thread = get_current_thread();
        va_list args;
        va_start(args, msg);
        if (protocol->wr_call_vlst(thread,sz,hdr,rid,msg,args) == Sid::Protocol::OK) {
               va_end(args);
               return wait_response(thread, rid);
        }
        va_end(args);
        return false;
}

bool SEClientSession::rd_parms_lst(Sid::CommandInitiator* thread,int msg,...) {
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        Sid::Protocol::Status status;
        va_list args;
        va_start(args, msg);
        if ((status = protocol->rd_parms_vlst(thread, msg, args)) != Sid::Protocol::OK) {
               va_end(args);
               if (protocol->resync_decode(thread)) terminate_requests();
               return false; 
        }
        va_end(args);
        return status == Sid::Protocol::OK;
}

bool SEClientSession::call_m0(int sz,const char* hdr,int msg,...) {
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint rid;
        Sid::Protocol::Status status;
        Sid::CommandInitiator* thread = get_current_thread();
        va_list args;
        va_start(args, msg);
        if (protocol->wr_call_vlst(thread,sz,hdr,rid,msg,args) == Sid::Protocol::OK) {
                va_end(args);
                if (wait_response(thread, rid)) {
                        if ((status = protocol->rd_parms(thread)) != Sid::Protocol::OK) {
                                if (protocol->resync_decode(thread)) terminate_requests();
                                return false; 
                        }
                        return status == Sid::Protocol::OK;
                }
        } else va_end(args);
        return false;
}

bool SEClientSession::call_0n(int sz,const char* hdr,int msg,...) {
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint rid;
        Sid::Protocol::Status status;
        Sid::CommandInitiator* thread = get_current_thread();
        if (protocol->wr_call(thread,sz,hdr,rid) == Sid::Protocol::OK) {
                if (wait_response(thread, rid)) {
                        va_list args;
                        va_start(args, msg);
                        if ((status = protocol->rd_parms_vlst(thread, msg, args)) != Sid::Protocol::OK) {
                               va_end(args);
                               if (protocol->resync_decode(thread)) terminate_requests();
                               return false; 
                        }
                        va_end(args);
                        return status == Sid::Protocol::OK;
                }
        }
        return false;
}

bool SEClientSession::call_00(int sz,const char* hdr) {
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        uint rid;
        Sid::Protocol::Status status;
        Sid::CommandInitiator* thread = get_current_thread();
        if (protocol->wr_call(thread,sz,hdr,rid) == Sid::Protocol::OK) {
                if (wait_response(thread, rid)) {
                        if ((status = protocol->rd_parms(thread)) != Sid::Protocol::OK) {
                               if (protocol->resync_decode(thread)) terminate_requests();
                               return false; 
                        }
                        return status == Sid::Protocol::OK;
                }
        }
        return false;
}

void SEClientSession::receive_get()
{
        Sid::CommandInitiator* thread;
        m_mutex.lock();
        if (m_get_size-- == 0) {
                m_mutex.unlock();
                return;
        }
        thread = m_get_queue[0];
        for (int i=0; i < m_get_size; i++) m_get_queue[i]=m_get_queue[i+1]; 
        m_mutex.unlock();
        thread->resume(1);
}

void SEClientSession::resize_queues(int sz) {
  m_response_queue = (ResponseQueueElem*) realloc(m_response_queue, sz*sizeof(ResponseQueueElem));
  assert(m_response_queue);
  m_get_queue      = (Sid::CommandInitiator**) realloc(m_get_queue, sz*sizeof(Sid::CommandInitiator*));
  assert(m_get_queue);
  m_queue_capacity = sz;
}

bool SEClientSession::enter_buffering_mode(Sid::CommandInitiator* requesting_thread, int buffering) {
        Sid::CommandInitiator* thread = 0;
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        Sid::Protocol::Status status = Sid::Protocol::OK;
        Sid::Protocol::Command cmd;
        do {
                while (buffering) {
                        //SID_DBGPRINTF("%p Buffering\n", requesting_thread);
                        buffering = 0;
                        if ((status = protocol->buffer_events(requesting_thread, cmd)) == Sid::Protocol::OK) {
                                switch (cmd) {
                                case Sid::Protocol::CMD_GET_REPLY:
                                        m_mutex.lock();
                                        thread = m_get_queue[0];
                                        m_get_size--;
                                        //SID_DBGPRINTF("%p unbuf get = %d => %p\n",requesting_thread,m_get_size,thread);
                                        for (int i=0; i < m_get_size; i++) m_get_queue[i]=m_get_queue[i+1]; 
                                        m_mutex.unlock();
                                        if (thread == requesting_thread) return true;
                                        thread->resume(1);
                                        break;
                                case Sid::Protocol::CMD_XREPLY: {
                                        // wake up the other thread and yield()
                                        uint rid = 0;
                                        if ((status = protocol->rd_response_id(requesting_thread, rid)) == Sid::Protocol::OK) {
                                                m_mutex.lock();
                                                thread = 0;
                                                for (int i=0; i<m_response_size;i++) {
                                                        if (m_response_queue[i].rid==rid) {
                                                                thread = m_response_queue[i].thread;
                                                                //SID_DBGPRINTF("%p rid = %d => %p\n",requesting_thread,rid,thread);
                                                                m_response_queue[i]=m_response_queue[--m_response_size];
                                                                m_mutex.unlock();
                                                                if (thread == requesting_thread) return true;
                                                                thread->resume(1);
                                                                break;
                                                        }
                                                }
                                                // happens when requesting thread hasn't registered yet in queue
                                                if (thread == 0) { 
                                                        if (m_response_size == m_queue_capacity)
                                                                resize_queues(m_queue_capacity+1);
                                                        m_response_queue[m_response_size].rid=rid;
                                                        m_response_queue[m_response_size++].thread=0;
                                                        //SID_DBGPRINTF("!!!%p rid = %d => %p\n",requesting_thread,rid,thread);
                                                        m_mutex.unlock();
                                                }
                                        } else if ((status = protocol->resync_decode(requesting_thread)) != Sid::Protocol::OK) {
                                                terminate_requests(requesting_thread);
                                                return false;
                                        }
                                        break;
                                }
                                default: { // better processing events instead of buffering them
                                        // events: end decode was called: either fork succeeded and new thread is reading or it failed
                                        // when it failed it may have resumed another thread... or more likely this one
                                        // in which case when reaching next yield, it will go on with switching 
                                        // maybe one may not call end_decoding in protocol->buffer_events to keep this thread for sure
                                        // and save a call to yield
                                        //SID_DBGPRINTF("%p store ev = %d, cmd = %c, m_fork_failed = %d, m_buffering = %d\n",requesting_thread,buffering,cmd,m_fork_failed,m_buffering);
                                        break;
                                }
                                }
                        } else if ((status = protocol->resync_decode(requesting_thread)) != Sid::Protocol::OK) {
                                terminate_requests(requesting_thread);
                                return false;
                        }
                }
                //SID_DBGPRINTF("%p Yielding\n",requesting_thread);
                if (!requesting_thread->yield()) return false;
                //SID_DBGPRINTF("%p Resuming %d\n",requesting_thread,m_buffering);
                // we are waken up for 2 reasons
                // either no more listeners were available: then we shall start buffering
                buffering   = m_buffering;
                if (buffering) m_buffering = 0;
        } while (buffering);
        // or we were replied 
        return status == Sid::Protocol::OK;
}

bool SEClientSession::wait_response(Sid::CommandInitiator* requesting_thread, const uint& rid)
{
        bool buffering;
        m_mutex.lock();
        if (is_stopped()) { m_mutex.unlock(); return false; } 
        //SID_DBGPRINTF("%p waiting for rid %d, fork %d buffering = %d\n",requesting_thread,rid,m_fork_failed,m_buffering); 
        // check if already triggered...
        for (int i=0; i<m_response_size;i++) {
                if (m_response_queue[i].rid==rid) {
                        m_response_queue[i]=m_response_queue[--m_response_size];
                        m_mutex.unlock();
                        //SID_DBGPRINTF("%p rid %d with status OK\n",requesting_thread,rid);
                        return true;
                }
        }
        buffering = (m_fork_failed == 1);       // there is no one reading the stream
        if (buffering) { m_fork_failed = 0; }   // I will
        if (m_response_size == m_queue_capacity)
                resize_queues(m_queue_capacity+1);
        m_response_queue[m_response_size].rid     =rid;
        m_response_queue[m_response_size++].thread=requesting_thread;
        m_mutex.unlock();
        //SID_DBGPRINTF("%p rid %d enter_buffering_modebuf %d\n",requesting_thread,rid,buffering);
        bool ok = enter_buffering_mode(requesting_thread, buffering);
        //SID_DBGPRINTF("%p rid %d with status %d\n",requesting_thread,rid,ok);
        return ok; 
}

void SEClientSession::receive_response(const uint& rid)
{
        //SID_DBGPRINTF("receiving resp for rid %u\n", rid); 
        m_mutex.lock();
        for (int i=0; i<m_response_size;i++) {
                if (m_response_queue[i].rid==rid) {
                        Sid::CommandInitiator* thread = m_response_queue[i].thread;
                        m_response_queue[i]=m_response_queue[--m_response_size];
                        m_mutex.unlock();
                        thread->resume(1);
                        return;
                }
        }
        if (m_response_size == m_queue_capacity)
                resize_queues(m_queue_capacity+1);
        m_response_queue[m_response_size].rid=rid;
        m_response_queue[m_response_size++].thread=0;
        m_mutex.unlock();
}



void SEClientSession::notify_no_more_command_processor() // called from allocate() thus end_decode() => single caller = current reader
{
        Sid::CommandInitiator* thread = 0;
        m_mutex.lock(); // still compete from deallocating threads
        //SID_DBGPRINTF("%p end_decoding() without successors fork %d buffering %d\n",SEClientSession::get_current_thread(),m_fork_failed,m_buffering);
        assert(!m_buffering);
        if      (m_get_size > 0)      { m_buffering = 1;   thread = m_get_queue[0]; assert(thread);             }
        else if (m_response_size > 0) { m_buffering = 1;   thread = m_response_queue[0].thread; assert(thread); }
        else                          { m_fork_failed = 1; /* SID_DBGPRINTF("Fork failed\n"); */ }
        m_mutex.unlock();
        if (thread) {
             //SID_DBGPRINTF("%p Buffering resuming %p\n",SEClientSession::get_current_thread(),thread);
             thread->resume(1); 
        } 
}
bool SEClientSession::reuse_command_processor(Sid::CommandProcessor* /*cp*/)
{
        // we have a transport listener that can at least unbuffer the events
        bool alloc;
        m_mutex.lock();
        //SID_DBGPRINTF("%p go back to pool fork %d\n",cp,m_fork_failed);
        alloc = (m_fork_failed == 1);
		if (alloc) { m_fork_failed = 0; }
        m_mutex.unlock();
        if (alloc) {  /* SID_DBGPRINTF("Keep command processor %p\n",cp); */ return true; }
        return false;
}

Sid::CommandInitiator* SEClientSession::get_current_thread()
{
        Sid::Thread* curr = Sid::Thread::current();
        if (!curr) return m_main_command_initiator;
        Sid::CommandProcessor* cmd = static_cast<Sid::CommandProcessor*>(curr); 
        return cmd; // can be 0! if appl uses multiple user threads... in which case it would be better having a map<threadid,CommandInitiator>;
}

void SEClientSession::process_command(Sid::CommandInitiator* thread)
{
        Sid::Protocol::Status status = Sid::Protocol::OK;
        Sid::Protocol::Command cmd;
        Sid::Protocol::ClientInterface* protocol = m_protocol;
        //SID_DBGPRINTF("%p is reading command...\n", thread);
        if ((status = protocol->rd_response_or_event(thread, cmd)) == Sid::Protocol::OK) {
				        //SID_DBGPRINTF("running %p %c\n", thread, cmd);
                switch (cmd) {
                case Sid::Protocol::CMD_XREPLY : {
                        uint rid = 0;
                        status = protocol->rd_response_id(thread, rid);
                        if (status == Sid::Protocol::OK) {
                                receive_response(rid);
                                return; // join
                        }
                        break;
                }
                case Sid::Protocol::CMD_EVENT: {
                        uint modid, event;
                        if (protocol->rd_event(thread, modid, event) == Sid::Protocol::OK) {
				if (__m_logging_out) { protocol->sk_parms(thread); return; } // ignore events...
                                if ((status = dispatch_event(thread, protocol, modid, event)) == Sid::Protocol::OK) { // forks here after decoding
                                        return; // join
                                }
                        }
                        break;
                }
                case Sid::Protocol::CMD_PROPCHANGE : {
                        if ((status = property_onchange(thread)) == Sid::Protocol::OK) {
                                return; // join                                
                        }
                        break;
                }
                case Sid::Protocol::CMD_GET_REPLY : {
                        receive_get();
                        return; // join
                }
                default: {
                        status = Sid::Protocol::ERR_DECODE; // force end_decode()
                        break;
                }
                }
        }
        if (status == Sid::Protocol::ERR_DECODE) { 
                if (protocol->resync_decode(thread) != Sid::Protocol::OK) terminate_requests();
        }
}

void SEClientSession::terminate_requests(Sid::CommandInitiator* listener) {
        m_mutex.lock();
        m_stopped = true;
	for (int i = 0; i < m_get_size; i++) {
		if (m_get_queue[i] && m_get_queue[i] != listener)
                        m_get_queue[i]->resume(0);
	}
	for (int i = 0; i < m_response_size; i++) {
		if (m_response_queue[i].thread && m_response_queue[i].thread != listener)
                        m_response_queue[i].thread->resume(0);
	}
	m_response_size = 0;
	m_get_size = 0;
        m_mutex.unlock();
}

