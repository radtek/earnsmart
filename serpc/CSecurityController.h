/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CFinancialsController.h
 * Author: santony
 *
 * Created on September 3, 2016, 9:58 AM
 */

#ifndef CFinancialsController_H
#define CFinancialsController_H

#include "CRPCServiceController.h"

class CSecurityController : public CRPCServiceController
{
public:
    CSecurityController();
    CSecurityController(const CSecurityController& orig) = delete;
    virtual ~CSecurityController();
private:
    
    NXR GetSecurity(Json::Value& response, const Json::Value& args);
    NXR GetExchangeSecurities(Json::Value& response, const Json::Value& args);
    

};

#endif /* CFinancialsController_H */

