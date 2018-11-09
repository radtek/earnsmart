/* 
 * File:   CCustomerSubscriptionInfo.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 5:24 PM
 */

#include "internal.h"
#include "CCustomerSubscriptionInfo.h"

namespace se
{
    namespace admin
    {
        CCustomerSubscriptionInfo::CCustomerSubscriptionInfo(ISession* ps) : CObjectRoot(ps) 
        {
        
        }

        CCustomerSubscriptionInfo::~CCustomerSubscriptionInfo() 
        {
        
        }
        
        ErrorId ICustomerSubscriptionInfo::Create(ISession* ps, ICustomerSubscriptionInfo** ppOut)
        {
            return CObject<CCustomerSubscriptionInfo>::Create(ps, ppOut);
        }

        ErrorId CCustomerSubscriptionInfo::get_Customer(ICustomer** ppOut) 
        {
		/*
            if (!_customerId) return E_NotFound;
            SP<IAdminAPI> api;
            ErrorId retCode = AdminOpen(get_Session(), &api);
            if (FAILED(retCode)) return retCode;
            return api->get_Customer(_customerId,ppOut);
		*/
		return E_NotImpl;
        }
        
        void CCustomerSubscriptionInfo::set_CustomerId(UID customerId) 
        {
            _customerId = customerId;
        }
        
        DATETIME CCustomerSubscriptionInfo::get_StartedOn() 
        {
            return _startedOn;
        }
        
        void CCustomerSubscriptionInfo::set_Started(DATETIME const& value) 
        {
           _startedOn = value;
        }
        
        DATETIME CCustomerSubscriptionInfo::get_EndedOn() 
        {
           return _endedOn;
        }
        
        void CCustomerSubscriptionInfo::set_EndedOn(DATETIME const& value) 
        {
            _endedOn = value;
        }
        
        RP<ICustomerStatus> CCustomerSubscriptionInfo::get_Status() 
        {
            return _pCustomerStatus;
        }
        
        void CCustomerSubscriptionInfo::set_Status(ICustomerStatus* pIn) 
        {
            _pCustomerStatus = pIn;
        }
        
        RP<ICustomerSubscriptionType> CCustomerSubscriptionInfo::get_Type() 
        {
            
            return _pCustomerSubscriptionType;
        }
        
        void CCustomerSubscriptionInfo::set_SubscriptionType(ICustomerSubscriptionType* pIn) 
        {
            _pCustomerSubscriptionType = pIn;
        }
        
        PERIOD CCustomerSubscriptionInfo::get_PayFrequency() 
        {
            return _payFrequency;
        }
        
        void CCustomerSubscriptionInfo::set_PayFrequency(PERIOD const& value) 
        {
            _payFrequency = value;
        }
        
        void CCustomerSubscriptionInfo::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
    
}

