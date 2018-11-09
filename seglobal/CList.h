/* 
 * File:   CList.h
 * Author: santony
 *
 * Created on March 18, 2013, 8:57 PM
 */

#ifndef CLIST_H
#define	CLIST_H

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
    
    template<typename T, typename TP = IList<T> >
    class CList : public CObjectRoot, public TP
    {
        template <typename TList, typename TItem, typename TIter> friend class CEnumerator;
        
    public:
        typedef std::vector<T> VECTOR;
    
    protected:
        typedef IList<T>  IF;
        VECTOR _vector;

    protected:
        virtual T GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName) {return T();};
        virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const T& value) {};
        
    public:
        CList(ISession* ps):CObjectRoot(ps)
        {
            
        }
        
        CList(const CList& orig)
        {
            
        }
        
        virtual ~CList()
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
        
        
        
        virtual ErrorId get_Enumerator(IEnumerator<T>** outVal) 
        {
            return CObject< CEnumerator<CList, T, typename VECTOR::const_iterator> >::Create(get_Session(), this, outVal);
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
        
        virtual SIZE get_Count() 
        {
            return _vector.size();
        }
        
        virtual bool RemoveItem(SIZE position)
        {
            if (position <0 or _vector.size()>=position)
                return false;
            _vector.erase(_vector.begin() + position);
            return true;
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
        
        virtual bool HasItem(T item) 
        {
            return std::find(_vector.begin(), _vector.end(), item) != _vector.end();
        }
        
        
        virtual typename IF::VECTOR::iterator begin() 
        {
            return _vector.begin();
        }

        virtual typename IF::VECTOR::iterator end() 
        {
            return _vector.end();
        }
        

    /* Static create facility */        
        static ErrorId CreateList(IList<T>** ppOut)
        {
            return CObject<CList<T>>::Create(ppOut);
        }
    

        /* internal only */
        typename VECTOR::const_iterator get_Begin()
        {
            return _vector.begin();
        }
        
        typename VECTOR::const_iterator get_End()
        {
            return _vector.end();
        }
        

    };
    
    // customized list for IObject types
    
    template<typename T, typename TP = ISPList<T> >
    class CSPList : public CObjectRoot, public TP
    {
        template <typename TList, typename TItem, typename TIter> friend class CEnumerator;
       
    protected:
        typedef ISPList<T>  IF;
        typename IF::VECTOR _vector;
        
    public:
        CSPList(ISession* ps):CObjectRoot(ps)
        {
            static_assert(boost::is_base_and_derived<IObject, T>::value, "Only type that implements IObject can be contained by this list.");
            static_assert(boost::is_base_and_derived<ISerialization, T>::value, "Only type that implements IObject and ISerialization can be contained by this list.");
            //static_assert(boost::is_base_and_derived<IDeserialization, T>::value, "Only type that implements IObject and IDeserialization can be contained by this list.");
        }
        
        CSPList(const CSPList& orig):CObjectRoot(orig)
        {
            
        }
        
        virtual ~CSPList()
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
            return E_NotImpl;   // we have a problem here since we don't know how each IObject is created using what parameters.
        }
        
        virtual ErrorId get_Enumerator(IEnumerator<T*>** outVal) 
        {
            return CObject< CEnumerator<CSPList, T*, typename IF::VECTOR::const_iterator> >::Create(get_Session(), this, outVal);
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
        
        virtual typename IF::VECTOR::iterator begin() 
        {
            return _vector.begin();
        }

        virtual typename IF::VECTOR::iterator end() 
        {
            return _vector.end();
        }
    
        

    /* Static create facility */        
        static ErrorId CreateSPList(ISession* ps, ISPList<T>** ppOut)
        {
            return CObject<CSPList<T>>::Create(ps, ppOut);
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

}
#endif	/* CLIST_H */

