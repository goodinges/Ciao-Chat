#ifndef SEClientSession__INCLUDED
#define SEClientSession__INCLUDED

#include "SidClientSession.hpp"
#include "SidPlatform.hpp"

class SEObject;

class SEClientSession : public Sid::ClientSession
{
public:
        SEClientSession(int num_threads, Sid::Field* descriptors);
        virtual ~SEClientSession();

        bool read_property_value(Sid::CommandInitiator* thread, Sid::Protocol::ClientInterface* protocol, uint moduleid, uint propertyid, uchar kind, void* value); 
        Sid::Protocol::Status read_properties(Sid::CommandInitiator* cmdini); 
        
        Sid::Protocol::Status get_property(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, uint propertyid);
        Sid::Protocol::Status get_property(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, uint propertyid, const char* preencoded, uint preencoded_len);
        
        //multiget for 1 object, cached props are filtered out
        Sid::Protocol::Status get_properties(Sid::CommandInitiator* cmdini, uint moduleid, uint objectid, Sid::List_uint properties);     
        //multiget for multiple objects
        Sid::Protocol::Status get_properties(Sid::CommandInitiator* cmdini, uint moduleid, Sid::List_ref objects, Sid::List_uint properties);
        
        Sid::Protocol::Status property_onchange(Sid::CommandInitiator* cmdini);
        
        bool wait_get(Sid::CommandInitiator* requesting_thread);
        void receive_get();
        bool wait_response(Sid::CommandInitiator* requesting_thread, const uint& rid);
        void receive_response(const uint& rid);

        bool call_mn(Sid::CommandInitiator*&,int,const char*,int,...);
        bool rd_parms_lst(Sid::CommandInitiator*,int,...);
        bool call_m0(int,const char*,int,...);
        bool call_0n(int,const char*,int,...);
        bool call_00(int,const char*);
        
        virtual void* get_cached_prop(const uint& moduleid, const uint& objectid, const uint& property_id, uchar& kind) = 0;
            
        Sid::Protocol::ClientInterface* get_protocol() {
                return m_protocol;
        }
        virtual Sid::CommandInitiator* get_current_thread();
        static SEClientSession* instance();

	void __logging_out(uint account_mod_id) { __m_logging_out = account_mod_id; }
	void __logout_done()                    { __m_logging_out = 0; }

	void set_dispatch_all()   { m_dispatch_all = 1; }
	int  dispatch_all() const { return m_dispatch_all; }
protected:
        
        struct ResponseQueueElem {
                uint                   rid;
                Sid::CommandInitiator* thread;
        };

        int                m_queue_capacity;
        ResponseQueueElem* m_response_queue;

        virtual Sid::Field* get_property_desc(const uint& moduleid, const uint& property_id)=0; 
        virtual bool is_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id) = 0;
        virtual void add_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id) = 0;
        virtual void del_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id) = 0;
        
        void notify_no_more_command_processor();    
        bool reuse_command_processor(Sid::CommandProcessor* /*cp*/);
        bool enter_buffering_mode(Sid::CommandInitiator* requesting_thread, int buffering);

        virtual Sid::Protocol::Status dispatch_event(Sid::CommandInitiator* thread, Sid::Protocol::ClientInterface* protocol, const uint& modid, const uint& evid) = 0;
        void process_command(Sid::CommandInitiator* thread);

        void resize_queues(int new_size);

        void terminate_requests(Sid::CommandInitiator* = 0);

        Sid::CommandInitiator** m_get_queue; // should be a semaphore...
        int m_get_size;
        int m_response_size;
        int m_buffering;
        int m_fork_failed;
	int m_dispatch_all;
	uint  __m_logging_out;
        Sid::Mutex m_mutex;
        Sid::CommandInitiator* m_main_command_initiator;        
        Sid::Mutex m_propcache_lock;
public:
        Sid::Mutex& get_cache_lock() { return m_propcache_lock; }
};

#endif

