/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAdminController.h
 * Author: santony
 *
 * Created on March 8, 2016, 5:08 PM
 */

#ifndef CADMINCONTROLLER_H
#define CADMINCONTROLLER_H

#include "CRPCServiceController.h"

class CAdminController : public CRPCServiceController
{
public:
    CAdminController();
    CAdminController(const CAdminController& orig) = delete;
    virtual ~CAdminController();

    
private:
    
    NXR VerifySession(Json::Value& response);

    // Sign in an user.
    NXR Login(Json::Value& response, const Json::Value& loginInfo);
    
    NXR ResetPassword(Json::Value& response, const string& emailAddress);
    
    NXR ConfirmEmail(Json::Value& response, const string& esid);
    
    // Register a user. The 2nd paramter is a json formatted string
    nxweb_result RegisterUser(Json::Value& response, const Json::Value& registrationInfo);
    
    // Request user's account details. Only user's session id is a permitted parameter.
    NXR RequestUserAccount(Json::Value& response, const string& usersessionId);
    
    NXR ExchangePackages(Json::Value& response);
    
    NXR CheckUser(Json::Value& response, const string& emailAddress);

    NXR UpgradeUser(Json::Value& response, const Json::Value& upgradeInfo);
    
    NXR DowngradeUser(Json::Value& response, const string& loginid);
};

#endif /* CADMINCONTROLLER_H */

