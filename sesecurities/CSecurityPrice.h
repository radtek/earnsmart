/* 
 * File:   CSecurityPrice.h
 * Author: aashok
 *
 * Created on October 7, 2013, 7:19 AM
 */

#ifndef CSECURITYPRICE_H
#define	CSECURITYPRICE_H
#include "sesecuritiesapi.h"

using namespace std;
using namespace se;
using namespace se::securities;


class CSecurityPrice :public CObjectRoot, public ISecurityPrice
{
public:
    CSecurityPrice(ISession*);
    CSecurityPrice(const CSecurityPrice& orig)=delete;
    virtual ~CSecurityPrice();
    
    virtual CDate& get_Period();
    virtual R4 get_Price();
    virtual R4 get_Volume();

    virtual void Serialize(ISerializedData<IString>* pIn);
    
private:
    
public:
    CDate _dt;
    R4 _price,_volume;
};

class CSecurityPriceEx : public ISecurityPriceEx, public CSecurityPrice 
{
public:
    
    CSecurityPriceEx(ISession* ps) : CSecurityPrice(ps){}
    
    virtual ErrorId Init() 
    {
        RETURNIFFAILED(CSecurityPrice::Init());
        
        return S_OK;
    }
    
    
    virtual CDate& get_Period() { return CSecurityPrice::get_Period(); }
    virtual R4 get_Price() { return CSecurityPrice::get_Price(); }
    virtual R4 get_Volume() { return CSecurityPrice::get_Volume(); }
    
    virtual R4 get_High();
    virtual R4 get_Low();
    virtual R4 get_Close();
    virtual R4 get_adjClose();
    virtual void Serialize(ISerializedData<IString>* pIn);
   
public:
    R4 _high,_low,_close,_adjClose;
    
};

#endif	/* CSECURITYPRICE_H */

