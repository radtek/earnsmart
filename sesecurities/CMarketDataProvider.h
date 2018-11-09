/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSecurityProviderBase.h
 * Author: santony
 *
 * Created on September 3, 2016, 5:51 PM
 */

#ifndef CSECURITYPROVIDERBASE_H
#define CSECURITYPROVIDERBASE_H

#include "internal.h"


class CMarketDataProvider 
{
public:
    CMarketDataProvider(ISession* pSession, se::securities::ISecurityAPI* api);
    CMarketDataProvider(const CMarketDataProvider& orig) = delete;
    virtual ~CMarketDataProvider();
    
    virtual SE_CODE get_Security(const Json::Value securityDesc, Json::Value& output) = 0;
    virtual SE_CODE get_ExchangeSecurities(const Json::Value input, Json::Value& output) = 0;
protected:
    CSTR get_CodeMessage(SE_CODE errorCode);
    
protected:
    RP<ISession> pSession;
    RP<se::securities::ISecurityAPI> pSecurityApi;
    
private:

};

#endif /* CSECURITYPROVIDERBASE_H */

