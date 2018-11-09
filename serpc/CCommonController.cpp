/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CCommonController.cpp
 * Author: santony
 * 
 * Created on April 4, 2016, 4:16 PM
 */

#include "internal.h"
#include "CCommonController.h"
#include "../seglobal/CAst.h"

CCommonController::CCommonController() 
{
    AddAction("countries", Function<>(bind(&CCommonController::ListCountries, this, _1)));
    AddAction("states", Function<const Json::Value&>(bind(&CCommonController::ListStates, this, _1, _2)));
}

CCommonController::~CCommonController() 
{
}

NXR CCommonController::ListCountries(Json::Value& response) 
{
    RP<ISessionInterface> pSession = GetSession(response);
    assert(pSession != nullptr);
    RP<IAdminAPI> pAdm = pSession->get_AdminApi();
    assert(pAdm != nullptr);
    Json::Value countries;
    auto retCode = pAdm->get_Countries(countries); // list all countries
    assert(SUCCEEDED(retCode));
    if (SUCCEEDED(retCode))
        response["result"] = countries;
    else
        InvalidAppOperationResponse(response, retCode);
    return NXWEB_OK;
}

NXR CCommonController::ListStates(Json::Value& response, const Json::Value& countryinfo) 
{
    if (!countryinfo.isMember("CountryId") && !countryinfo.isMember("CountryCode"))
    {
        InvalidAppOperationResponse(response, "Country Id or Code not provided.");
    }
    else
    {
        RP<ISessionInterface> pSession = GetSession(response);
        assert(pSession != nullptr);
        RP<IAdminAPI> pAdm = pSession->get_AdminApi();
        assert(pAdm != nullptr);
        Json::Value states;
        CSqlAst* past;
        if (countryinfo.isMember("CountryId"))
        {
            past = new CSqlAst(AST_CONDITION(QUERYFIELDS::Country::Id, AST_OPERATOR::Equal, countryinfo["CountryId"].asInt()));
        }
        else
        {
            past = new CSqlAst(AST_CONDITION(QUERYFIELDS::Country::Code, AST_OPERATOR::Equal, countryinfo["CountryCode"].asCString()));
        }
        auto retCode = pAdm->get_States(states, past );
        if (SUCCEEDED(retCode))
        {
            response["result"] = states;
        }
        else
        {
            InvalidAppOperationResponse(response, retCode);
        }
        delete past;
    }
    return NXWEB_OK;
}

NXR CCommonController::GetBrowserLocation(Json::Value& response) 
{
    RP<ISessionInterface> pSession = GetSession(response);
    assert(pSession != nullptr);

    return NXWEB_OK;
}
