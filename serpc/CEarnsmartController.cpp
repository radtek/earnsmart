/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEarnsmartController.cpp
 * Author: santony
 * 
 * Created on March 8, 2016, 6:41 PM
 */

#include "CEarnsmartController.h"
#include "CAdminController.h"

CEarnsmartController::CEarnsmartController() 
{
    this->AddAction("getappdata", Function<const string&, const string&>(std::bind(&CEarnsmartController::GetAppData, this, _1)));
}

CEarnsmartController::~CEarnsmartController() 
{
    
}

NXR CEarnsmartController::GetAppData(Json::Value& response) 
{
    // TODO: Fetch application data from database.
    Json::Value appData ;
    appData["MarketdataVendor"] = "EODDATA";
    appData["Version"] = "0.0";
    OkResponse(response, appData);
    return NXWEB_OK;
}
