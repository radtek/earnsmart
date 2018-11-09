/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEodDataProvider.h
 * Author: santony
 *
 * Created on September 3, 2016, 5:54 PM
 */

#ifndef CEODDATAPROVIDER_H
#define CEODDATAPROVIDER_H

#include "internal.h"
#include "CMarketDataProvider.h"

class CEodDataProvider : public CMarketDataProvider
{
public:
    CEodDataProvider(ISession* pSession, se::securities::ISecurityAPI* api);
    CEodDataProvider(const CEodDataProvider& orig) = delete;
    virtual ~CEodDataProvider();
    
    SE_CODE get_Security(const Json::Value securityDesc, Json::Value& output) override final;
    long unsigned int get_ExchangeSecurities(const Json::Value input, Json::Value& output) override;

    
private:
    
    void BuildSecurity(const Row& row, Json::Value& output);
    
    string getsecurityTable(CSTR exchName);
    string getpriceHistoryTable(CSTR exchName);
    string getfundamentalsTable(CSTR exchName);
    string gettechnicalsTable(CSTR exchName);
    
    Json::Value getsecurityTypes(CSmartConnection& conn);
    Json::Value getexchangeDetails(CSmartConnection& conn, CSTR exch);
    string getsecurityType(CSmartConnection& conn, UID id);
    Json::Value gettechnicals(CSmartConnection& conn, CSTR exchName, UID secId);
    Json::Value getfundamentals(CSmartConnection& conn, CSTR exchName, UID secId);

};

#endif /* CEODDATAPROVIDER_H */

