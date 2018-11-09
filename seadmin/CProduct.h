/* 
 * File:   CProduct.h
 * Author: aashok
 *
 * Created on August 31, 2013, 5:12 PM
 */

#ifndef CPRODUCT_H
#define	CPRODUCT_H


#include "seadminapi.h"
#include <string>

using namespace std;

namespace se
{
    namespace admin
    {
        class CProduct : public CObjectRoot, public IProduct
        {
            public:
                CProduct(ISession*);
                CProduct(const CProduct& orig)=delete;
                virtual ~CProduct();
                
                virtual CSTR get_Name() ;
                virtual void set_Name(CSTR name) ;
                virtual R4 get_PriceMonthlyRate() ;
                virtual R4 get_PriceQuarterlyRate() ;
                virtual R4 get_PriceYearlyRate() ;
                virtual void set_PriceMonthlyRate(R4 value) ;
                virtual void set_PriceQuarterlyRate(R4 value) ;
                virtual void set_PriceYearlyRate(R4 value) ;
                virtual void Serialize(ISerializedData<IString>* pIn) ;
                
            private:
                
                string _name;
                R4 _priceMonthlyRate, _priceQuarterlyRate, _priceYearlyRate;

        };
    }
}


#endif	/* CPRODUCT_H */

