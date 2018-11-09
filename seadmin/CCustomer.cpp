/* 
 * File:   CCustomer.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 5:54 PM
 */

#include "internal.h"
#include "CCustomer.h"

namespace se
{
    namespace admin
    {
        CCustomer::CCustomer(ISession* ps) : CObjectRoot(ps)
        {
            
        }

        CCustomer::~CCustomer() 
        {
            
        }
        
        ErrorId ICustomer::Create(ISession* ps, ICustomer** ppOut)
        {
            return CObject<CCustomer>::Create(ps, ppOut);
        }
        
        CSTR CCustomer::get_Code() 
        {
            return _code.c_str();
        }

        ErrorId CCustomer::get_Details(IUser** ppOut) 
        {
            SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(),&api);
            if (FAILED(ret))
            {
               // LOGEP("Failed to open admin api.");
                return ret;
            }
            ret = api->get_User(userId,ppOut);
            return ret;
        }

        ErrorId CCustomer::get_Address(IAddress** ppOut) 
        {
            if (!addrId) return S_Optional;
            SP<IAdminAPI> api;
            ErrorId retCode = AdminOpen(get_Session(),&api);
            if (FAILED(retCode)) return retCode;
            return api->get_Address(addrId, ppOut);
        }

        void CCustomer::set_Code(CSTR value) 
        {
            _code = value;
        }

        ErrorId CCustomer::set_Details(IUser *pIn) 
        {
            _pUserDetails = pIn;
        }

        ErrorId CCustomer::set_Address(IAddress *pIn) 
        {
            _inputAddress = pIn;
        }

        ErrorId CCustomer::get_CurrentSubscriptionInfo(ICustomerSubscriptionInfo** ppOut) 
        {
            SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(),&api);
            if (FAILED(ret))
            {
                // log
               // LOGEP("Failed to open admin api.");
                return ret;
            }
            SP<ICustomerSubscriptionInfo> pUser;
            ErrorId retCode = pUser->Create(get_Session(),&pUser);
            if (FAILED(retCode))
            {
               // LOGE(_logger, "can't create User Type");
                return retCode;
            }
            pUser.CopyTo(ppOut);
            return ret;
            
        }

        ErrorId CCustomer::set_CurrentSubscriptionInfo(ICustomerSubscriptionInfo* pIn) 
        {
            _pCustomerSubscriptionInfo = pIn;
        }
 
        void CCustomer::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
}


