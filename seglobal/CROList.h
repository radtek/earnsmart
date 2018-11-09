/* 
 * File:   CROList.h
 * Author: aashok
 *
 * Created on July 9, 2013, 8:54 PM
 */


#ifndef CROLIST_H
#define	CROLIST_H

#include "IObject.h"
#include "Object.h"
#include <vector>
#include "CEnumerator.h"
#include <algorithm>
#include <boost/type_traits.hpp>
#include <algorithm>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>



using namespace std;

namespace se
{
    
    template<typename T, typename TP = IROList<T> >
    class CROList : public CObjectRoot, public TP
    {
        
        template <typename TList, typename TItem, typename TIter> friend class CEnumerator;
        
    private:
        typedef IROList<T>  IF;
        typename IF::VECTOR _vector;

    protected:
        virtual T GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName) = 0;
        virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const T& value) = 0;
        
    public:
        CROList(ISession* ps):CObjectRoot(ps)
        {
            
        }
        
        CROList(const CROList& orig):CObjectRoot(orig)
        {
            
        }
        
        virtual ~CROList()
        {
            
        }
        
        virtual void Serialize(ISerializedData<IString>* pIn)
        {
            // loop the vector.
            pIn->Add("Count", (U64)_vector.size());
            for (auto i = 0; i != _vector.size(); ++i)
            {
                stringstream str;
                str << i;
                SetSerializedValue(pIn, str.str().c_str(), _vector[i]);
            }
        }
        
        virtual ErrorId Deserialize(ISerializedData<IString>* pIn)
        {
            // get count
            SIZE size = pIn->getUInt64("Count");
            for (int i=0; i!=size; ++i)
            {
                stringstream str;
                str << i;
                _vector.push_back(GetSerializedvalue(pIn, str.str().c_str()));
            }
            return S_Success;
        }
        
        virtual void Add(T item) 
        {
            _vector.push_back(item);
        }
        
        virtual bool Remove(T item) 
        {
            auto iter = find(_vector.begin(), _vector.end(), item);
            if (iter != _vector.end())
            {
                _vector.erase(iter);
                return true;
            }
            return false;
        }        
        
        virtual void Sort(std::function<bool(T, T)> sortfunc)
        {
            std::sort(_vector.begin(), _vector.end(), sortfunc);
        }
        
        virtual ErrorId get_Enumerator(IEnumerator<T>** outVal) 
        {
            return CObject< CEnumerator<CROList, T, typename IF::VECTOR::const_iterator> >::Create(get_Session(), this, outVal);
        }
        
        
        virtual SIZE get_Count() 
        {
            return _vector.size();
        }
        
        virtual ErrorId get_Item(SIZE index, T* pOut) 
        {
            if (_vector.size() > index && index >=0 )
            {
                *pOut = _vector.at(index);
                return S_Success;
            }
            return E_NotFound;
        }
        

    /* Static create facility */        
        
        virtual typename IF::VECTOR::iterator begin() 
        {
            return _vector.begin();
        }

        virtual typename IF::VECTOR::iterator end() 
        {
            return _vector.end();
        }
        

        /* internal only */
        typename IF::VECTOR::const_iterator get_Begin()
        {
            return _vector.begin();
        }
        
        typename IF::VECTOR::const_iterator get_End()
        {
            return _vector.end();
        }
        

    };
    
    // custom i16 list
    class CROI16List : public CROList<I16>
    {
    public:
        CROI16List(ISession* ps):CROList<I16>(ps)
        {
            
        }
        
    protected:
        virtual I16 GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName)
        {
            return pIn->getShort(propName);
        };
        
        virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const I16& value)
        {
            pIn->Add(PropName, value);
        }
        
    public:
        static ErrorId Create(se::ISession* ps, IROList<I16>** ppOut)
        {
            return CObject<CROI16List>::Create(ps, ppOut);
        }
    };

    // custom R8 list
    class CROR8List : public CROList<R8>
    {
    protected:
        virtual R8 GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName)
        {
            return pIn->getDoubleReal(propName);
        };
        
        virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const R8& value)
        {
            pIn->Add(PropName, value);
        }
        
    public:
        CROR8List(ISession* ps):CROList<R8>(ps)
        {
            
        }
        static ErrorId Create(se::ISession* ps, IROList<R8>** ppOut)
        {
            return CObject<CROR8List>::Create(ps, ppOut);
        }
    };
    
    template<typename T>
    class CROListNonSerialized : public CROList<T>
    {
    protected:
        virtual T GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName)
        {
            return T();
        };
        
        virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const T& value)
        {
        }
        
    public:
        CROListNonSerialized(ISession* ps):CROList<T>(ps)
        {
            
        }
        
        static ErrorId Create(se::ISession* ps, IROList<T>** ppOut)
        {
            return CObject<CROListNonSerialized>::Create(ps, ppOut);
        }
    };
    
    
    // customized list for IObject types
    
    template<typename T = IObject, typename TP = IROSPList<T> >
    class CROSPList : public CObjectRoot, public TP
    {
        template <typename TList, typename TItem, typename TIter> friend class CEnumerator;
        
    protected:
        typedef IROSPList<T>  IF;
        typename IF::VECTOR _vector;
        
    public:
        CROSPList(ISession* ps):CObjectRoot(ps)
        {
            static_assert(boost::is_base_and_derived<IObject, T>::value, "Only type that implements IObject can be contained by this list.");
            static_assert(boost::is_base_and_derived<ISerialization, T>::value, "Only type that implements IObject and ISerialization can be contained by this list.");
            //static_assert(boost::is_base_and_derived<IDeserialization, T>::value, "Only type that implements IObject and IDeserialization can be contained by this list.");
        }
        
        CROSPList(const CROSPList& orig):CObjectRoot(orig)
        {
            
        }
        
        virtual ~CROSPList()
        {
            _vector.clear();
        }
        
        virtual void Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Count", (U64)_vector.size());
            for (auto i = 0; i != _vector.size(); ++i)
            {
                stringstream str;
                str << i;
                pIn->Add(str.str().c_str(), _vector[i]);
            }
        }
        
        virtual ErrorId Deserialize(ISerializedData<IString>* pIn)
        {
            return E_NotImpl;
        }
        
        virtual ErrorId get_Enumerator(IEnumerator<T*>** outVal) 
        {
            return CObject< CEnumerator<CROSPList, T*, typename IF::VECTOR::const_iterator> >::Create(get_Session(), this, outVal);
        }
        
        virtual bool HasItem(UID uid) 
        {
            auto it = find_if(_vector.begin(), _vector.end(), [&uid](T* item){ return item->get_Id()==uid;});
            return it != _vector.end();
        }

        virtual SE_CODE get_ItemById(UID uid, T** ppOut) 
        {
            auto it = find_if(_vector.begin(), _vector.end(), [&uid](T* item){ return item->get_Id()==uid;});
            if (it == _vector.end())
                return E_NotFound;
            (*it).CopyTo(ppOut);
            return S_Success;
        }
        
        virtual void Add(T* item) 
        {
            _vector.push_back(item);
        }
        
        virtual bool Remove(T* item) 
        {
            auto iter = find(_vector.begin(), _vector.end(), item);
            if (iter != _vector.end())
            {
                _vector.erase(iter);
                return true;
            }
            return false;
        }
        
        virtual bool RemoveById(UID id) 
        {
            // search by id
            auto it = find_if(_vector.begin(), _vector.end(), [&id](T* item)
            { 
               return (item->get_Id() == id) ;
            });
            if (it != _vector.end())
            {
                _vector.erase(it);
                return true;
            }
            return false;
        }
        
        virtual void Sort(std::function<bool(T*, T*)> sortfunc)
        {
            std::sort(_vector.begin(), _vector.end(), sortfunc);
        }
        
        virtual SIZE get_Count() 
        {
            return _vector.size();
        }
        
        virtual ErrorId get_Item(SIZE index, T** pOut) 
        {
            if (_vector.size() > index && index >=0 )
            {
                _vector.at(index).CopyTo(pOut);
                return S_Success;
            }
            return E_NotFound;
        }
        

      
        
    /* Static create facility */        
        static ErrorId CreateROSPList(ISession* ps, CROSPList<T>** ppOut)
        {
            return CObject<CROSPList<T>>::Create(ps, ppOut);
        }
        
        virtual typename IF::VECTOR::iterator begin() 
        {
            return _vector.begin();
        }

        virtual typename IF::VECTOR::iterator end() 
        {
            return _vector.end();
        }
    

        /* internal only */
        typename IF::VECTOR::const_iterator get_Begin()
        {
            return _vector.begin();
        }
        
        typename IF::VECTOR::const_iterator get_End()
        {
            return _vector.end();
        }
    };    
    
    class CIdNameList : public CROSPList<IIdName, IIdNameList> 
    {
    public:
        CIdNameList(ISession*);
        virtual ~CIdNameList();
    };

}
#endif	

