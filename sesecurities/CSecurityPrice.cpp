/* 
 * File:   CSecurityPrice.cpp
 * Author: aashok
 * 
 * Created on October 7, 2013, 7:19 AM
 */
#include "internal.h"
#include "CSecurityPrice.h"

CSecurityPrice::CSecurityPrice(ISession* ps) : CObjectRoot(ps) 
{

}

CSecurityPrice::~CSecurityPrice() 
{
}

CDate& CSecurityPrice::get_Period()
{
    return _dt;
}

R4 CSecurityPrice::get_Price()
{
    return _price;
}
    
R4 CSecurityPrice::get_Volume()
{
    return _volume;
}

void CSecurityPrice::Serialize(ISerializedData<IString>* pIn)
{
    pIn->set_TypeName("se::securities::ISecurityPrice");
    pIn->Add("price", _price);
    pIn->Add("volume", _volume);
}

void CSecurityPriceEx::Serialize(ISerializedData<IString>* pIn)
{
    pIn->set_TypeName("se::securities::ISecurityPriceEx");
    pIn->Add("price", _price);
    pIn->Add("volume", _volume);
    pIn->Add("high", _high);
    pIn->Add("low", _low);
    pIn->Add("close", _close);
    pIn->Add("adjClose", _adjClose);
}
R4 CSecurityPriceEx::get_High()
{
    return _high;
}

R4 CSecurityPriceEx::get_Low()
{
    return _low;
}

R4 CSecurityPriceEx::get_Close()
{
    return _close;
}

R4 CSecurityPriceEx::get_adjClose()
{
    return _adjClose;
}