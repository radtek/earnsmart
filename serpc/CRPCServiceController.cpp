/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "internal.h"
#include "rpc.h"
#include "CRPCServiceController.h"
#include "CHttpUtils.h"
#include "CAdminController.h"
#include "CEarnsmartController.h"
#include "CCommonController.h"
#include "CHelpDocTreeController.h"
#include "CSecurityController.h"
#include "CEconomicsController.h"

extern SP<se::IApp> pApp;


ControllerMap CRPCServiceController::cachedControllers;
std::mutex CRPCServiceController::_lock;

CRPCServiceController::CRPCServiceController() 
{

}

CRPCServiceController::~CRPCServiceController()
{

}

void CRPCServiceController::Register() 
{
    std::lock_guard<std::mutex> g(_lock);
    {
        cachedControllers.insert(make_pair("/rpc", new CEarnsmartController()));
        cachedControllers.insert(make_pair("/rpc/admin", new CAdminController()));
        cachedControllers.insert(make_pair("/rpc/common", new CCommonController()));
        cachedControllers.insert(make_pair("/rpc/help", new CHelpDocTreeController()));
        cachedControllers.insert(make_pair("/rpc/fin/sec", new CSecurityController()));
        cachedControllers.insert(make_pair("/rpc/ecn", new CEconomicsController()));
    }
}

void CRPCServiceController::UnRegister() 
{
    std::lock_guard<std::mutex> g(_lock);
    {
        for (auto pr : cachedControllers)
        {
            delete pr.second;
        }
        cachedControllers.clear();
    }
}




/////////////////////// C++ Wrappers ///////////////////////

nxweb_result CRPCServiceController::GenerateCacheKey(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) 
{
    
    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnSelect(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) 
{
    
    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnHeaders(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) {

    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnPostData(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) {

    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnPostDataComplete(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) {

    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnComplete(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) {

    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnError(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) {

    return NXWEB_OK;
}

nxweb_result CRPCServiceController::OnRequest(CHttpConnection& conn, CHttpRequest& req, CHttpResponse& rsp) 
{
    CHttpContext context(conn, req, rsp);
    
    return OnRequestResponse(context);
    

}

NXR CRPCServiceController::OnRequestResponse(CHttpContext& context) 
{

    // WE ARE USING JSON-RPC 2.0 Specifications
    /* Basic Rules are :
     *  1.  The request is always posted
     *  2.  Request received is a json object
     *  3.  A single request object looks like
     *      { 
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // this is number we pass to uniquely identify method .
     *          "method" : "methodname",
     *          "params" : { "p1" : "p1value", "p2" : "p2value", ...... }
     *      }
     *  3.a. If the request is not expecting a response, nothing is returned. It is called a notification request.
     *  3.b. If a response is expected, the response object is json as well.
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "result" : 'value' // can be another json object as well.
     *      }
     *  3.c. If method is not found, the response will be
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32601, "message" : "Method not found." } 
     *      }
     *  3.d. If request is invalid (in this case: invalid json request)
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32700, "message" : "Parse error." } 
     *      }
     *  3.e. If request is invalid (in this case: request itself is not perfect (incorrect parameters for example)
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32600, "message" : "Invalid Request." } 
     *      }
     * 4. Batch Request is an array of individual request. Example
     *      [
     *          { 
     *              "jsonrpc" : "2.0",
     *              "id" : 1, // this is number we pass to uniquely identify method .
     *              "method" : "methodname1",
     *              "params" : { "p1" : "p1value", "p2" : "p2value", ...... }
     *          }
     *          {    
     *              "jsonrpc" : "2.0",
     *              "id" : 2, // this is number we pass to uniquely identify method .
     *              "method" : "methodname2",
     *              "params" : { "p1" : "p1value", "p2" : "p2value", ...... }
     *          }
     *      ]
     *      When a batch request is received, the controller can get individual responses
     *      and return a combination response as below.
     *      [
     *          {
     *              "jsonrpc" : "2.0",
     *              "id" : 1, // same as id of the request.
     *              "result" : 'value' // can be another json object as well.
     *          }
     *          {
     *              "jsonrpc" : "2.0",
     *              "id" : 2, // same as id of the request.
     *              "result" : 'value' // can be another json object as well.
     *          }
     *      ]
     *      In the agove response messages, some of them may be an error response detailed as 3.x above.
     *  4.a However if a batch request is invalid (such as empty array, invalid json, etc) return one of the 
     *      above appropriate message (3.x).
     */
    
    BDBG << "Receive request : " << context.Request().Content();
    
    NXR retValue = NXWEB_OK;

    Json::Value request, response;
    
    try
    {

        CSTR sessionId = context.Request().SessionCookie();

        BDBG << "Session Id: " << sessionId;

        CSTR requestStr = context.Request().Content();
        Json::Reader rdr;
        bool hasParsed = rdr.parse(requestStr, request, false);
        if (hasParsed && CheckJsonRpcRequestMessage(request))
        {
            // find if session is available. 'esid' is our session id
            // if not create automatically one and save the new session id in the cookie
            RP<ISessionInterface> pSession = this->OpenOrCreateSession(sessionId);
            BDBG << "Session Pointer Address:" << pSession._ptr;
            BDBG << "Session Id from session class: " << pSession->get_UniqueId();
            response["esid"] = pSession->get_UniqueId();

            pSession->set_Tag((POINTER)&context);

            if (request.isArray())
            {
                for(Json::Value& v : request)
                {
                    Json::Value subresponse;
                    retValue = ProcessAction(v, subresponse);
                    subresponse["jsonrpc"] = "2.0";
                    subresponse["id"] = v["id"].asString();
                    subresponse["esid"] = pSession->get_UniqueId();
                    response.append(subresponse);
                }
            }      
            else
            {
                response["jsonrpc"] = "2.0";                
                response["id"] = request["id"].asString();
                retValue = ProcessAction(request, response);
                BDBG << "Writing Session Id: " << pSession->get_UniqueId();
                response["esid"] = pSession->get_UniqueId();
            }
            
            pSession->set_Tag(nullptr);
            
        }
        else
        {
            response["jsonrpc"] = "2.0";                
            response["id"] = Json::nullValue;
            InvalidJsonResponse(response);
        }


        context.Response().SessionCookie(response["esid"].asCString());
        context.Response().ContentType("application/jsonrpc");
        
        Json::FastWriter writer;
        context.Response() << writer.write(response).c_str();

        
        return retValue;
    }
    catch(const std::exception& ex)
    {
        BERR << ex.what();
        context.Response() << "Request failed be executed by the server failed due to a unrecoverable exception. Please ensure that content-type is correctly set.";
    }
    catch(...)
    {
        BERR << "Fatal unhandled exception.";
        context.Response() << "Request failed be executed by the server failed due to a unrecoverable exception. Please ensure that content-type is correctly set.";
    }    
    
}



//////////// Macros to build variable number of parameters into a matching function /////////////
#define P0(x) x[0].asString()
#define P1(x) P0(x), x[1].asString()
#define P2(x) P1(x), x[2].asString()
#define P3(x) P2(x), x[3].asString()
#define P4(x) P3(x), x[4].asString()
#define P5(x) P4(x), x[5].asString()
#define P6(x) P5(x), x[6].asString()
#define P7(x) P6(x), x[7].asString()
#define P8(x) P7(x), x[8].asString()
#define P9(x) P8(x), x[9].asString()
#define P10(x) P9(x), x[10].asString()

#define P(x, i) P##i(x)



nxweb_result CRPCServiceController::ProcessAction(const Json::Value& request, Json::Value& response) 
{
    
    // NOTES:
    /*
     * 1.   Posted rpc message can only send a single json object as parameter. That case will be handled
     *      by "case 1" below.
     * 2.   If json rpc message contains more than one parameters, then individually they must be 
     *      delivered as string. In that case, one of the parameters cannot be another type includign
     *      a json object.
     * 3.   If these conditions are not met an exception is thrown by the server.
     */
    
    BERR << request;
    
    try
    {
        
        if (request.isMember("params") && request["params"].isArray() && request["params"].size())
        {
            auto params = request["params"];
            auto size = params.size();
            switch (size)
            {
                case 0:
                    return this->CallAction(request["method"].asString(), response);
                case 1:
                    if (params[0].isObject())
                        return this->CallAction(request["method"].asString(), response, params[0]);
                    else
                        return this->CallAction(request["method"].asString(), response, params[0].asString());
                case 2:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString());
                case 3:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString());
                case 4:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString(), params[3].asString());
                case 5:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString(), params[3].asString(), params[4].asString());
                case 6:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString(), params[3].asString(), params[4].asString(), params[5].asString());
                case 7:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString(), params[3].asString(), params[4].asString(), params[5].asString(), params[6].asString());
                case 8:
                    return this->CallAction(request["method"].asString(), response, params[0].asString(), params[1].asString(), params[2].asString(), params[3].asString(), params[4].asString(), params[5].asString(), params[6].asString(), params[7].asString());
                default:
                    return this->CallAction(request["method"].asString(), response);
            }
        }
        else
        {
            return this->CallAction(request["method"].asString(), response);
        }
    }
    catch(const std::exception& ex)
    {
        BERR << "Server json rpc processing error: " << ex.what();
        this->InvalidAppOperationResponse(response,  "Server encountered a fatal exception while processing the posted message");
    }
    catch(...)
    {
        BERR << "Fatal error while server processing json rpc posted message.";
        this->InvalidAppOperationResponse(response,  "Server encountered a fatal exception while processing the posted message");
    }
}


bool CRPCServiceController::CheckJsonRpcRequestMessage(const Json::Value& request) 
{
    if (request.isArray())
    {
        for(Json::Value v : request)
        {
            if (!CheckJsonRpcRequestMessage(v))
                return false;
        }
        return true;
    }
    else
        return (request.isMember("jsonrpc") && request.isMember("method") && request.isMember("id"));
}

void CRPCServiceController::InsufficientParameters(Json::Value& response) 
{

    /*
     *  3.e. If request is invalid (in this case: request itself is not perfect (incorrect parameters for example)
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32600, "message" : "Invalid Request." } 
     *      }
     */
    
    response["jsonrpc"] = "2.0";
    Json::Value error;
    error["code"] = E_JsonRpcInsufficientParameters; // -326000;
    error["message"] = "Invalid request. Check posted data for parameters passed.";
    response["error"] = error;        
}



void CRPCServiceController::InvalidJsonResponse(Json::Value& response) 
{
    /*
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32700, "message" : "Parse error." } 
     *      }
    */
    
    Json::Value error;
    error["code"] = E_JsonRpcInvalidResponse; // -327000;
    error["message"] = "Json parse error. Check posted data.";
    response["error"] = error;    

}

bool CRPCServiceController::IsMethodIdCorrect(const Json::Value& response, const string& id) 
{
    if (response.isMember("id"))
    {
        if (response["id"].asString() == id)
        {
            return true;
        }
    }
    return false;
}


void CRPCServiceController::MethodIdIncorrect(Json::Value& response) 
{
    /*
     *  3.c. If method is not found, the response will be
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32601, "message" : "Method not found." } 
     *      }
     */
    response["jsonrpc"] = "2.0";
    Json::Value error;
    error["code"] = E_JsonRpcIncorrectMethod; // -326000;
    error["message"] = "Method ID incorrect. Check 'id' value in posted data and ensure that id corresponds to the method name.";
    response["error"] = error;
}


void CRPCServiceController::MethodNotFoundResponse(string methodName, Json::Value& response) 
{

    /*
     *  3.c. If method is not found, the response will be
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32601, "message" : "Method not found." } 
     *      }
     */
    response["jsonrpc"] = "2.0";
    Json::Value error;
    error["code"] = E_JsonRpcMethodNotFound; // -326001;
    error["message"] = "Method not found. Check 'method' value in posted data.";
    response["error"] = error;
}

void CRPCServiceController::InvalidAppOperationResponse(Json::Value& response, const string& errorMessage) 
{
    response["jsonrpc"] = "2.0";
    Json::Value error;
    error["code"] = E_UnknownError; // -32000
    error["message"] = errorMessage;
    response["error"] = error;
}


void CRPCServiceController::InvalidAppOperationResponse(Json::Value& response, SE_CODE errorCode)
{
    /*
     * Server error codes range between -32000 to -32099
     * 
     *  3.c. If method is not found, the response will be
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "error" : { "code" : -32000, "message" : "Method not found." } 
     *      }
     */
    
    response["jsonrpc"] = "2.0";
    Json::Value error;
    error["code"] = (Json::Int)errorCode; //(Json::Int64)(  -1 * (0x7D000 | (0x7DFFF & errorCode) ));   // default json-rpc error code error-32000 = 0x7D00
    error["message"] = pApp->get_CodeMessage(errorCode);
    response["error"] = error;
}


void CRPCServiceController::OkResponse(Json::Value& output, const Json::Value& result) 
{

    /*
     *      {
     *          "jsonrpc" : "2.0",
     *          "id" : 1, // same as id of the request.
     *          "result" : 'value' // can be another json object as well.
     *      }
     */
    output["result"] = result;
}

void CRPCServiceController::OkResponse(Json::Value& output) 
{
    output["result"] = "OK";
}

void CRPCServiceController::OkAppOperationResponse(Json::Value& output, long unsigned int code, Json::Value data, const char* message) 
{
    if (ISOK(code))
    {
        if (data == Json::nullValue)
        {
            OkResponse(output, (Json::Int)code);
        }
        else
        {
            output["code"] = (Json::Int)code;
            output["message"] = "OK";
            output["result"] = data;
        }
        return;
    }
    else
    {
        if (SUCCEEDED(code))
        {
            output["code"] = (Json::Int)code;
            if (!message)
            {
                output["message"] = pApp->get_CodeMessage(code);
                output["result"] = data;
            }
            else
            {
                output["message"] = message;
                output["result"] = data;
            }
        }
    }
}


void CRPCServiceController::SECodeResponse(Json::Value& output, long unsigned int code, const Json::Value data) 
{
    if (FAILED(code))
    {
        InvalidAppOperationResponse(output, code);
        return;
    }
    if (SUCCEEDED(code))
    {
        OkAppOperationResponse(output, code, data);
    }
}


void CRPCServiceController::ClearSession(Json::Value& response) 
{
    response["esid"] = "";
}



bool CRPCServiceController::TryController(const string& url, CRPCServiceController*& controller) 
{
    ControllerMap::iterator it;
    it = cachedControllers.find(url);
    if (it != cachedControllers.end())
    {
        controller = it->second;
        return true;
    }
    return false;
}

RP<ISessionInterface> CRPCServiceController::OpenOrCreateSession(const string& sessionId) 
{
    return ((IAppEx*)pApp._ptr)->OpenSession(sessionId.c_str()); 
}

RP<ISessionInterface> CRPCServiceController::GetSession(const Json::Value& response) 
{
    RP<ISessionInterface> pSession = ((IAppEx*)pApp._ptr)->OpenSession( response.get("esid", "").asCString() ); 
    //assert( pSession != nullptr);
    return pSession;
}



///////////////// nxlib invocation ////////////////////////

CEarnsmartController ServiceController;


#define WRAPHTTPCLASSES(conn, req, resp, method) \
    CRPCServiceController* controller; \
    if (!ServiceController.TryController(req->uri, controller)) \
        return NXWEB_ERROR ; \
    if (controller != nullptr) \
    { \
        CHttpConnection connection(conn); \
        CHttpRequest request(req); \
        CHttpResponse response(resp); \
        response.Secure(conn->secure); \
        return controller->method(connection, request, response); \
    } \
    return NXWEB_ERROR;


nxweb_result rpchandler_onload()
{
    BINF << "Handler rpc is called for once at start up.";
    ServiceController.Register();
    return NXWEB_OK;
}

nxweb_result rpchandler_onunload()
{
    BINF << "Handler rpc is called for once at unload.";
    ServiceController.UnRegister();
    return NXWEB_OK;
}


nxweb_result rpchandler_generate_cache_key(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, GenerateCacheKey)
}

nxweb_result rpchandler_on_select(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    BINF << req->path_info << endl;
    WRAPHTTPCLASSES(conn, req, resp, OnSelect)
}

nxweb_result rpchandler_on_headers(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, OnHeaders)
}

nxweb_result rpchandler_on_post_data(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, OnPostData)

}

nxweb_result rpchandler_on_post_data_complete(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, OnPostDataComplete)

}

nxweb_result rpchandler_on_complete(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, OnComplete)

}

nxweb_result rpchandler_on_error(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    WRAPHTTPCLASSES(conn, req, resp, OnError)

}

nxweb_result rpchandler_on_request(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp)
{
    
    WRAPHTTPCLASSES(conn, req, resp, OnRequest)

/*    
    

    nxweb_set_response_content_type(resp, "text/html");

    nxweb_response_append_str(resp, "<html><head><title>Hello, nxweb!</title></head><body>");

    /// Special %-printf-conversions implemeted by nxweb:
    //    %H - html-escape string
    //    %U - url-encode string
    nxweb_response_printf(resp, "<p>Received request:</p>\n<blockquote>"
            "connection_id=%016" PRIx64 "<br/>\n"
            "request_id=%016" PRIx64 "<br/>\n"
            "remote_addr=%H<br/>\n"
            "method=%H<br/>\n"
            "uri=%H<br/>\n"
            "path_info=%H<br/>\n"
            "http_version=%H<br/>\n"
            "http11=%d<br/>\n"
            "ssl=%d<br/>\n"
            "keep_alive=%d<br/>\n"
            "host=%H<br/>\n"
            "cookie=%H<br/>\n"
            "user_agent=%H<br/>\n"
            "content_type=%H<br/>\n"
            "content_length=%ld<br/>\n"
            "content_received=%ld<br/>\n"
            "transfer_encoding=%H<br/>\n"
            "accept_encoding=%H<br/>\n"
            "request_body=%H</blockquote>\n",
            conn->uid,
            req->uid,
            conn->remote_addr,
            req->method,
            req->uri,
            req->path_info,
            req->http_version,
            req->http11,
            conn->secure,
            req->keep_alive,
            req->host,
            req->cookie,
            req->user_agent,
            req->content_type,
            req->content_length,
            req->content_received,
            req->transfer_encoding,
            req->accept_encoding,
            req->content
            );



    if (req->headers)
    {
        nxweb_response_append_str(resp, "<h3>Headers:</h3>\n<ul>\n");
        nx_simple_map_entry* itr;
        for (itr = nx_simple_map_itr_begin(req->headers); itr; itr = nx_simple_map_itr_next(itr))
        {
            nxweb_response_printf(resp, "<li>%H=%H</li>\n", itr->name, itr->value);
        }
        nxweb_response_append_str(resp, "</ul>\n");
    }

    if (req->parameters)
    {
        nxweb_response_append_str(resp, "<h3>Parameters:</h3>\n<ul>\n");
        nx_simple_map_entry* itr;
        for (itr = nx_simple_map_itr_begin(req->parameters); itr; itr = nx_simple_map_itr_next(itr))
        {
            nxweb_response_printf(resp, "<li>%H=%H</li>\n", itr->name, itr->value);
        }
        nxweb_response_append_str(resp, "</ul>\n");
    }

    if (req->cookies)
    {
        nxweb_response_append_str(resp, "<h3>Cookies:</h3>\n<ul>\n");
        nx_simple_map_entry* itr;
        for (itr = nx_simple_map_itr_begin(req->cookies); itr; itr = nx_simple_map_itr_next(itr))
        {
            nxweb_response_printf(resp, "<li>%H=%H</li>\n", itr->name, itr->value);
        }
        nxweb_response_append_str(resp, "</ul>\n");

        CThinMap map = req->cookies;
        nxweb_response_append_str(resp, "<h3>Cookies:</h3>\n<ul>\n");
        for (auto itr = map.begin(); itr != map.end(); ++itr)
        {
            nxweb_response_printf(resp, "<li>%H=%H</li>\n",  ((nx_simple_map_entry*)itr)->name, ((nx_simple_map_entry*)itr)->value );
        }
    }
    
    

    nxweb_response_append_str(resp, "</body></html>");

    
    

    char cookie[1024];
    if (conn->secure)
        sprintf(cookie, "SessionId=%ull;path=/;secure\\0", 20012);
    else
        sprintf(cookie, "SessionId=%ull;path=/\\0", 20012);

    nxweb_add_response_header(resp, "Set-Cookie", cookie);
    

    //return NXWEB_OK;

    */

}

