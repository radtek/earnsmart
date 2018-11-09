#include <dirent.h>
#include "seglobalinternal.h"
#include "IObject.h"
#include "../seglobaltypedefs.h"
#include "exceptions.h"
#include <sstream>
#include <iomanip>
#include <limits>
#include <math.h>
#include <locale>
#include <string.h>

namespace se
{
    OmniType::OmniType() : dt(DT_NONE)
    {
        //Value._i64Value = std::numeric_limits<long long>::min();
        Value._pointer = nullptr;
    }
    
    OmniType::OmniType(const OmniType& o) 
    {
        if (o.IsEmpty())
        {
            SetEmpty();
            return;
        }
        this->Value = o.Value;
        dt = o.dt;
        if (o._pstr)
            _pstr.Assign(o._pstr);
    }

    OmniType::OmniType(OmniType&& o)
    {
        if (o.IsEmpty())
        {
            SetEmpty();
            return;
        }
        this->Value = o.Value;
        dt = o.dt;
        _pstr.Assign(o._pstr);
    }
    

    OmniType::OmniType(const R8& v) 
    {
        Value._r8Value = v;
        dt = DT_R8;
    }

    OmniType::OmniType(const R4& v) 
    {
        Value._r4Value = v;
        dt = DT_R4;
    }

    OmniType::OmniType(const I64& v) 
    {
        Value._i64Value = v;
        dt = DT_I64;
    }

    OmniType::OmniType(const U64& v) 
    {
        Value._u64value = v;
        dt = DT_U64;
    }
    
    OmniType::OmniType(const I32& v) 
    {
        Value._i32Value = v;
        dt = DT_I32;
    }
    
    OmniType::OmniType(const U32& v) 
    {
        Value._u32Value = v;
        dt = DT_U32;
    }
    
    OmniType::OmniType(const char* v)
    {
        SP<IString> pstr;
        IString::Create(v, &pstr);
        this->Value._stringPointer = pstr->get_Buffer();
        _pstr = (IObject*)pstr;
        dt = DT_CSTR;
    }
    
    OmniType::OmniType(const void* v)
    {
        Value._pointer = const_cast<void*>(v);
        dt = DT_POINTER;
    }
    
    OmniType::OmniType(IObject* v)
    {
        _pstr.Assign(v);
        dt = DT_OBJECT;
    }
    
    OmniType::~OmniType()
    {
        _pstr.Clear();
    }
    
    bool OmniType::IsEmpty() const
    {
        switch (this->dt)
        {
            case DT_R8:
                return this->Value._r8Value == 0.;
            case DT_R4:
                return this->Value._r4Value == 0.f;
            case DT_I64:
                return this->Value._i64Value == 0ll;
            case DT_U64:
                return this->Value._u64value == 0ull;
            case DT_I32:
                return this->Value._i32Value == 0;
            case DT_U32:
                return this->Value._u32Value == 0;
            case DT_POINTER:
                return this->Value._pointer == nullptr;
            case DT_OBJECT:
                return this->_pstr._ptr == nullptr;
            case DT_CSTR:
                return strcasecmp(this->Value._stringPointer, "") == 0;
        }
        return false;
    }
    
    void OmniType::SetEmpty()
    {
        Value._pointer = nullptr;
        _pstr.Clear();
        dt = DT_NONE;
    }
    
    bool OmniType::operator ==(const OmniType& v) const
    {
        switch (this->dt)
        {
            case DT_R8:
                return this->Value._r8Value == v.Value._r8Value;
            case DT_R4:
                return this->Value._r4Value == v.Value._r4Value;
            case DT_I64:
                return this->Value._i64Value == v.Value._i64Value;
            case DT_U64:
                return this->Value._u64value == v.Value._u64value;
            case DT_I32:
                return this->Value._i32Value == v.Value._i32Value;
            case DT_U32:
                return this->Value._u32Value == v.Value._u32Value;
            case DT_POINTER:
                return this->Value._pointer == v.Value._pointer;
            case DT_OBJECT:
                return this->_pstr._ptr == v._pstr._ptr;
            case DT_CSTR:
                return strcasecmp(this->Value._stringPointer, v.Value._stringPointer) == 0;
        }
        return false;
    }
    
    const OmniType& OmniType::operator =(OmniType&& o)
    {
        this->Value = o.Value;
        dt = o.dt;
        _pstr.Assign(o._pstr);
        return *this;
    }
    
    const OmniType& OmniType::operator =(const OmniType& o)
    {
        this->Value = o.Value;
        dt = o.dt;
        if (o._pstr)
            _pstr.Assign(o._pstr);
        return *this;
    }
    
    const OmniType& OmniType::operator =(double v)
    {
        this->Value._r8Value = v;
        dt = DT_R8;
        return *this;
    }

    const OmniType& OmniType::operator =(float v)
    {
        this->Value._r4Value = v;
        dt = DT_R4;
        return *this;
    }

    const OmniType& OmniType::operator =(const char* v)
    {
        SP<IString> pstr;
        IString::Create(v, &pstr);
        this->Value._stringPointer = pstr->get_Buffer();
        _pstr = (IObject*)pstr;
        dt = DT_CSTR;
        return *this;
    }
    
    const OmniType& OmniType::operator =(signed long v)
    {
        this->Value._i64Value = v;
        dt = DT_I64;
        return *this;
    }
    
    const OmniType& OmniType::operator =(signed int v)
    {
        this->Value._i32Value = v;
        dt = DT_I32;
        return *this;
    }
    
    const OmniType& OmniType::operator =(unsigned long v)
    {
        this->Value._u64value = v;
        dt = DT_U64;
        return *this;
    }
    
    const OmniType& OmniType::operator =(unsigned int v)
    {
        this->Value._u32Value = v;
        dt = DT_U32;
        return *this;
    }

    const OmniType& OmniType::operator=(POINTER v) 
    {
        Value._pointer = v;
        dt = DT_POINTER;
        return *this;
    }
    
    const OmniType& OmniType::operator=(IObject* v)
    {
        _pstr.Assign(v);
        dt = DT_OBJECT;
        return *this;
    }
    
    OmniType::operator const char*() const
    {
        return Value._stringPointer;
    }
    
    OmniType::operator R8() const
    {
        return Value._r8Value;
    }
    
    OmniType::operator R4() const
    {
        return Value._r4Value;
    }
    
    OmniType::operator I32() const
    {
        return Value._i32Value;
    }
    
    OmniType::operator I64() const
    {
        return Value._i64Value;
    }
    
    OmniType::operator U32() const
    {
        return Value._u32Value;
    }

    OmniType::operator U64() const
    {
        return Value._u64value;
    }
    
    OmniType::operator POINTER() const
    {
        return Value._pointer;
    }
    
    OmniType::operator  IObject*() const
    {
        return _pstr._ptr;
    }

    SE_CODE OmniType::toString(IString** ppOut) const
    {
        if (IsEmpty() || 
                dt == DT_NONE)
        {
            return E_NoDataToReturn;
        }
        
        std::stringstream str;
        if (dt == DT_R8)
        {
            str << std::setprecision(std::numeric_limits<R8>::digits10) << Value._r8Value;
        }
        else if (dt == DT_R4)
        {
            str << std::setprecision(std::numeric_limits<R4>::digits10) << Value._r4Value;
        }
        else if (dt == DT_I32)
        {
            str << std::setprecision(std::numeric_limits<I32>::digits10) << Value._i32Value;
        }
        else if (dt == DT_U32)
        {
            str << std::setprecision(std::numeric_limits<U32>::digits10) << Value._u32Value;
        }
        else if (dt == DT_I64)
        {
            str << std::setprecision(std::numeric_limits<I64>::digits10) << Value._i64Value;
        }
        else if (dt == DT_U64)
        {
            str << std::setprecision(std::numeric_limits<U64>::digits10) << Value._u64value;
        }
        else if (dt == DT_POINTER)
        {
            str << Value._pointer;
        }
        else if (dt == DT_OBJECT)
        {
            str << _pstr._ptr;
        }
        else
        {
            str << Value._stringPointer;
        }
        
        return IString::Create(str.str().c_str(), ppOut);
        
    }

    void OmniType::Serialize(ISerializedData<IString>* pIn) 
    {
        std::stringstream str;
        str << this;
        pIn->Add("Value", str.str().c_str());
    }


    std::ostream & operator << (std::ostream& os, const OmniType& ot)
    {
        if (ot.IsEmpty() || ot.dt == DT_NONE)
            return os << "-";
        
        if (ot.dt == DT_R8)
        {
            return os << std::setprecision(std::numeric_limits<R8>::digits10) << ot.Value._r8Value;
        }
        else if (ot.dt == DT_R4)
        {
            return os << std::setprecision(std::numeric_limits<R4>::digits10) << ot.Value._r4Value;
        }
        else if (ot.dt == DT_I32)
        {
            return os << std::setprecision(std::numeric_limits<I32>::digits10) << ot.Value._i32Value;
        }
        else if (ot.dt == DT_U32)
        {
            return os << std::setprecision(std::numeric_limits<U32>::digits10) << ot.Value._u32Value;
        }
        else if (ot.dt == DT_I64)
        {
            return os << std::setprecision(std::numeric_limits<I64>::digits10) << ot.Value._i64Value;
        }
        else if (ot.dt == DT_U64)
        {
            return os << std::setprecision(std::numeric_limits<U64>::digits10) << ot.Value._u64value;
        }
        else if (ot.dt == DT_POINTER)
        {
            return os << ot.Value._pointer;
        }
        else if (ot.dt == DT_OBJECT)
        {
            return os << ot._pstr._ptr;
        }
        else
        {
            return os << ot.Value._stringPointer;
        }
    }
    
    OmniType operator *(const OmniType& l, const OmniType& r) 
    {
        if (l.dt != r.dt || l.dt == DT_CSTR || l.dt == DT_POINTER)
            return OmniType();
        
        switch (l.dt)
        {
            case DT_R8:
                return l.Value._r8Value * r.Value._r8Value;
            case DT_R4:
                return l.Value._r4Value * r.Value._r4Value;
            case DT_I64:
                return l.Value._i64Value * r.Value._i64Value;
            case DT_U64:
                return l.Value._u64value * r.Value._u64value;
            case DT_I32:
                return l.Value._i32Value * r.Value._i32Value;
            case DT_U32:
                return l.Value._u32Value * r.Value._u32Value;
        }
        return OmniType();
    }

    OmniType operator +(const OmniType& l, const OmniType& r) 
    {
        if (l.dt != r.dt || l.dt == DT_CSTR || l.dt == DT_POINTER)
            return OmniType();
        
        switch (l.dt)
        {
            case DT_R8:
                return l.Value._r8Value + r.Value._r8Value;
            case DT_R4:
                return l.Value._r4Value + r.Value._r4Value;
            case DT_I64:
                return l.Value._i64Value + r.Value._i64Value;
            case DT_U64:
                return l.Value._u64value + r.Value._u64value;
            case DT_I32:
                return l.Value._i32Value + r.Value._i32Value;
            case DT_U32:
                return l.Value._u32Value + r.Value._u32Value;
        }
        return OmniType();
    }

    OmniType operator -(const OmniType& l, const OmniType& r) 
    {
        if (l.dt != r.dt || l.dt == DT_CSTR || l.dt == DT_POINTER)
            return OmniType();
        
        switch (l.dt)
        {
            case DT_R8:
                return l.Value._r8Value - r.Value._r8Value;
            case DT_R4:
                return l.Value._r4Value - r.Value._r4Value;
            case DT_I64:
                return l.Value._i64Value - r.Value._i64Value;
            case DT_U64:
                return l.Value._u64value - r.Value._u64value;
            case DT_I32:
                return l.Value._i32Value - r.Value._i32Value;
            case DT_U32:
                return l.Value._u32Value - r.Value._u32Value;
        }
        return OmniType();

    }

    OmniType operator /(const OmniType& l, const OmniType& r) 
    {
        if (l.dt != r.dt || l.dt == DT_CSTR || l.dt == DT_POINTER)
            return OmniType();
        
        switch (l.dt)
        {
            case DT_R8:
                return l.Value._r8Value / r.Value._r8Value;
            case DT_R4:
                return l.Value._r4Value / r.Value._r4Value;
            case DT_I64:
                return l.Value._i64Value / r.Value._i64Value;
            case DT_U64:
                return l.Value._u64value / r.Value._u64value;
            case DT_I32:
                return l.Value._i32Value / r.Value._i32Value;
            case DT_U32:
                return l.Value._u32Value / r.Value._u32Value;
        }
        return OmniType();

    }

    OmniType& OmniType::operator *=(const OmniType& r) 
    {
        if (dt != r.dt || r.dt == DT_CSTR || r.dt == DT_POINTER || dt == DT_CSTR || dt == DT_POINTER)
            throw seexception("Invalid type");
        *this = *this * r;
        return *this;
    }
    
    OmniType& OmniType::operator /=(const OmniType& r) 
    {
        if (dt != r.dt || r.dt == DT_CSTR || r.dt == DT_POINTER || dt == DT_CSTR || dt == DT_POINTER)
            throw seexception("Invalid type");
        switch (r.dt)
        {
            case DT_R8:
                if (!r.Value._r8Value) throw seexception("divide by zero");
                break;
            case DT_R4:
                if (!r.Value._r4Value) throw seexception("divide by zero");
                break;
            case DT_I64:
                if (!r.Value._i64Value) throw seexception("divide by zero");
                break;
            case DT_U64:
                if (!r.Value._u64value) throw seexception("divide by zero");
                break;
            case DT_I32:
                if (!r.Value._i32Value) throw seexception("divide by zero");
                break;
            case DT_U32:
                if (!r.Value._u32Value) throw seexception("divide by zero");
                break;
        }
        *this = *this / r;
        return *this;
    }
    
    OmniType& OmniType::operator +=(const OmniType& r) 
    {
        if (dt != r.dt || r.dt == DT_CSTR || r.dt == DT_POINTER || dt == DT_CSTR || dt == DT_POINTER)
            throw seexception("Invalid type");
        *this = *this + r;
        return *this;
    }

    OmniType& OmniType::operator -=(const OmniType& r) 
    {
        if (dt != r.dt || r.dt == DT_CSTR || r.dt == DT_POINTER || dt == DT_CSTR || dt == DT_POINTER)
            throw seexception("Invalid type");
        *this = *this - r;
        return *this;
    }

    OmniType OmniType::Empty() 
    {
        return OmniType();
    }

}
