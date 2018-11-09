/* 
 * File:   CCustomerStatus.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 4:59 PM
 */

#include "internal.h"
#include "CCustomerStatus.h"

namespace se
{
    namespace admin
    {
        CCustomerStatus::CCustomerStatus(ISession* ps) : CObjectRoot(ps)
        {
        
        }


        CCustomerStatus::~CCustomerStatus() 
        {
        
        }
        
        ErrorId ICustomerStatus::Create(ISession* ps, ICustomerStatus** ppOut)
        {
            return CObject<CCustomerStatus>::Create(ps, ppOut);
        }
        
        CSTR CCustomerStatus::get_Name()
        {
            return _name.c_str();
        }
        
        void CCustomerStatus::set_Name(CSTR value)
        {
            _name = value;
        }
        
        void CCustomerStatus::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
    
}



