/* 
 * File:   CCustomerStatus.h
 * Author: aashok
 *
 * Created on August 31, 2013, 4:59 PM
 */

#ifndef CCUSTOMERSTATUS_H
#define	CCUSTOMERSTATUS_H

#include "seadminapi.h"
#include <string>

using namespace std;

namespace se
{
    namespace admin
    {
        class CCustomerStatus : public CObjectRoot, public ICustomerStatus
        {
            public:
                CCustomerStatus(ISession*);
                CCustomerStatus(const CCustomerStatus& orig)=delete;
                virtual ~CCustomerStatus();
                
                virtual CSTR get_Name();
                virtual void set_Name(CSTR value);
                virtual void Serialize(ISerializedData<IString>* pIn) ;
                
            private:
                string _name;
        };
    }

}


#endif	/* CCUSTOMERSTATUS_H */

