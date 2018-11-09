/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSecurityProviderBase.cpp
 * Author: santony
 * 
 * Created on September 3, 2016, 5:51 PM
 */

#include "CMarketDataProvider.h"

CMarketDataProvider::CMarketDataProvider(ISession* pSession, se::securities::ISecurityAPI* api) 
{
    this->pSession = pSession;
    this->pSecurityApi = api;
}

//CMarketDataProvider::CMarketDataProvider(const CMarketDataProvider& orig) 
//{
//}

CMarketDataProvider::~CMarketDataProvider() 
{
}

const char* CMarketDataProvider::get_CodeMessage(long unsigned int errorCode) 
{
    return pSecurityApi->get_CodeMessage(errorCode);
}


