/* 
 * File:   CProduct.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 5:12 PM
 */

#include "internal.h"
#include "CProduct.h"

namespace se
{
    namespace admin
    {
        CProduct::CProduct(ISession* ps) : CObjectRoot(ps)
        {
            
        }


        CProduct::~CProduct() 
        {
            
        }
        
        ErrorId IProduct::Create(ISession* ps, IProduct** ppOut)
        {
            return CObject<CProduct>::Create(ps, ppOut);
        }

        CSTR CProduct::get_Name()
        {
            return _name.c_str();
        }

        void CProduct::set_Name(CSTR name) 
        {
            _name = name;
        }

        R4 CProduct::get_PriceMonthlyRate() 
        {
            return _priceMonthlyRate;
        }

        R4 CProduct::get_PriceQuarterlyRate() 
        {
            return _priceQuarterlyRate;
        }

        R4 CProduct::get_PriceYearlyRate() 
        {
            return _priceYearlyRate;
        }

        void CProduct::set_PriceMonthlyRate(R4 value) 
        {
            _priceMonthlyRate = value;
        }

        void CProduct::set_PriceQuarterlyRate(R4 value) 
        {
            _priceQuarterlyRate = value;
        }

        void CProduct::set_PriceYearlyRate(R4 value) 
        {
            _priceYearlyRate = value;
        }

        void CProduct::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
}
