/* 
 * File:   CJsonRPCApp.cpp
 * Author: santony
 * 
 * Created on September 1, 2015, 3:33 PM
 */

#include <booster/log.h>
#include <cppcms/http_response.h>

#include "CJsonRPCApp.h"
#include <jsoncpp/json/json.h>
#include <cppcms/http_request.h>
#include <vector>
#include <cppcms/http_context.h>

#include "../seadmin/seadminapi.h"

using namespace Json;
using namespace se;
using namespace se::admin;

// json errors

#define CHECKID(ID) \
    if (id != ID) \
    { \
        ReturnError(-32600, "Invalid Request", id, "id doesn't match method, must be 1"); \
        return; \
    } \

CJsonRPCApp::CJsonRPCApp(cppcms::service &srv) : 
    cppcms::application(srv)
{
    
    dispatcher().assign("", &CJsonRPCApp::Default, this);
    mapper().assign("");
}

CJsonRPCApp::~CJsonRPCApp() 
{
    
}

void CJsonRPCApp::ReturnError(int code, const string& message, int id, const string& data) 
{
    Json::Value root;
    root["code"] = code;
    root["message"] = message;
    root["data"] = data;
    if (id)
        root["id"] = id;
    response().set_content_header("application/json-rpc");
    response().out() << root;
}

void CJsonRPCApp::ReturnSuccess(int id, const Json::Value& value) 
{
    Json::FastWriter writer;
    Json::Value root;
    root["result"] =  value;
    root["id"] = id;
    root["error"] = Json::nullValue;
    response().set_content_header("application/json-rpc");
    response().out() << writer.write(root);
}


void CJsonRPCApp::Default() 
{
    // this is an rpc 2.0 call.
    auto& req = request();
    string posted = req.post("cdata");
    if (posted.empty())
        return;
    
    Json::Value calldata;
    Json::Reader rdr;
    if (!rdr.parse(posted, calldata, false))
    {
        ReturnError(-32700, "Parse Error");
        return;
    }
    int id = calldata.get("id", 0).asInt();
    string method = calldata.get("method", "").asString();
    string stoken = calldata.get("token", "").asString();
    Json::Value params = calldata.get("params", Json::Value::null);
    try
    {
        if (!id)
        {
            ReturnError(-32600, "Invalid Request", id, "id cannot be zero");
            return;
        }
        
        if (method == "div")
        {
            if (params.size() < 2)
                throw invalid_argument("Not enough parameters given");
            div(id, stoken, params[0].asDouble(), params[1].asDouble());
        }
        else if (method == "countries")
        {
            // if no params
            if (!params.size())
            {
                Countries(id, stoken);
            }
            // else with params like country id or country name or code
            else 
            {
                const string code = params[0].asString();
                Countries(id, stoken, code);
            }
        }
        else if (method == "checkloginid")
        {
            if (params.size()<1)
                throw invalid_argument("Not enough parameters given");
            CheckLoginId(id, stoken, params[0].asString());
        }
        else
        {
            ReturnError(-32601, "Method not found");
        }
    }
    catch (const std::invalid_argument& ex)
    {
        ReturnError(-32602, "Invalid params", id, ex.what());
    }
    catch(const std::exception& ex)
    {
        ReturnError(-32603, "Internal error", id, ex.what());
    }
}

void CJsonRPCApp::CheckLoginId(int id, const string& token, const string& emailAddress) 
{
    BDBG << "CheckLoginId called";
    CHECKID(3)
    SP<IAdminAPI> papi;
    auto rCode = AdminOpen(&papi);
    if (ISOK(rCode))
    {
        rCode = papi->CheckUserLogin(emailAddress.c_str());
        ReturnSuccess(id, rCode == S_OK );
        return;
    }
    ReturnError(-33001, "Failed to check login id.");
}


void CJsonRPCApp::Countries(int id, const string& token, const string& code) 
{ 
    BDBG << "Countries called";
    CHECKID(2)
    SP<IAdminAPI> padm;
    if (ISOK(AdminOpen(&padm)))
    {
        Json::Value countries;
        if (code.empty())
        {
            if (ISOK(padm->get_Countries(countries)))
            {

                ReturnSuccess(id, countries);
                return;
            }
        }
        else
        {
            if (ISOK(padm->get_Countries(countries, [](ICountry::QUERYFLDS fld, LGX& lg, BOOL& b) -> CSTR
            {
                if (fld == ICountry::QUERYFLDS::TwoChar)
                {
                    b = true;
                    return "US";
                }
                return "";
            }
            )
            )
            )
            {

                ReturnSuccess(id, countries);
                return;
            }
        }
    }
    ReturnError(-33000, "Failed to list countries.");
}

void CJsonRPCApp::div(int id, const string& token, double x, double y) 
{
    CHECKID(1)
    ReturnSuccess(id, x / y);
}


