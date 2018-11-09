/* 
 * File:   CCustomerSubscriptionType.h
 * Author: aashok
 *
 * Created on August 31, 2013, 4:53 PM
 */

#ifndef CCUSTOMERSUBSCRIPTIONTYPE_H
#define	CCUSTOMERSUBSCRIPTIONTYPE_H

#include "seadminapi.h"
#include <string>

using namespace std;

namespace se
{
    namespace admin
    {
        class CCustomerSubscriptionType : public CObjectRoot, public ICustomerSubscriptionType
        {
            public:
                CCustomerSubscriptionType(ISession*);
                CCustomerSubscriptionType(const CCustomerSubscriptionType& orig)=delete;
                virtual ~CCustomerSubscriptionType();

                virtual CSTR get_Name();
                virtual void set_Name(CSTR value);
                virtual void Serialize(ISerializedData<IString>* pIn) ;

            private:
                string _name;

        };
    }
    
}

        

#endif	/* CCUSTOMERSUBSCRIPTIONTYPE_H */

