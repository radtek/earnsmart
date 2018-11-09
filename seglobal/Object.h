/* 
 * File:   Object.h
 * Author: santony
 *
 * Created on July 2, 2012, 7:33 AM
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include "cpplinq.hpp"
#include "CObjectRoot.h"

using namespace se;

template<typename T>
class CObject : public T
{
public:
    
    CObject(se::ISession* ps): T(ps)
    {
        
    }
    
    virtual ~CObject()
    {
        
    }
    
    virtual void AddRef() 
    {
        T::_AddRef();
    }
    
    virtual void DecRef()
    {
        T::_DecRef();
    }
    
    virtual bool Equals(IObject* other)
    {
        return T::_Equals(other);
    }
    
    virtual UID get_Id() 
    {
        return T::_get_Id();
    }
    
    virtual void set_Id(UID id) 
    {
        T::_set_Id(id);
    }
    
    virtual bool set_Property(CSTR key, const OmniType& value) 
    {
        return T::_set_Property(key, value);
    }
    
    virtual OmniType get_Property(CSTR key) const
    {
        return T::_get_Property(key);
    }
    
    template<typename I>
    static ErrorId Create(se::ISession* ps, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init();
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }
    
    template<typename P0, typename P1, typename P2, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2);
        t->AddRef();
        if (FAILED(e))
            return e;
        *outVal = t;
        return e;
    }
    
    template<typename P0, typename P1, typename P2, typename P3, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6, p7);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6, p7, p8);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename I>
    static ErrorId Create(se::ISession* ps, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, I** outVal)
    {
        CObject<T>* t = new CObject<T>(ps);
        ErrorId e = t->Init(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
        if (FAILED(e))
            return e;
        t->AddRef();
        *outVal = t;
        return e;
    }
    
    
};


#endif	/* OBJECT_H */

