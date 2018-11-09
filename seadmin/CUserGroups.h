/* 
 * File:   CUserGroups.h
 * Author: santony
 *
 * Created on April 8, 2013, 8:45 PM
 */

#ifndef CUSERGROUPS_H
#define	CUSERGROUPS_H

#include "../seglobal/CList.h"
#include "seadminapi.h"

namespace se
{
    namespace admin
    {
        class CUserGroups : public CList<USERGROUPS>
        {
        public:
            CUserGroups();
            CUserGroups(const CUserGroups& orig);
            virtual ~CUserGroups();
        private:
            
        protected:
            virtual USERGROUPS GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName);
            virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const USERGROUPS& value);
        };
        
    }
}


#endif	/* CUSERGROUPS_H */

