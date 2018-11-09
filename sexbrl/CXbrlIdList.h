/* 
 * File:   CXbrlIdList.h
 * Author: santony
 *
 * Created on May 1, 2014, 4:29 PM
 */

#ifndef CXBRLIDLIST_H
#define	CXBRLIDLIST_H

#include "../seglobal/CList.h"
#include "sexbrlapi.h"

namespace se
{
    namespace xbrl
    {
        class CXbrlIdList : public CList< UID, IXbrlIdList >
        {
        public:
            CXbrlIdList(ISession*);
            virtual ~CXbrlIdList();
            

            virtual unsigned long GetSerializedvalue(ISerializedData<IString>* pIn, const char* propName);

            virtual void SetSerializedValue(ISerializedData<IString>* pIn, const char* PropName, const unsigned long& value);
            
#ifdef __DEBUG__

            virtual void _AddRef();

            virtual void _DecRef();
#endif

        private:

        };
    }
}


#endif	/* CXBRLIDLIST_H */

