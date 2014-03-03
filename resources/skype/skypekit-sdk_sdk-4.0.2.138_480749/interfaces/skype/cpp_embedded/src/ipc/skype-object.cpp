#include "skype-object.h"
#include "SidProtocolBinClient.hpp"
#include "SidProtocolBinCommon.hpp"

#include <stdio.h>

SEPMap::SEPMap(const PMAP* pmap)
                :p_map(pmap)
{
}

String SEPMap::map(uint key) const
{
        int n = 0;
        while (p_map[n].prop) {
                if (p_map[n].key == key)
                        return p_map[n].prop;
                n++;
        }
        return String();
}

int SEPMap::map(const String& prop) const
{
        int n = 0;
        while (p_map[n].prop) {
                if (p_map[n].prop == prop)
                        return p_map[n].key;

                n++;
        }
        return -1;
}

//static uint num_created_objects = 0, num_deleted_objects = 0;

SEObject::SEObject(unsigned int oid, SERootObject* s) :
                root(s),
                __m_constructed(0),
                object_id(oid),
                __num_ref(1)
{
//num_created_objects++;
        s->__insert_object(this, oid);
}

SEObject::~SEObject()
{
//num_deleted_objects++;
}

Dict_int SEObject::GetProps(const List_uint& what)
{
        Dict_int r;
        for (int i=0,e=what.size(); i<e; i++) {
                r.insert(what[i], GetProp(what[i]));
        }

        return r;
}

bool SEObject::FetchProps(const List_uint& properties)
{
        return root->get_properties(root->get_current_thread(),
                        moduleID(), getOID(), properties) == Sid::Protocol::OK;
}

String SEObject::GetProp(int property)
{
        String value = "";
        uchar kind;
        root->get_property(root->get_current_thread(), moduleID(), getOID(), property);
        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        //assert(cached_ptr); may happen if 'N'
        if (!cached_ptr) return "<missing>";
        switch (kind) {
        case Sid::Field::KIND_ENUM:
        case Sid::Field::KIND_INT:
        case Sid::Field::KIND_UINT:
        case Sid::Field::KIND_UINT64:
        case Sid::Field::KIND_OBJECTID: {
                int val = *static_cast<int*>(cached_ptr);
                char res[12];
                snprintf(res, 12, "%d", val);
                value = res;
                break;
        }
        case Sid::Field::KIND_TRUE:
                value = "1";
                break;
        case Sid::Field::KIND_FALSE:
                value = "0";
                break;
        case Sid::Field::KIND_BOOLEAN: {
                bool val = *static_cast<bool*>(cached_ptr);
                value = val ? "1" : "0";
                break;
        }
        case Sid::Field::KIND_STRING:
        case Sid::Field::KIND_FILENAME:
        case Sid::Field::KIND_XML: {
                value = *static_cast<String*>(cached_ptr);
                break;
        }
        case Sid::Field::KIND_BINARY:
                value = "<binary>";
                break;
        }

        return value;
}

String SEObject::GetStrProp(uint property)
{
        uchar kind;
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        assert((kind == Sid::Field::KIND_STRING
                || kind == Sid::Field::KIND_XML) && cached_ptr);
        root->get_property(root->get_current_thread(), moduleID(),
                                               getOID(), property);

        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        return *static_cast<String*>(cached_ptr);
}

bool SEObject::GetBoolProp(uint property)
{
        uchar kind;
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        assert((kind == Sid::Field::KIND_TRUE
                || kind == Sid::Field::KIND_FALSE
                || kind == Sid::Field::KIND_BOOLEAN) && cached_ptr);
        root->get_property(root->get_current_thread(), moduleID(),
                                               getOID(), property);

        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        return *static_cast<bool*>(cached_ptr);
}

uint SEObject::GetUintProp(uint property)
{
        uchar kind;
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        assert((kind == Sid::Field::KIND_ENUM
                || kind == Sid::Field::KIND_INT
                || kind == Sid::Field::KIND_UINT
                || kind == Sid::Field::KIND_UINT64) && cached_ptr);
        root->get_property(root->get_current_thread(), moduleID(),
                                               getOID(), property);
        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        return *static_cast<uint*>(cached_ptr);
}

SEReference SEObject::GetObjProp(uint property)
{
        uchar kind;
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        assert(kind == Sid::Field::KIND_OBJECTID && cached_ptr);
        root->get_property(root->get_current_thread(), moduleID(),
                                               getOID(), property);

        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        return *static_cast<SEReference*>(cached_ptr);
}

Binary SEObject::GetBinProp(uint property)
{
        uchar kind;
        void* cached_ptr = root->get_cached_prop(moduleID(), getOID(), property, kind);
        assert(kind == Sid::Field::KIND_BINARY && cached_ptr);
        root->get_property(root->get_current_thread(), moduleID(),
                                               getOID(), property);

        Sid::Mutex::ScopedLock sl(root->m_propcache_lock);
        return *static_cast<Binary*>(cached_ptr);
}

List_String SEObject::getPropDebug(int key, const String& value)
{
        List_String result;
        const SEPMap pm(getPMap());

        result.append(getNamespace());
        result.append(pm.map(key));
        String better_value = value;
        const EMAP* em = getEMap();
        while (em->prop) {
                if (key == em->prop) {
                        SEPMap cur_map(em->map);
                        uint i = strtoul(value.data(), NULL, 10);
                        better_value = cur_map.map(i);
                        break;
                }
                em++;
        };

        result.append(better_value);
        return result;
}

List_String SEObject::getPropDebug(int key, const uint value)
{
        List_String result;
        const SEPMap pm(getPMap());

        result.append(getNamespace());
        result.append(pm.map(key));
        char res[11];
        snprintf(res, 11, "%u", value);
        String better_value = res;
        const EMAP* em = getEMap();

        while (em->prop) {
                if (key == em->prop) {
                        SEPMap cur_map(em->map);
                        better_value = cur_map.map(value);
                        break;
                }
                em++;
        };

        result.append(better_value);
        return result;
}

inline void SEObject::__safe_inc_ref() {
        Sid::Mutex::ScopedLock sl(root->m_objmap_lock);
        __num_ref++;               // make them thread safe?
}

inline unsigned SEObject::__safe_dec_ref() {
       Sid::Mutex::ScopedLock sl(root->m_objmap_lock);
       return --__num_ref;
}


SERootObject* SERootObject::m_instance = 0;

SERootObject::~SERootObject()
{
        cleanup(1);
        m_instance = 0;
//printf("created %u / deleted %u \n", num_created_objects, num_deleted_objects);
}

SERootObject::SERootObject(int num_threads, Sid::Field* descriptors)
:       SEClientSession(num_threads,descriptors),
        __m_ts(0){
        m_instance = this;
}


void SERootObject::cleanup(int all, SEReference* account_ref)
{
	if (all != 1) return;
        Sid::Mutex::ScopedLock lprops(m_propcache_lock);
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	__m_ts++; // unref can't occur anymore when a property in cache is a reference
        List_int keys = oid2obj.keys();
	for (uint o = 0; o < (uint)keys.size(); o++) {
		uint oid = keys[o];
                SEObject* obj = const_cast<SEObject*>(oid2obj.value(oid));
		void* cache = const_cast<void*>(m_cached_props.value(oid));
		if (cache) {
		        m_cached_props.remove(oid);
		        obj->__clear_cache(cache); // may delete some references!
		}
		delete obj;   
		oid2obj.remove(oid);     
	}
	if (account_ref) {
		// update the reference timestamp so that it is still usable
		account_ref->m_ts = __m_ts; 
		account_ref->m_object =0;
	}
}


void SERootObject::PrintObjectStatistics()
{
        List_int keys = oid2obj.keys();    
        for (uint i = 0; i < (uint)keys.size(); i++) {
                uint oid = keys[i];
                SEObject* object = const_cast<SEObject*>(oid2obj.value(oid));
                printf("\t%d| %s oid %d\n", i+1, (const char*)object->getNamespace(), object->getOID());                
        }
}

bool SERootObject::FetchMultipleObjectsProps(uint modid, const List_ref objects, const List_uint properties)
{
        if (objects.size() == 0) return true; // protect from empty object list (may happen when cbl not in sync)
// in case of a multiget, the properties may be queried for objects that are not present in the table: add them?
        fetch(objects);
        return get_properties(get_current_thread(), modid, objects, properties) == Sid::Protocol::OK;
}

void* SERootObject::get_cached_prop(const uint& moduleid, const uint& objectid, const uint& property_id, uchar& kind)
{
        Sid::Mutex::ScopedLock sl(m_propcache_lock);        
        void* structptr = (void*)m_cached_props.value(objectid);
        if (!structptr) {
                structptr = prop_factory(moduleid);
                assert(structptr);
                m_cached_props.insert(objectid, structptr);
//                 printf("creating cache for oid %u\n\n", objectid);
        }
        
        Sid::Field* field = get_property_desc(moduleid, property_id);
        if (field) {
                kind = (uchar)field->kind();
                return field->offset(structptr);
        }
        
//         printf("returning 0 for prop %u\n", property_id);

        return 0;
}

void SERootObject::add_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id)
{
#ifndef PROPCACHE_DISABLED
        Sid::Mutex::ScopedLock sl(m_propcache_lock);
        void* structptr = (void*)m_cached_props.value(objectid);
//         printf("adding to propcache oid %u prop %u %p\n", objectid, property_id, structptr);
        if (structptr) {
                Sid::List_uint* cp = prop_cached_list(structptr, moduleid);
                if (!cp->contains(property_id)) cp->append(property_id);
        }
#endif
}

bool SERootObject::is_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id)
{
#ifndef PROPCACHE_DISABLED
        Sid::Mutex::ScopedLock sl(m_propcache_lock);
        void* structptr = (void*)m_cached_props.value(objectid);
//         printf("is_prop_cache oid %u prop %u %p\n", objectid, property_id, structptr);
        if (structptr) {
                Sid::List_uint* cp = prop_cached_list(structptr, moduleid);
                return cp->contains(property_id);
        }
#endif
        return false;
}

void SERootObject::del_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id)
{
#ifndef PROPCACHE_DISABLED
        Sid::Mutex::ScopedLock sl(m_propcache_lock);
        void* structptr = (void*)m_cached_props.value(objectid);
        if (structptr) {
                Sid::List_uint* cp = prop_cached_list(structptr, moduleid);
                for (uint i = 0; i < (uint)cp->size(); i++) {
                        if ((uint)cp->operator [](i) == property_id) {
                                Platform::list_remove(cp, i); 
                                break;
                        }
                }
        }
#endif
}


class SERootObject::BinClient : public Sid::Protocol::BinClient {
public:
	BinClient(Sid::Connection* connection, Field* descriptors, SERootObject* root)
	: Sid::Protocol::BinClient(connection, descriptors),
	  m_root(root)
	{
		m_readers['O'] = static_cast<Reader>(&BinClient::rd_ref);
	}

	Sid::Protocol::Status rd_ref(CommandInitiator* thread, void* val, uint)
	{
		uint oid;
		Sid::Protocol::Status status = rd_value(thread, oid);
		if (status == Sid::Protocol::OK) {
			m_root->__decode(static_cast<SEReference*>(val), oid);
		}
		return status;
	}
	SERootObject* m_root;
};

Sid::Protocol::BinClient* SERootObject::create_protocol(Sid::Connection* connection)
{
	return new BinClient(connection, get_descriptors(), this);
}

void SERootObject::__insert_object(SEObject* o, uint oid) 
{
	oid2obj.insert(oid, o);
	m_objmap_lock.unlock();
	// called from within the object constructor
	//  => unlock() allows sending requests in the
	// user code of the constructor as the object is 
	// now "fetched". Keeping the lock may create a 
	// deadlock on the response if the responsed is 
	// catched from another thread.
	// drawback is that it potentially creates a race condition
	// in user constructor
}

SEReference& SERootObject::__assign(SEReference& dst, const SEReference& src)
{
        Sid::Mutex::ScopedLock lprops(m_propcache_lock); // calling unref ...
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
        SEObject* o = __lookup(&src);
        if (o) o->__num_ref++;
	if (dst.m_root != 0 && dst.m_ts == __m_ts) {
		if (dst.m_object) __unref(&dst);
	}
	dst.m_oid    = src.m_oid; 
	dst.m_ts     = src.m_ts;
	dst.m_root   = src.m_root;
        dst.m_object = src.m_object;
	return dst;
}

SEObject* SERootObject::__lookup(const SEReference* reference) 
{
	if (reference->m_ts < __m_ts) return 0;
	if (reference->m_object) return reference->m_object;
	SEObject* o = (SEObject*)oid2obj.value(reference->m_oid);
	if (o) {
		o->__num_ref++;
		reference->m_object = o;
	}
	return o;
}

SEObject* SERootObject::__check_present(const SEReference* reference)  {
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	return __lookup(reference);
}

uint SERootObject::__construct_by_oid(SEReference* reference) 
{
	uint oid = reference->m_oid;
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	reference->m_ts = __m_ts;
	SEObject* o = (SEObject*)oid2obj.value(oid);
	if (o) { 
		if (o->__m_constructed && o->moduleID() != reference->m_modid) return 0; // if object is being constructed moduleID() may not be ready...
                                                                   // but intent is to detect an oid misused from a previous session
		o->__num_ref++;
		reference->m_object = o;
	}
	return oid;
}

bool SERootObject::__present(const SEReference* reference, const int& block)
{
        if (reference->m_oid == 0) return false; 
	m_objmap_lock.lock();
	SEObject* o = __lookup(reference);
        m_objmap_lock.unlock();
	// present is only true once the object if fully constructed (including user part)
        if (o && block) while (!o->__m_constructed) Thread::sleep(1);
	return o != 0;
}

SEObject* SERootObject::__fetch(const SEReference* reference) {
        if (reference->m_oid == 0) return 0; 
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	SEObject* o = __lookup(reference);
	if (!o) {
		o = factory(reference->m_oid, reference->m_modid);
		m_objmap_lock.lock();            // has been unlocked during construction
		if (reference->m_ts < __m_ts) return 0; // race condition safety check...
		o->__m_constructed  = 1;
		reference->m_object = o;
	}
	return o;
}

SEObject* SERootObject::__can_dispatch(const SEReference* reference) {
        SEObject* o = dispatch_all() ? __fetch(reference) : __check_present(reference); 
	// present is only true once the object if fully constructed (including user part)
        if (o) while (!o->__m_constructed) Thread::sleep(1);
	return o;
}

void SERootObject::__del_reference(SEReference* reference) { 
        Sid::Mutex::ScopedLock lprops(m_propcache_lock);
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	if (reference->m_ts < __m_ts) return;
	__unref(reference);
}

void SERootObject::__unref(SEReference* reference) {
	SEObject* o   = reference->m_object; 
	uint      oid = reference->m_oid;
	if (--o->__num_ref == 0) { 
		o->finalize(); // may call other del_reference, lock is reentrant... should not make request...
		void* cache = const_cast<void*>(m_cached_props.value(oid));
		if (cache) {
		        m_cached_props.remove(oid);
		        o->__clear_cache(cache); // may delete some references!
		}
		oid2obj.remove(oid); 
		delete o;
	}
}

void SERootObject::__decode(SEReference* r, uint oid) 
{
        Sid::Mutex::ScopedLock lprops(m_propcache_lock); // calling unref ...
	Sid::Mutex::ScopedLock lock(m_objmap_lock);
	if (r->m_root != 0 && r->m_ts == __m_ts) {
		if (r->m_oid == oid) return;
		if (r->m_object) __unref(r);
	}
	r->m_oid  = oid; 
	r->m_ts   = __m_ts;
	r->m_root = this;
        r->m_object = 0;
	if (oid) __lookup(r);
}

namespace Sid {
  void fetch(const List_ref& l) {
    for (int i = 0, e = l.size(); i < e; l[i++].fetch());
  }
}

// ideally would prefer Reference(int modid = 0) to only create default reference...
SEReference::SEReference(const uint& oid, const uint& modid, SERootObject* root) { 
	m_oid    = oid;
	m_modid  = modid;
	m_root   = root;
	m_ts     = 0;
	m_object = 0;
	if (oid) {
		if (!root) m_root = root = SERootObject::instance();
		m_oid = root->__construct_by_oid(this); 
		// m_oid set to 0 if invalid... can happen if reusing an oid from a previous session
	}
}

SEReference::~SEReference() {
	if (m_root && m_object && SERootObject::instance() == m_root) m_root->__del_reference(this);
} 

SEReference::SEReference(const SEReference& other) { 
	m_oid    = other.m_oid;
	m_modid  = other.m_modid;
	m_root   = other.m_root;
	m_ts     = other.m_ts;
	m_object = 0;
	if (m_root) m_object = m_root->__check_present(this);
}

SEObject* SEReference::fetch() const { 
        if (!m_root) return 0;
        return m_root->__fetch(this);
}

bool SEReference::present(const int& block) const {
        if (!m_root) return false;
        return m_root->__present(this, block);
}

SEObject* SEReference::can_dispatch() const {
        if (!m_root) return false;
        return m_root->__can_dispatch(this);
}

SEReference& SEReference::operator=(const SEReference& other) {
        if (this == &other) return *this;
	if (m_root) { 
		m_root->__assign(*this, other);
	} else {
		m_oid    = other.m_oid;
		m_modid  = other.m_modid;
		m_root   = other.m_root;
		m_ts     = other.m_ts;
		m_object = 0;
		if (m_root) m_object = m_root->__check_present(this);
	}
	return *this;
}

