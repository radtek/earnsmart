/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAdminController.cpp
 * Author: santony
 * 
 * Created on March 8, 2016, 5:08 PM
 */

#include <tar.h>

#include "CAdminController.h"
#include "../senet/CEmailTemplate.h"
#include "../seglobal/CFile.h"
#include "../senet/CSmtpClient.h"
#include "../seglobal/exceptions.h"
#include "CEmailNotifications.h"

extern SP<se::IApp> pApp;

using namespace se::net;
using namespace se::admin;

CAdminController::CAdminController() 
{
    AddAction("verifysession", Function<>(bind(&CAdminController::VerifySession, this, _1)));
    AddAction("login", Function<const Json::Value&>(bind(&CAdminController::Login, this, _1, _2)));
    AddAction("resetpwd", Function<const string&>(bind(&CAdminController::ResetPassword, this, _1, _2)));
    AddAction("register", Function<const Json::Value&>(bind(&CAdminController::RegisterUser, this, _1, _2)));
    AddAction("exchangepackages", Function<>(bind(&CAdminController::ExchangePackages, this, _1)));
    AddAction("confirmemail", Function<const string&>(bind(&CAdminController::ConfirmEmail, this, _1, _2)));
    AddAction("checkuser", Function<const string&>(bind(&CAdminController::CheckUser, this, _1, _2)));
    AddAction("upgradeuser", Function<const Json::Value&>(bind(&CAdminController::UpgradeUser, this, _1, _2)));
    AddAction("downgradeuser", Function<const string&>(bind(&CAdminController::DowngradeUser, this, _1, _2)));
}

CAdminController::~CAdminController() 
{
    
}

NXR CAdminController::VerifySession(Json::Value& response) 
{
    RP<ISessionInterface> pSession = GetSession(response);

    // construct message.
    Json::Value sessionInfo ;
    sessionInfo["isauthenticated"] = pSession->get_IsAuthenticated();
    RP<se::admin::IUser> pUser = nullptr;
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    pUser = pSession->get_User();
    if (pUser)
    {
        sessionInfo["firstName"] = pUser->get_FirstName();
        sessionInfo["lastName"] = pUser->get_LastName();
        sessionInfo["userName"] = pUser->get_UserId();
        sessionInfo["regstage"] = (I32)pUser->get_RegistrationStage();
        sessionInfo["role"] = pSession->get_UserRoleStr()->get_Buffer();
    }
    else
    {
        sessionInfo["firstName"] = "";
        sessionInfo["lastName"] = "";
        sessionInfo["userName"] = "";
        sessionInfo["role"] = "anonymous";
        sessionInfo["regstage"] = (I32)(se::admin::REGSTAGES::UserTypeAndAddress);
    }
    
    OkResponse(response, sessionInfo);
}


nxweb_result CAdminController::Login(Json::Value& response, const Json::Value& signindata) 
{
    if (!IsMethodIdCorrect(response, "1000"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    
    RP<ISessionInterface> pSession = GetSession(response);
    
    BDBG << signindata;
    
    // attempt to parse json
    SE_CODE ret = pSession->get_AdminApi()->AuthenticateUser(
            signindata.get("emailAddress", (Json::Value)"unknown").asCString(), 
            signindata.get("pwd", (Json::Value)"unknown").asCString(),
            true
        );
    
    if (FAILED(ret))
    {
        InvalidAppOperationResponse(response, ret);
    }
    else
    {
        // capture user infro.
        Json::Value result ;
        result["user"] = pSession->get_UserName();
        result["isauthenticated"] = pSession->get_IsAuthenticated();
        result["role"] = pSession->get_UserRoleStr()->get_Buffer();
        result["status"] = "OK";
        OkResponse(response, result);
    }
    return NXWEB_OK;
}

NXR CAdminController::ResetPassword(Json::Value& response, const string& emailAddress) 
{
    RP<ISessionInterface> pSession = GetSession(response);

    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    
    SP<IString> pTmpPwd;
    auto retValue = padmin->ResetGetTemporaryPassword(emailAddress.c_str(), &pTmpPwd);
    if (FAILED(retValue))
    {
        BERR << "Failed to generate temporary password with error " << padmin->get_CodeMessage(retValue);
        InvalidAppOperationResponse(response, "Failed to reset password. Please try again.");
        return NXWEB_OK;
    }
    
    CHttpContext* httpContext = static_cast<CHttpContext*>(pSession->get_Tag()); 
    
    const string& signInUrl = httpContext->HostName() + "#/signon"; 

    CEmailNotifications notifier(pSession);
    retValue = notifier.NotifyResetPassword(emailAddress, pTmpPwd->get_Buffer(), signInUrl);
    if (FAILED(retValue))
    {
        InvalidAppOperationResponse(response, "Failed to reset password. Please try again.");
        return NXWEB_OK;
    }
    OkResponse(response, "Success");
    return NXWEB_OK;
}


nxweb_result CAdminController::RegisterUser(Json::Value& response, const Json::Value& reginfo)
{
    if (!IsMethodIdCorrect(response, "1005"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    RP<ISessionInterface> pSession = GetSession(response);
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    
    BDBG << reginfo;
    
    auto retCode = padmin->RegisterUser(reginfo);
    if (FAILED(retCode))
    {
        InvalidAppOperationResponse(response, retCode);
    }
    else
    {
        OkResponse(response);
    }
    return N_OK;
}

NXR CAdminController::RequestUserAccount(Json::Value& response, const string& usersessionId)
{
    // if usersessionid doesn't exist return invalid response.
    
    Json::Value accountInfo ;
    
    // fill in account info.

    OkResponse(response, accountInfo );
    
    return N_OK;
}

NXR CAdminController::ExchangePackages(Json::Value& response) 
{
    RP<ISessionInterface> pSession = GetSession(response);
    
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);

    Json::Value fees;
    auto retCode = padmin->get_ExchangePackages(fees);
    if (FAILED(retCode))
    {
        InvalidAppOperationResponse(response, retCode);
    }
    else
    {
        OkResponse(response, fees);
    }
    return N_OK;
}

NXR CAdminController::ConfirmEmail(Json::Value& response, const string& esid) 
{
    RP<ISessionInterface> pSession = GetSession(response);
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    auto sessionId = pSession->get_UniqueId();
    auto confirmSessionId = esid.c_str();
    if (!strcmp(sessionId, confirmSessionId))
    {
        // extend session date/time
        Json::Value regInfo ;
        regInfo["RegStages"] = REGSTAGES::EmailConfirmed;
        auto retCode = padmin->RegisterUser(regInfo);
        if (SUCCEEDED(retCode))
        {
            OkResponse(response);
        }
        else
        {
            InvalidAppOperationResponse(response, retCode);
        }
    }
    else
    {
        ClearSession(response);
        InvalidAppOperationResponse(response, R"(Registration session has expired. Please click <a href="#/register">here</a> to try again.)");
    }
    return N_OK;
}

NXR CAdminController::CheckUser(Json::Value& response, const string& emailAddress) 
{
    RP<ISessionInterface> pSession = GetSession(response);
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    SE_CODE ret = padmin->CheckUserLogin(emailAddress.c_str());
    OkResponse(response, ISOK(ret));
    return N_OK;
}

NXR CAdminController::UpgradeUser(Json::Value& response, const Json::Value& upgradeInfo) 
{
    if (!IsMethodIdCorrect(response, "1007"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    RP<ISessionInterface> pSession = GetSession(response);
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    auto ret = padmin->UpgradeUser(upgradeInfo);
    SECodeResponse(response, ret);
    return N_OK;

}

NXR CAdminController::DowngradeUser(Json::Value& response, const string& loginid) 
{
    if (!IsMethodIdCorrect(response, "1008"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    RP<ISessionInterface> pSession = GetSession(response);
    RP<IAdminAPI> padmin = pSession->get_AdminApi();
    assert(padmin != nullptr);
    auto ret = padmin->DowngradeUser(loginid.c_str());
    SECodeResponse(response, ret);
    return N_OK;
}
