/* 
 * File:   CPurchase.h
 * Author: aashok
 *
 * Created on August 31, 2013, 3:04 PM
 */

#ifndef CPURCHASE_H
#define	CPURCHASE_H

#include "seadminapi.h"
////#include <log4cxx/logger.h>

namespace se
{
    namespace admin
    {
        class CPurchase : public CObjectRoot, public IPurchase
        {

            public:
                 CPurchase(ISession*);
                 CPurchase(const CPurchase& orig)=delete;
                 virtual ~CPurchase();

//                 virtual ICustomer* get_Customer();
                 virtual ErrorId get_Customer(ICustomer** ppOut);
                 
//                 virtual IProduct* get_Product();
                 virtual ErrorId get_Product(IProduct** ppOut);
                 
//                 virtual ICustomerSubscriptionInfo* get_SubscriptionInfo();
                 virtual ErrorId get_SubscriptionInfo(ICustomerSubscriptionInfo** ppOut);
                 
                 
                 virtual CDate get_PurchaseDate();
                 virtual R4 get_Amount();
                 virtual ErrorId get_PurchaseType(ICustomerSubscriptionType** ppOut);
//                 virtual ICustomerSubscriptionType* get_PurchaseType();

                 virtual void set_CustomerId(UID id);
                 virtual void set_ProductId(UID id);
                 virtual void set_PurchaseTypeId(UID id);
                 virtual void set_PurchaseDate(CDate const& value);
                 virtual void set_Amount(R4 value);
                 virtual void set_CustomerSubscriptionInfoId(UID value);
                 virtual void Serialize(ISerializedData<IString>* pIn) ;

            public:
                UID _customerId;
                UID _productId;
                UID _subscriptionInfoId ;
                UID _subscriptionType;
                
            private:
                
                R4 _amount;
                CDate _purchaseDate;                                                
                //log4cxx::LoggerPtr _logger;
            
        };
    }
}


#endif	/* CPURCHASE_H */

