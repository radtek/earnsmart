/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CFinancialsController.cpp
 * Author: santony
 * 
 * Created on September 3, 2016, 9:58 AM
 */

#include "CSecurityController.h"

CSecurityController::CSecurityController() 
{
    AddAction("getsecurity", Function<const Json::Value&>(bind(&CSecurityController::GetSecurity, this, _1, _2)));
    AddAction("getexchsecurities", Function<const Json::Value&>(bind(&CSecurityController::GetExchangeSecurities, this, _1, _2)));
}


CSecurityController::~CSecurityController() 
{
}

NXR CSecurityController::GetSecurity(Json::Value& response, const Json::Value& args) 
{
    RP<ISecurityAPI> psapi = GetSession(response)->get_SecurityApi();
    Json::Value output;
    SE_CODE ret = psapi->get_Security(args, output);
    if(FAILED(ret))
    {
        InvalidAppOperationResponse(response, ret);
    }
    OkResponse(response, output);
    return N_OK;
}

NXR CSecurityController::GetExchangeSecurities(Json::Value& response, const Json::Value& args) 
{
    RP<ISecurityAPI> psapi = GetSession(response)->get_SecurityApi();
    Json::Value output;
    auto ret = psapi->get_ExchangeSecurities(args, output);
    if(FAILED(ret))
    {
        InvalidAppOperationResponse(response, ret);
    }
    OkResponse(response, output);
    return N_OK;
}
