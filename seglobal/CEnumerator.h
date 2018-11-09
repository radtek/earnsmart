/* 
 * File:   CEnumerator.h
 * Author: santony
 *
 * Created on March 18, 2013, 9:08 PM
 */

#ifndef CENUMERATOR_H
#define	CENUMERATOR_H

#include "IObject.h"
#include "CObjectRoot.h"


namespace se
{

    template <typename TList, typename T, typename TIter>
    class CEnumerator : public CObjectRoot, public IEnumerator<T>
    {
    public:
        CEnumerator(ISession* ps) : CObjectRoot(ps)
        {
        }

        CEnumerator(const CEnumerator& orig)
        {

        }
        
        virtual ErrorId Init(TList* list)
        {
            _list = list;
            _list->AddRef();
            Reset();
            
            return S_OK;
        }

        virtual ~CEnumerator()
        {
            _list->DecRef();
        }
        
        virtual void Reset()
        {
            _startiter = _list->get_Begin();
        }
        
        virtual bool MoveNext() 
        {
            if (_startiter == _list->get_End())
                return false;
            _currentValue = *_startiter;
            _startiter++;
            return true;
        }
        
        virtual T get_Current() 
        {
            return _currentValue;
        }

    protected:
        TList *_list;
        TIter _startiter;
        bool _bReset;
        T _currentValue;

    };
    
    template <typename TList, typename TKey, typename TValue, typename TIter>
    class CDictionaryEnumerator : public CEnumerator<TList, KeyValuePair<TKey, TValue>, TIter >
    {
        typedef CEnumerator<TList, KeyValuePair<TKey, TValue>, TIter > BASE;
        
    public:
        virtual KeyValuePair<TKey, TValue> get_Current() 
        {
            return KeyValuePair<TKey, TValue>(BASE::_startiter->first, BASE::_startiter->second);
        }
        
    };
   

}

#endif	/* CENUMERATOR_H */

