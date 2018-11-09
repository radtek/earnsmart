/* 
 * File:   CCustomerSubscriptionType.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 4:53 PM
 */

#include "internal.h"
#include "CCustomerSubscriptionType.h"

namespace se
{
    namespace admin
    {
        CCustomerSubscriptionType::CCustomerSubscriptionType(ISession* ps) : CObjectRoot(ps)
        {
        
        }

        CCustomerSubscriptionType::~CCustomerSubscriptionType() 
        {
              
        }
        
        ErrorId ICustomerSubscriptionType::Create(ISession* ps, ICustomerSubscriptionType** ppOut)
        {
            return CObject<CCustomerSubscriptionType>::Create(ps, ppOut);
        }
        
        CSTR CCustomerSubscriptionType::get_Name()
        {
            return _name.c_str();
        }
        
        void CCustomerSubscriptionType::set_Name(CSTR value)
        {
            _name = value;
        }
        
        void CCustomerSubscriptionType::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
    
}


