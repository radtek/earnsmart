/* 
 * File:   CSPSerializable.h
 * Author: santony
 *
 * Created on April 4, 2014, 1:43 PM
 */

#ifndef CSPSERIALIZABLE_H
#define	CSPSERIALIZABLE_H

#include "../seglobal/IObject.h"
#include <cppcms/serialization.h>

using namespace se;
using namespace cppcms;

namespace sg
{
    namespace web
    {
        
        template<typename T = IObject>
        struct __SGSmartPointer : public serializable
        {
        public:
            SIZE _ptr ;

            __SGSmartPointer() : _ptr(0)
            {

            }

            __SGSmartPointer(T* ptr) : _ptr(ptr)
            {
                if (!ptr)
                    throw E_InvalidPointer;
                ptr->AddRef();
            }

            __SGSmartPointer(const __SGSmartPointer<T>& p) : _ptr(p._ptr)
            {
                if (!_ptr)
                    throw E_InvalidPointer;
                ((T*)_ptr)->AddRef();
            }

            ~__SGSmartPointer()
            {
                Clear();
            }
            

            virtual void serialize(archive& a)
            {
                a & _ptr;
            }


            void Clear()
            {
                T* pTmp = (T*)_ptr; 
                _ptr = 0;
                if (pTmp)
                    pTmp->DecRef();
            }

            // cast operator
            operator T*() const
            {
                return (T*)_ptr;
            }
    
            // dereference operator
            T& operator*() const
            {
                return *((T*)_ptr);
            }

            // indirect access to internal pointer
            T* operator->() const
            {
                return ((T*)_ptr);
            }

            // Address of operator.
            T** operator &()
            {
                if (_ptr)
                    Clear();
                return (T**)&_ptr;
            }

            // assign by adding reference
            void Assign(T* p)
            {
                if (((T*)_ptr)) Clear();
                _ptr = (SIZE)p;
                if (_ptr)
                    ((T*)_ptr)->AddRef();
            }

            // attach, but don't addref
            void Attach(T* p)
            {
                if (_ptr) Clear();
                _ptr = (SIZE)p;
            }

            // detach, by not dec-refing, and return pointer.
            T* Detach()
            {
                T* p = (T*)_ptr; 
                _ptr = 0;
                return p;
            }

            // assignment operator.
            T* operator=(T* p)
            {
                Assign(p);
                return (T*)_ptr;
            }

            // copy by addref-ing.
            template<class I>
            bool CopyTo(I** pp)
            {
                *pp = (T*)_ptr;
                if (_ptr) ((T*)_ptr)->AddRef();
                return _ptr != 0;
            }

        };

        #define SGSP __SGSmartPointer

        
        
        template<typename T>
        class MakeSerializable : public serializable
        {
            T _t;
        public:
            MakeSerializable()
            {
                
            }

            MakeSerializable(const T& t)
            {
               _t = t; 
            }
            
            MakeSerializable(MakeSerializable&& m)
            {
                _t = std::move(m._t);
            }
            
            MakeSerializable(const MakeSerializable& c)
            {
                _t = c._t;
            }
            
            MakeSerializable& operator=(const MakeSerializable& o)
            {
                _t = o._t;
                return *this;
            }

            MakeSerializable& operator=(MakeSerializable&& m)
            {
                _t = std::move(m._t);
                return *this;
            }
            
            virtual void serialize(archive& a)
            {
                a & _t;
            }
            
            const T& get() const
            {
                return _t;
            }

            static MakeSerializable<T> Make(T value)
            {
                return std::move(MakeSerializable<T>(value));
            }
        };
    }    
}

#endif	/* CSPSERIALIZABLE_H */

