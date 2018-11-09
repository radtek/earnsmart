/* 
 * File:   CCustomerSubscriptionInfo.h
 * Author: aashok
 *
 * Created on August 31, 2013, 5:24 PM
 */

#ifndef CCUSTOMERSUBSCRIPTIONINFO_H
#define	CCUSTOMERSUBSCRIPTIONINFO_H

#include "seadminapi.h"

namespace se
{
    namespace admin
    {
        class CCustomerSubscriptionInfo : public CObjectRoot,public ICustomerSubscriptionInfo
        {
            public:
                CCustomerSubscriptionInfo(ISession*);
                CCustomerSubscriptionInfo(const CCustomerSubscriptionInfo& orig)=delete;
                virtual ~CCustomerSubscriptionInfo();
                
                virtual ErrorId get_Customer(ICustomer** ppOut) ;
                virtual void set_CustomerId(UID customerId) ;
                virtual DATETIME get_StartedOn() ;
                virtual void set_Started(DATETIME const& value) ;
                virtual DATETIME get_EndedOn() ;
                virtual void set_EndedOn(DATETIME const& value) ;
                virtual RP<ICustomerStatus> get_Status() ;
                virtual void set_Status(ICustomerStatus* pIn) ;
                virtual RP<ICustomerSubscriptionType> get_Type() ;
                virtual void set_SubscriptionType(ICustomerSubscriptionType* pIn) ;
                virtual PERIOD get_PayFrequency() ;
                virtual void set_PayFrequency(PERIOD const& value) ;
                virtual void Serialize(ISerializedData<IString>* pIn) ;
            
            
            public:
                UID _subscriptionId;
                UID _customerId;
                
            private:
                
                SP<ICustomerStatus> _pCustomerStatus;
                SP<ICustomerSubscriptionType> _pCustomerSubscriptionType;
                DATETIME _startedOn,_endedOn;
                PERIOD _payFrequency;
                

        };
    }
    
}




#endif	/* CCUSTOMERSUBSCRIPTIONINFO_H */

