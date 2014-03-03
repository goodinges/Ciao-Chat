
#ifndef __SKYPE_OBJECT_H
#define __SKYPE_OBJECT_H

#include <assert.h>
#include "SidPlatform.hpp"
#include "skype-clientsession.h"
#include "SidProtocolClientInterface.hpp"

#ifndef _WIN32
#define __se_unused __attribute__((unused))
#else
#define __se_unused
#endif

class SEObject;
class SERootObject;

using namespace Sid;

/** \struct PMAP
 *  \brief For internal use only. This struct represents a dictionary item that contains property key and property name as string.
 */
struct PMAP {
        const char* prop;
        uint key;
};

/** \struct EMAP
 *  \brief For internal use only. This struct represents a dictionary item that contains property ID and PMAP.
 */
struct EMAP {
        int prop;
        const PMAP* map;
};

/** \class SEPMap
 *  \brief For internal use only. Provides property / string map access. Used in getPropDebug.
 */
class SEPMap
{
public:
        SEPMap(const PMAP* pmap);
        String map(uint key) const;
        int map(const String& prop) const;

protected:
        const PMAP* p_map;
};

namespace Sid {
	template <class T> Sid::String tostring(const T&);
       inline Sid::String tostring(int v)    { return SEString::from(v); }
       inline Sid::String tostring(uint v)   { return SEString::from(v); }
       inline Sid::String enumtostring(const char* t, const PMAP* m, const uint& v) {
                Sid::String r(t);
                for (int i = 0; m[i].prop; i++)
                    if (m[i].key == v) return r+"."+m[i].prop;
                return r+"<Unknown:"+tostring(v)+">";
       }
}

/** \class SEReference
 *  \brief For internal use only. This is the parent class for all the SkypeKit class-specifix Ref classes (parent of AccountRef for example).
 */
class SEReference
{
public:
        SEReference(const unsigned int& oid = 0, const unsigned int& mid = 0, SERootObject* root = 0);
	SEReference(const SEReference& other);
	~SEReference();

	bool operator==(const SEReference& o) const  {
        	return (o.m_oid==m_oid && m_ts==o.m_ts) || (m_oid==0&&0==o.m_oid);
	}

	bool operator!=(const SEReference& o) const  {
        	return !operator==(o);
	}

        unsigned int get_oid() const {
                return m_oid;
        }
        unsigned int get_modid() const {
                return m_modid;
        }

	inline operator bool() const {
		return m_oid != 0;
	}

	inline SEObject* operator->() const { return fetch(); } 

	SEObject* fetch() const;
	SEObject* can_dispatch() const;

	bool present(const int& block = 1) const;

	SEReference& operator=(const SEReference& other);

private:
	uint              m_oid;
	SERootObject*     m_root;
	mutable SEObject* m_object;
	uint              m_ts;
	uint              m_modid;
	friend class SERootObject;
};

/** \struct DRef<  D, O >
 *  \brief Used for downcast the skypekit references to references to you own derived classes to acces your own functions.
 *  \ For usage, see examples on deriving your own SkypeKit classes.
 */
template<class D, class O>
struct DRef : O::Ref {
        typedef typename O::Ref ORef;
        DRef() {}
        DRef(const ORef& r) : ORef(r) { }
        DRef& operator=(const ORef& r) {
                ORef::operator=(r);
                return *this;
        }
        D* operator->() {
                return static_cast<D*>(ORef::operator->());
        }
};

/** \struct DRefs<  D, O >
 *  \brief Used for downcast the skypekit references contained in a list to references to you own derived classes to acces your own functions.
 *  \ For usage, see examples on deriving your own SkypeKit classes.
 */
template<class D, class O> 
struct DRefs : O::Refs {
        typedef typename O::Refs Base;
        typedef DRef<D,O> Ref;
        DRefs() { }
        DRefs(const Base& r) : Base(r) { }
        DRefs& operator=(const Base& r) {
                Base::operator=(r);
                return *this;
        }
        Ref& operator[](size_t n) const {
                return (Ref&) Base::operator[](n);
        }
};

/** \class SEObject skype-object.h "skype-object.h"
 *  \brief SEObject is a base class for all API modules
 */
class SEObject
{
        friend class SERootObject;
        friend class SEClientSession;

protected:
        SERootObject *root;
        int   __m_constructed;
        unsigned int object_id;
        unsigned int __num_ref;

        void __safe_inc_ref();
        unsigned __safe_dec_ref();
        virtual void __clear_cache(void*) = 0;

        virtual unsigned int moduleID() const = 0;
        virtual String getNamespace() const = 0;
        virtual const PMAP* getPMap() const = 0;
        virtual const EMAP* getEMap() const = 0;

        virtual void OnChange(__se_unused int /*prop*/) {}
        virtual void finalize() {}

public:
        unsigned int getOID() const {
                return object_id;
        }

        SEObject(unsigned int oid, SERootObject* s);
        virtual ~SEObject();
        
        List_String getPropDebug(int prop, const String& value);
        List_String getPropDebug(int prop, const uint value);

        /** Send get request for multiple properties in one protocol message.
         * Once the function returns, the properties can be obtained with SEObject::Get***Prop()
         */
        bool FetchProps(const List_uint& properties);

        /** get property, property value is converted to string if possible*/
        String GetProp(int property);
        Dict_int GetProps(const List_uint& properties);

        /** get string property*/
        String GetStrProp(uint property);
        /** get boolean property */
        bool GetBoolProp(uint property);
        /** get uint, enum property*/
        uint GetUintProp(uint property);
        /** get binary property */
        Binary GetBinProp(uint property);
        /** get ref object */
        SEReference GetObjProp(uint property);

private:// non assignable
        SEObject(const SEObject&);
        SEObject& operator=(const SEObject&);
};


/** \class SERootObject skype-object.h "skype-object.h"
 *  \brief SERootObject is the root module and entry point for API
 */
class SERootObject : public SEClientSession
{
        friend class SEObject;
        friend class SEReference;

protected:
        static SERootObject* m_instance;
        Dict_SEObj      oid2obj;
        Dict_ptr        m_cached_props;
        unsigned        __m_ts;
        Mutex           m_objmap_lock;

        void insert_object(SEObject* obj);

        virtual SEObject* factory(const uint& oid, const uint& modid) = 0;
        virtual void* prop_factory(const uint& moduleid) = 0;
        virtual List_uint* prop_cached_list(void* propstruct, const uint& moduleid) = 0;

        virtual bool is_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id);
        virtual void add_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id);
        virtual void del_prop_cached(const uint& moduleid, const uint& objectid, const uint& property_id);
        virtual void* get_cached_prop(const uint& moduleid, const uint& objectid, const uint& property_id, uchar& kind);
        
//        unsigned int __get_ts();
        void cleanup(int, SEReference* account_ref = 0);
        virtual Sid::Protocol::BinClient* create_protocol(Sid::Connection* connection);
        class BinClient;                
public:
        static SERootObject* instance() {
                return m_instance;
        }

        SERootObject(int num_threads, Sid::Field* descriptors);
        virtual ~SERootObject();

        void PrintObjectStatistics();
        
        bool SkypekitConnected() { return is_server_connected(); }
        
        /** Send get request for multiple properties in one protocol message.
         * Once the function returns, the properties can be obtained with SEObject::Get***Prop()
         */
        bool FetchMultipleObjectsProps(uint modid, List_ref objects, List_uint properties);

private:
	void __insert_object(SEObject* o, uint oid);
	SEReference& __assign(SEReference& dst, const SEReference& src);
	SEObject* __lookup(const SEReference* reference);
	SEObject* __check_present(const SEReference* reference);
	uint __construct_by_oid(SEReference* reference); 
	bool __present(const SEReference* reference, const int& block);
	SEObject* __fetch(const SEReference* reference);
	void __del_reference(SEReference* reference);
	void __unref(SEReference* reference);
	void __decode(SEReference* r, uint oid); 
protected:
	SEObject* __can_dispatch(const SEReference* reference);
};

namespace Sid {
  void fetch(const List_ref&);
}

#endif

