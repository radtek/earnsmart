/* 
 * File:   CCustomer.h
 * Author: aashok
 *
 * Created on August 31, 2013, 5:54 PM
 */

#ifndef CCUSTOMER_H
#define	CCUSTOMER_H

#include "seadminapi.h"
#include <string>
////#include <log4cxx/logger.h>

using namespace std;

namespace se
{
    namespace admin
    {
        class CCustomer : public CObjectRoot, public ICustomer
        {
        public:
            CCustomer(ISession*);
            CCustomer(const CCustomer& orig)=delete;
            virtual ~CCustomer();

            virtual CSTR get_Code() ;
            virtual ErrorId get_Details(IUser** ppOut) ;
            virtual ErrorId get_Address(IAddress** ppOut) ;
            virtual void set_Code(CSTR value) ;
            virtual ErrorId set_Details(IUser *pIn) ;
            virtual ErrorId set_Address(IAddress *pIn) ;
            virtual ErrorId get_CurrentSubscriptionInfo(ICustomerSubscriptionInfo** ppOut) ;
            virtual ErrorId set_CurrentSubscriptionInfo(ICustomerSubscriptionInfo* pIn) ;
            virtual void Serialize(ISerializedData<IString>* pIn) ;
                
        public:
            UID userId;
            UID addrId;
            UID _subscrInfoId;
                
        private:
            string _code;
            SP<IUser> _pUserDetails;
            SP<IAddress> _inputAddress;
            SP<ICustomerSubscriptionInfo> _pCustomerSubscriptionInfo;
            //log4cxx::LoggerPtr _logger;
        };
    }
}



#endif	/* CCUSTOMER_H */

