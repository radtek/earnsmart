/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CCommonController.h
 * Author: santony
 *
 * Created on April 4, 2016, 4:16 PM
 */

#ifndef CCOMMONCONTROLLER_H
#define CCOMMONCONTROLLER_H

#include "CRPCServiceController.h"

class CCommonController : public CRPCServiceController
{
public:
    CCommonController();
    CCommonController(const CCommonController& orig) = delete;
    virtual ~CCommonController();
private:

    NXR ListCountries(Json::Value& response);
    
    NXR ListStates(Json::Value& response, const Json::Value& countryinfo);
    
    NXR GetBrowserLocation(Json::Value& response);
    
};

#endif /* CCOMMONCONTROLLER_H */

