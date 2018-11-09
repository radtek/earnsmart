/* 
 * File:   IObject.h
 * Author: santony
 *
 * Created on July 9, 2012, 7:29 AM
 */

#ifndef IOBJECT_H
#define	IOBJECT_H

#include "../ReturnCodes.h"
#include "../seglobaltypedefs.h"
#include <vector>
#include <functional>
#include "CDate.h"
#include <jsoncpp/json/json.h>
#include <mutex>

#define GBLEXP __attribute__ ((visibility ("default")))

/*
 * Error codes are defined the following way in a 32-bit number (HRESULT)
 * 
 *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
 *------------------------------------------------------------------------------------------
 *  E| |C|          |     Facility     | T |                    Code                        |
 * 
 * Facility code of this library is 0
 * 
 */

namespace se
{
    
    /*
        * When implemented, assists with automatic reference counting 
        * and memory deallocations.
        */ 
    
    struct OmniType;
    
    struct IObject 
    {
        virtual UID get_Id() = 0;
        virtual void set_Id(UID id) = 0;
        virtual void AddRef() = 0;
        virtual void DecRef() = 0;
        virtual bool Equals(IObject* other) = 0;
        virtual bool set_Property(CSTR key, const OmniType& value) = 0;
        virtual OmniType get_Property(CSTR key) const = 0;
    };
    
    struct IJsonObject : public IObject
    {
    public:
        virtual Json::Value& get_Root() = 0;
    };
    
    
    template<typename T = IObject>
    struct __SmartPointer
    {
    public:
        T* _ptr ;

        __SmartPointer() : _ptr(0)
        {

        }

        __SmartPointer(T* ptr) : _ptr(ptr)
        {
            if (ptr)
                ptr->AddRef();
        }

        __SmartPointer(const __SmartPointer<T>& p) : _ptr(p._ptr)
        {
            if (_ptr)
                _ptr->AddRef();
        }

        __SmartPointer(__SmartPointer<T>&& p) : _ptr(p._ptr)
        {
            if (p._ptr)
            {
                _ptr  = p._ptr;
                p._ptr = nullptr;
            }
        }
        
        __SmartPointer<T>& operator=(__SmartPointer<T>&& p)
        {
            if (p._ptr)
            {
                this->_ptr = p._ptr;
                p._ptr = nullptr;
            }
            return *this;
        }

        __SmartPointer<T>& operator=( const __SmartPointer<T>& p)
        {
            if (p._ptr)
            {
                this->_ptr = p._ptr;
                _ptr->AddRef();
            }
            return *this;
        }

        ~__SmartPointer()
        {
            Clear();
        }

        void Clear()
        {
            T* pTmp = _ptr; _ptr = 0;
            if (pTmp)
                pTmp->DecRef();
        }

        // cast operator
        operator T*() const
        {
            return _ptr;
        }

//        // cast operator
//        template<typename TRet>
//        operator TRet*() const
//        {
//            return _ptr;
//        }
//
        // dereference operator
        T& operator*() const
        {
            return *_ptr;
        }

        // indirect access to internal pointer
        T* operator->() const
        {
            return _ptr;
        }

        // Address of operator.
        T** operator &()
        {
            if (_ptr)
                Clear();
            return &_ptr;
        }

        // assign by adding reference
        void Assign(T* p)
        {
            if (_ptr) Clear();
            _ptr = p;
            if (_ptr)
                _ptr->AddRef();
        }

        // attach, but don't addref
        void Attach(T* p)
        {
            if (_ptr) Clear();
            _ptr = p;
        }

        // detach, by not dec-refing, and return pointer.
        T* Detach()
        {
            T* p = _ptr; _ptr = 0;
            return p;
        }

        // assignment operator.
        T* operator=(T* p)
        {
            Assign(p);
            return _ptr;
        }

        // copy by addref-ing.
        template<class I>
        bool CopyTo(I** pp)
        {
            *pp = _ptr;
            if (_ptr) _ptr->AddRef();
            return _ptr != 0;
        }

    };    
    
    template<typename T = IObject>
    struct __SharedPointer
    {
    public:
        T* _ptr ;
        
        __SharedPointer(){}

        __SharedPointer(T* p)
        {
            Assign(p);
        }

        __SharedPointer(const __SmartPointer<T>& p)
        {
            Assign(p._ptr);
        }

        __SharedPointer(const __SharedPointer<T>& p)
        {
            Assign(p._ptr);
        }
        
        __SharedPointer(__SharedPointer<T>&& p)
        {
            Assign(p._ptr);
        }
//
//        __SharedPointer(T*&& p) 
//        {
//            Assign(p);
//        }
        
        __SharedPointer<T>& operator=(__SharedPointer<T>&& p)
        {
            Assign(p._ptr);
            return *this;
        }

        __SharedPointer<T>& operator=(T*&& p)
        {
            Assign(p);
            return *this;
        }

        ~__SharedPointer()
        {
            Clear();
        }

        void Clear()
        {
            T* pTmp = _ptr; _ptr = 0;
            if (pTmp)
                pTmp->DecRef();
        }

        // cast operator
        operator T*() const
        {
            return _ptr;
        }

//        // cast operator
//        template<typename TRet>
//        operator TRet*() const
//        {
//            return _ptr;
//        }
//
        // dereference operator
        T& operator*() const
        {
            return *_ptr;
        }

        // indirect access to internal pointer
        T* operator->() const
        {
            return _ptr;
        }

        // Address of operator.
        T** operator &()
        {
            if (_ptr)
                Clear();
            return &_ptr;
        }

        // assign by adding reference
        void Assign(T* p)
        {
            _ptr = p;
            if (_ptr)
                _ptr->AddRef();
        }

        // attach, but don't addref
        void Attach(T* p)
        {
            if (_ptr) Clear();
            _ptr = p;
        }

        // detach, by not dec-refing, and return pointer.
        T* Detach()
        {
            T* p = _ptr; _ptr = 0;
            return p;
        }

        // assignment operator.
        T* operator=(T* p)
        {
            Assign(p);
            return _ptr;
        }

        // copy by addref-ing.
        template<class I>
        bool CopyTo(I** pp)
        {
            *pp = _ptr;
            if (_ptr) _ptr->AddRef();
            return _ptr != 0;
        }

    };        

#define SP __SmartPointer
#define RP __SharedPointer    
    
    enum class UserRole : U32 
    {
        Anonymous      = 0x1,
        Standard       = 0x2,
        Licensed       = UserRole::Standard | ((U32)UserRole::Standard << 1),
        Internal       = 0x4,
        Admin          = UserRole::Internal | ((U32)UserRole::Internal << 1)
    };
    
    
    struct ISession;
    
    struct IErrorDescription
    {
    public:
        typedef std::map<SE_CODE, CSTR> CodeMessages;
        typedef CodeMessages::value_type CodeMessageItem;
        
    protected:
        static CodeMessages msgs;

    public:
        virtual CSTR get_CodeMessage(SE_CODE errorCode);
    
        /* Do not use directly. Internal use only. */
        static GBLEXP void __loadMessages__(const std::initializer_list<CodeMessageItem>& list);
        
    protected:
        
    };
    
    typedef U8 NamedObjectNamingType;

    struct INamedObject : public IObject
    {
        virtual NamedObjectNamingType get_TypeId()=0;
    };
    
    struct INamedObjectList : public IObject
    {
        // Returned as named type from an efficient container keyed by name.
        virtual RP<INamedObject> get(NamedObjectNamingType name) = 0;
        virtual SE_CODE set(NamedObjectNamingType name, INamedObject* instance) = 0;
        virtual SE_CODE remove(NamedObjectNamingType name)=0;
    };
    
    namespace config
    {
        typedef enum enEnvironment
        {
            ENV_Dev = 0x1,
            ENV_Staging = 0x2,
            ENV_Prod = 0x4,
            ENV_All = ENV_Dev | ENV_Staging | ENV_Prod
        } ENV;
        
        #define GLOBALREGIONID  32767
        #define USAREGIONID 32766
        
        struct IConfigItem : public IObject
        {
            virtual DATATYPES get_Type() = 0;
            virtual CSTR get_Name() = 0;
            virtual CSTR get_Value() = 0;
            virtual IConfigItem* get_Parent() = 0;
            virtual void set_Name(CSTR name) = 0;
            virtual void set_Value(const OmniType& value) = 0;
            static SE_CODE GBLEXP Create(ISession* ps, IConfigItem**);
        };
        
        struct IConfigItemList : public IObject
        {
            virtual IConfigItem* get_Item(UID configId) = 0;
            virtual IConfigItem* get_Item(CSTR configName) = 0;
        };
        
        struct IConfigurator : public IObject
        {
            //virtual ErrorId InitLog() = 0;
            //virtual ErrorId InitDb() = 0;
            virtual CSTR get_ExecutableDir() = 0;
            virtual CSTR get_ExecutableFile() = 0;
            virtual ENV get_Environment() = 0;
            virtual ErrorId get_Settings(U16 regionId, CSTR parent, IConfigItemList** ppOut) = 0;
            virtual SE_CODE set_Settings(U16 regionId, CSTR parent, IConfigItem* ppOut) = 0;
            virtual bool get_FunctionCacheSettings(CSTR funcName) = 0;
            virtual bool get_GlobalCacheEnabled() = 0;
            virtual bool get_IsInMaintenanceMode() = 0;
            virtual CSTR get_ConfigFilePath() = 0;
            virtual const Json::Value& get_ConfigRoot() const = 0;
            virtual MARKETDATAPROVIDERS get_DefaultMarketDataProvider() const = 0;
        };
    }
    
    struct IApp;
    struct ISessionActivity;
        
    /* global session instance */
    struct ISession : public IObject, IErrorDescription
    {
        virtual RP<config::IConfigurator> get_Configs() = 0;
        virtual UID get_UserId() = 0;
        virtual CSTR get_UserName() = 0;
        virtual UserRole get_UserRole()=0;
        virtual RP<IString> get_UserRoleStr() = 0;
        virtual bool IsUserHasRole(UserRole)=0;
        virtual bool IsUserRoleHigherThan(UserRole)=0;
        virtual ErrorId get_Data(I32 key, LPVOID** outVal) = 0;
        virtual RP<INamedObject> get_Object(NamedObjectNamingType) = 0;
        virtual void set_Object(NamedObjectNamingType typeName, INamedObject* pObject) = 0;
        virtual std::recursive_mutex& get_LockObject() = 0; 
        virtual RP<IApp> get_App() = 0;
        virtual bool get_Remember() = 0;
        virtual void set_Remember(bool value) = 0;
        virtual CSTR get_UniqueId() = 0;
        virtual void set_UniqueId(CSTR) = 0; 
        virtual bool get_IsAuthenticated() = 0;
        virtual POINTER get_Tag() = 0;
        virtual void set_Tag(POINTER tag) = 0;
        // Logout the session. The session now becomes an anonymous session. 
        virtual SE_CODE Logout() = 0;
        
    };
    
    // interface that implement a storable(create or edit) and removable object.
    struct ISDObject 
    {
        virtual SE_CODE Store() = 0;
        virtual SE_CODE Delete() = 0;
    };

    struct IJSObject 
    {
        virtual const Json::Value& get_JsObject() = 0;
    };
    

    template<typename T>
    struct IEnumerator : public IObject
    {
        virtual void Reset() = 0;
        virtual bool MoveNext() = 0;
        virtual T get_Current() = 0;
    };

    template<typename TEnum>
    struct IEnumerable : public IObject
    {
        virtual ErrorId get_Enumerator(TEnum** outVal) = 0;
    };

    template<typename T>
    struct IEnumerator1 : public IObject
    {
        virtual void Reset() = 0;
        virtual bool MoveNext() = 0;
        virtual T* get_Current() = 0;
    };

    template<typename T>
    struct IEnumerable1 : public IObject
    {
        virtual ErrorId get_Enumerator(IEnumerator1<T>** outVal) = 0;
    };
    
    template<typename TItem>
    struct IFinder 
    {
        virtual SE_CODE Find( std::function<bool(const TItem&)> func, TItem* pOut) = 0;
    };
    
    // Useful to provided additional properties to an IEnumerable1
    template<typename TItem>
    struct IROList1 
    {
        virtual SIZE get_Size() = 0;
    };
    
    template<typename TItem>
    struct IList1 : public IROList1<TItem>
    {
        
    };
    
    struct ISerialization;
    struct IDeserialization;
    struct IString;
    
    template<typename TOutputType = IString>
    struct ISerializedData : public IObject
    {
        virtual void set_TypeName(CSTR) = 0;
        virtual CSTR get_TypeName() = 0;
        virtual void Add(CSTR propertyName, CSTR propertyValue) = 0;
        virtual void Add(CSTR propertyName, const BOOL propertyValue) = 0;
        virtual void Add(CSTR propertyName, const CHR propertyValue) = 0;
        virtual void Add(CSTR propertyName, const I16 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const I32 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const I64 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const U8 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const U16 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const U32 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const U64 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const R4 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const R8 propertyValue) = 0;
        virtual void Add(CSTR propertyName, const IString* propertyValue) = 0;
        virtual void Add(CSTR propertyName, ISerialization* propertyValue) = 0;
        virtual void AddList(CSTR propertyName, ISerialization* propertyValue) = 0;
        virtual void AddListItem(ISerialization* value) = 0;
        virtual BOOL getBool(CSTR propertyName) = 0;
        virtual CSTR getString(CSTR propertyName) = 0;
        virtual CHR getChar(CSTR propertyName) = 0;
        virtual I16 getShort(CSTR propertyName) = 0;
        virtual I32 getInt32(CSTR propertyName) = 0;
        virtual I64 getInt64(CSTR propertyName) = 0;
        virtual U8  getUChar(CSTR propertyName) = 0;
        virtual U16 getUShort(CSTR propertyName) = 0;
        virtual U32 getUInt32(CSTR propertyName) = 0;
        virtual U64 getUInt64(CSTR propertyName) = 0;
        virtual R4 getSingleReal(CSTR propertyName) = 0;
        virtual R8 getDoubleReal(CSTR propertyName) = 0;
        virtual ErrorId getString(CSTR propertyName, IString** ppOut) = 0;
        virtual ErrorId getObject(CSTR propertyName, IDeserialization* pInOut) = 0;
        virtual ErrorId get_Output(TOutputType** ppOut) = 0;
        virtual bool get_IsFormatted() = 0;
        virtual void Reset() = 0;
    };
    
    struct ISerialization
    {
        virtual void Serialize(ISerializedData<IString>* pIn) = 0;
    };
    
    struct IDeserialization
    {
        virtual ErrorId Deserialize(ISerializedData<IString>* pIn) = 0;
    };
    
    template<typename T>
    struct IROList : 
        public IEnumerable< IEnumerator<T> >, 
        public ISerialization, 
        public IDeserialization
    {
        typedef std::vector<T> VECTOR;
        virtual SIZE get_Count() = 0;
        virtual ErrorId get_Item(SIZE index, T* pOut) = 0;
        virtual typename VECTOR::iterator begin() = 0;
        virtual typename VECTOR::iterator end() = 0;
        virtual void Sort(std::function<bool(T, T)> sortfunc) = 0;
    };
    
    template<typename T>
    struct IList : public IEnumerable< IEnumerator<T> >, public ISerialization, public IDeserialization
    {
        typedef std::vector<T> VECTOR;
        virtual void Add(T item) = 0;
        virtual bool Remove(T item) = 0;
        virtual bool RemoveItem(SIZE position) = 0;
        virtual SIZE get_Count() = 0;
        virtual ErrorId get_Item(SIZE index, T* pOut) = 0;
        virtual typename VECTOR::iterator begin() = 0;
        virtual typename VECTOR::iterator end() = 0;
        virtual bool HasItem(T item) = 0;
    };
    
    template<typename T = IObject>
    struct IROSPList : public IEnumerable< IEnumerator<T*> >, public ISerialization, public IDeserialization
    {
        typedef std::vector<SP<T>> VECTOR;
        virtual SIZE get_Count() = 0;
        virtual ErrorId get_Item(SIZE index, T** pOut) = 0;
        virtual bool HasItem(UID uid) = 0;
        virtual SE_CODE get_ItemById(UID uid, T** ppOut) = 0;
        virtual typename VECTOR::iterator begin() = 0;
        virtual typename VECTOR::iterator end() = 0;
        virtual void Sort(std::function<bool(T*, T*)> sortfunc) = 0;
    };
    
    template<typename T = IObject>
    struct ISPList : public IEnumerable< IEnumerator<T*> >, public ISerialization, public IDeserialization
    {
        typedef std::vector< SP<T> > VECTOR;
        virtual void Add(T* item) = 0;
        virtual bool Remove(T* item) = 0;
        virtual bool RemoveById(UID id) = 0;
        virtual bool HasItem(UID uid) = 0;
        virtual SIZE get_Count() = 0;
        virtual ErrorId get_Item(SIZE index, T** pOut) = 0;
        virtual SE_CODE get_ItemById(UID uid, T** ppOut) = 0;
        virtual typename VECTOR::iterator begin() = 0;
        virtual typename VECTOR::iterator end() = 0;
    };
    
    template<typename TKey, typename TValue>
    struct KeyValuePair
    {
    public:
        TKey Key;
        TValue Value;
    };
    
    // basic structure used to return id and name properties.
    struct IIdName : public IObject, public ISerialization
    {
        virtual CSTR get_Name() = 0;
        virtual void set_Name(CSTR name) = 0;
    };
    
    struct IIdNameList : public IROSPList<IIdName>
    {
    };
    
    template<typename TKey, typename TValue>
    struct IDictionary : public IEnumerable< KeyValuePair<TKey, TValue> >
    {
        virtual void Add(TKey key, TValue item) = 0;
        virtual bool Remove(TKey item) = 0;
        virtual SIZE get_Count() = 0;
        virtual ErrorId get_Item(TKey key, TValue* pOut) = 0;
    };
    
    struct IAddress : public IObject , public ISerialization, public IDeserialization
    {
        // optional name field for address
        virtual CSTR get_Name() = 0;
        virtual void set_Name(CSTR) = 0;
        
        virtual CSTR get_Street() = 0;
        virtual CSTR get_Street2() = 0;
        virtual CSTR get_City() = 0;
        virtual CSTR get_State() = 0;
        virtual CSTR get_Country() = 0;
        virtual CSTR get_PostalCode() = 0;

        virtual void set_Street(CSTR value) = 0;
        virtual void set_Street2(CSTR value) = 0;
        virtual void set_City(CSTR value) = 0;
        virtual void set_State(CSTR value) = 0;
        virtual void set_Country(CSTR value) = 0;
        virtual void set_PostalCode(CSTR value) = 0;
        
        static ErrorId GBLEXP Create(se::ISession*, IAddress**);
    };
    
    struct IString : public IObject
    {
        virtual CSTR get_Buffer() = 0;
        virtual void set_Buffer(CSTR str) = 0;
        virtual SIZE get_Length() = 0;
        virtual bool get_IsEmpty() = 0;
        virtual void Clear() = 0;
        virtual void Assign(CSTR str) = 0;
        virtual void Replace(CSTR replace, CSTR with) = 0;
        virtual SIZE Find(CSTR str, SIZE startPosition = 0) = 0;
        virtual SIZE ReverseFind(CSTR str, SIZE startPosition = 0) = 0;
        virtual ErrorId Substring(SIZE pos, SIZE count, IString** outVal) = 0;
        virtual I32 Compare(CSTR otherString) = 0;

        virtual void Append(CSTR str) = 0;
//        virtual void Append(I8 data) = 0;
//        virtual void Append(U8 data) = 0;
//        virtual void Append(I16 data) = 0;
//        virtual void Append(U16 data) = 0;
//        virtual void Append(I32 data) = 0;
//        virtual void Append(U32 data) = 0;
//        virtual void Append(I64 data) = 0;
//        virtual void Append(U64 data) = 0;
//        virtual void Append(R4 data) = 0;
//        virtual void Append(R8 data) = 0;
        
        static ErrorId GBLEXP Create(CSTR defvalue, IString** ppOut);
        
        
    };
    
    typedef struct DateTime
    {
        DateTime();
        short Year;
        short Month;
        short Day;
        short Hour;
        short Minute;
        short Second;
        short Millisecond;
        SP<IString> Zone;
        
        bool IsEmpty() ;
        ErrorId ToString(IString** ppOut);
        static DateTime Now();
        // specify format in the form "%d.%m.%Y %H:%M:%S" 
        // where d, m, y , H, M and S can be position any place and delimited by any char.
        static DateTime Parse(CSTR dateasstring, CSTR format);
        
        friend std::ostream& operator<<(std::ostream& os, const DateTime& dp);
        
        static DateTime Empty();
    
        DateTime(const time_t& time);
        DateTime(const DateTime& cp);
        DateTime(DateTime&& mv);
        
        DateTime& operator=(const DateTime& cp);
        DateTime& operator=(DateTime&& mv);
      
        DateTime& operator=(const time_t& tm);
        
        
    } DATETIME;

    
    
    struct OmniType : public ISerialization
    {
    private:
        SP<IObject> _pstr;
        union unpt
        {
            POINTER _pointer;
            I32 _i32Value;
            U32 _u32Value;
            U64 _u64value;
            R4 _r4Value;
            R8 _r8Value;
            I64 _i64Value;
            CSTR _stringPointer;     // note that string buffer is allocated in _pstr variable.
        };

    public:        
        unpt Value; // value is held here.
        DATATYPES dt;   // data type held

        template<typename TBeg, typename TEnd>
        OmniType(TBeg begin, TEnd end)
        {
            //TODO : For Arrays
        }
        OmniType();
        ~OmniType();
        OmniType(const OmniType&);
        OmniType(const R8&);
        OmniType(const R4&);
        OmniType(const I64&);
        OmniType(const U64&);
        OmniType(const I32&);
        OmniType(const U32&);
        OmniType(CSTR);
        OmniType(const POINTER);
        OmniType(IObject*);
        
        OmniType(OmniType&&);
        const OmniType& operator=(OmniType&&);
        
        const OmniType& operator=(const OmniType&);
        
        const OmniType& operator=(R8 v);
        const OmniType& operator=(R4 v);
        const OmniType& operator=(CSTR v);
        const OmniType& operator=(I64 v);
        const OmniType& operator=(I32 v);
        const OmniType& operator=(U64 v);
        const OmniType& operator=(U32 v);
        const OmniType& operator=(POINTER v);
        const OmniType& operator=(IObject* v);
        
        friend OmniType operator+(const OmniType& l, const OmniType& r) ;
        friend OmniType operator-(const OmniType& l, const OmniType& r) ;
        friend OmniType operator*(const OmniType& l, const OmniType& r) ;
        friend OmniType operator/(const OmniType& l, const OmniType& r) ;
        
        OmniType& operator+=(const OmniType& r);
        OmniType& operator-=(const OmniType& r);
        OmniType& operator*=(const OmniType& r);
        OmniType& operator/=(const OmniType& r);
        
        bool operator==(const OmniType& v) const;

        operator CSTR() const;
        operator R8() const;
        operator R4() const;
        operator I32() const;
        operator I64() const;
        operator U64() const;
        operator U32() const;
        operator POINTER() const;
        operator IObject*() const;
        
        SE_CODE toString(IString**) const;
        
        bool IsEmpty() const;
        
        void SetEmpty();
        
        static OmniType Empty();

        virtual void Serialize(ISerializedData<IString>* pIn);

        friend std::ostream& operator<<(std::ostream& os, const OmniType& ot);
        
    };
    
    #define QUERYFUNC(T) std::function<CSTR (T::QUERYFLDS, LGX&, BOOL& )>

    
    /*
     * Occassionally a remote application's 
     * session activity information 
     * is transferred to the server using this structure.
     */
    struct ISessionActivity : public IObject
    {
        virtual DATETIME& get_Time() = 0;
        virtual void set_Time(DATETIME& ts) = 0;

        virtual CSTR get_UserAgent() = 0;
        virtual CSTR get_RemoteAddress() = 0;
        virtual CSTR get_RemoteUser() = 0;
        virtual CSTR get_RemoteIdent() = 0;
        virtual CSTR get_RemoteHost() = 0;
        virtual CSTR get_HttpReferer() = 0;

        virtual void set_UserAgent(CSTR) = 0;
        virtual void set_RemoteAddress(CSTR) = 0;
        virtual void set_RemoteUser(CSTR) = 0;
        virtual void set_RemoteIdent(CSTR) = 0;
        virtual void set_RemoteHost(CSTR) = 0;
        virtual void set_HttpReferer(CSTR) = 0;

    };

    
    extern "C"
    {
        ErrorId CreateAddress(ISession*, IAddress** ppOut);
    }
    
    // utility class that maps out regular quarter to company quarters and vice versa
    // caches all different unique financial end dates.
    struct IFinancialPeriod : public IObject
    {
        // provided the financial month and last day of a company
        // and a natural period of any quarter and year, 
        // returns mapping company period. 
        // example : A company that reports annually on June 30,
        // a natural period of 4Q/2013 is equivalent to 2Q/2014.
        // note that the month set in returned dataperiod is the last month of the quarter.
        // so for first quarter of jan-feb-mar (natural period), the month is set to 3.
        virtual SE_CODE get_CompanyPeriod(const DATAPERIOD& AnyNaturalPeriod, DATAPERIOD* pCompanyPeriod) = 0;
        // provided the financial month and last day of a company
        // and a company period of any quarter and year, 
        // returns mapping natural period. 
        // example : A company that reports annually on June 30,
        // a company period of 4Q/2013 is equivalent to 2Q/2013.
        // note that the month set in returned dataperiod is the last month of the quarter.
        // so for first quarter of jan-feb-mar (company period), the month is set to 3.
        virtual SE_CODE get_NaturalPeriod(const DATAPERIOD& AnyCompanyPeriod, DATAPERIOD* pNaturalPeriod) = 0;
        
        // find if given date has a matching fiscal period in this instance.
        virtual bool FindFiscalPeriod(const CDate& dtIn, DATAPERIOD* dpOut) = 0;
        virtual bool FindFiscalPeriod(const DATAPERIOD dpIn, DATAPERIOD* dpOut) = 0 ;
    
        virtual const DATAPERIOD& get_LatestPeriod() = 0;
        
        // create an empty instance for general use.
        static SE_CODE GBLEXP Create(ISession*, IFinancialPeriod**);

    };

 
    namespace securities
    {
        
        enum class SECTYPES
        {
            Equity = 1,
            Index = 2,
            Future = 4,
            Option = 8,
            Currency = 16
        };
        
        struct IExchange : public IIdName
        {
            virtual CSTR get_Code() = 0;
            virtual void set_Code(CSTR code) = 0;
            virtual CSTR get_Country() = 0;
            virtual void set_Country(CSTR country) = 0;
            virtual CSTR get_TimeZone() = 0;
            virtual void set_TimeZone(CSTR zone) = 0;
            static SE_CODE GBLEXP Create(ISession*, IExchange** ppOut);
        };
        
        typedef IROSPList<IExchange> IExchangeList;
        
        /* a description of a security */
        struct ISecurityDescription : public IObject, public ISerialization
        {
            // ESid is the unique identifier of this security within an exchange.
            virtual UID get_ESid() = 0;
            virtual CSTR get_Symbol() = 0;
            virtual CSTR get_ExchCode() = 0;
            virtual CSTR get_Currency() = 0;
            virtual CSTR get_Country() = 0;
            virtual SECTYPES get_Type() = 0;

            virtual void set_ESid(UID esid) = 0;
            virtual void set_Symbol(CSTR symbol) = 0;
            virtual void set_ExchCode(CSTR exchangeCode) = 0;
            // Optional.
            virtual void set_Currency(CSTR currency) = 0;
            virtual void set_Country(CSTR country) = 0;
            virtual void set_Type(SECTYPES typeId) = 0;
            
            static SE_CODE GBLEXP Create(ISession* ps, ISecurityDescription**);
            
        };
        
        struct ISecurityDescriptionList : public ISPList<ISecurityDescription>
        {
            static SE_CODE GBLEXP Create(ISession* ps, ISecurityDescriptionList**);
        
            
        
        };
        

    }
    
    enum class SearchMethod
    {
        Exact,
        Regex,
        SqlPattern,
    };
    
    struct ISearcheable 
    {
        virtual SE_CODE SearchModule(CSTR searchText, SearchMethod searchMethod, Json::Value& output) = 0;
    };
    
    namespace threading
    {
        struct ITask : public IObject
        {
            virtual void Execute() = 0;
        };
        
        struct IThreadPool : public IObject
        {
            virtual void RunTask(ITask* task) = 0;
            virtual void ExecuteFunction(typename std::function<void(POINTER)> function , POINTER state) = 0;
        };
        
        typedef enum enThreadPoolTypes
        {
            TPT_SynchronousPool,    // same thread as caller executes the task.
            TPT_ParallelPool,   // multiple threads from a thread pool handles tasks simultaneously
            TPT_ConcurrentExecution   // only one thread handles all tasks.
        } TPT;
        
    }
    
    struct IApp : public IObject, public IErrorDescription
    {
        virtual RP<se::config::IConfigurator> get_Configurator() = 0;
        virtual SE_CODE get_Configurator(se::config::IConfigurator** ppOut) = 0;
        virtual SE_CODE CreateSession(CSTR userId, CSTR password, ISession** ppOut) = 0;
        virtual SE_CODE OpenSession(CSTR sessionId, ISession** ppOut) = 0;
        virtual SE_CODE CloseSession(ISession* pIn) = 0 ;
        virtual SE_CODE get_ThreadPool(se::threading::TPT pooltype, se::threading::IThreadPool** ppOut) = 0;
    };
    
    
}

#endif	/* IOBJECT_H */

