/* 
 * File:   CDictionary.h
 * Author: santony
 *
 * Created on March 18, 2013, 9:00 PM
 */

#ifndef CDICTIONARY_H
#define	CDICTIONARY_H

#include "IObject.h"
#include "Object.h"
#include <map>
#include "CEnumerator.h"

using namespace std;

namespace se
{

    template<typename TKey, typename TValue>
    class CDictionary : public CObjectRoot, public IDictionary<TKey, TValue>
    {
        CDictionary(ISession* ps):CObjectRoot(ps)
        {
            
        }
        
    public:

        CDictionary(const CDictionary& orig)
        {
            
        }
        
        virtual ~CDictionary()
        {
            
        }
        
        virtual void Add(TKey key, TValue item) 
        {
            _map.insert(make_pair(key, item));
        }
        
        virtual bool Remove(TKey item) 
        {
            return _map.erase(item) > 0;
        }
        
        virtual SIZE get_Count() 
        {
            return _map.size();
        }
        
        virtual ErrorId get_Item(TKey key, TValue* pOut) 
        {
            auto iter = _map.find(key);
            if (iter != _map.end())
            {
                *pOut = *iter->second;
                return S_Success;
            }
            return E_NotFound;
        }
        
        virtual ErrorId get_Enumerator(IEnumerator< KeyValuePair<TKey, TValue> >** outVal) 
        {
            return CObject< CEnumerator<CDictionary<TKey, TValue>, KeyValuePair<TKey, TValue>, typename MAP::const_iterator> >::Create(this, outVal);
        }        
        
        
    /* Static create facility */        
        static ErrorId CreateDictionary(IDictionary<TKey, TValue>** ppOut)
        {
            return CObject<CDictionary<TKey, TValue>>::Create(&ppOut);
        }
        
    public :
        typedef map<TKey, TValue> MAP;
        
        typename MAP::const_iterator get_Begin()
        {
            return _map.begin();
        }
        
        typename MAP::const_iterator get_End()
        {
            return _map.end();
        }
        
    /* Static create facility */        
        static ErrorId CreateList(IDictionary<TKey, TValue>** ppOut)
        {
            return CObject<CDictionary<TKey, TValue>>::Create(ppOut);
        }        
        
    private:
        MAP _map;

    };

}
#endif	/* CDICTIONARY_H */

