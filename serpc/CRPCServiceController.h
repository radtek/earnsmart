/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CJsonCalls.h
 * Author: santony
 *
 * Created on December 27, 2015, 11:01 PM
 */

#ifndef CJSONCALLS_H
#define CJSONCALLS_H

#include "internal.h"
#include <string>
#include <unordered_map>
#include "CHttpConnection.h"
#include "CHttpRequest.h"
#include "CHttpResponse.h"
#include <memory>
#include "CLazy.h"
#include "CServiceRouter.h"
#include <functional>
#include "../semain/semain.h"
#include "CHttpContext.h"


using namespace std;
using namespace std::placeholders;

class CRPCServiceController;

typedef unordered_map<string, CRPCServiceController*> ControllerMap;

class BaseFunction
{
    
};

template<typename... T>
class Function : public BaseFunction 
{
public:
    typedef function<nxweb_result(Json::Value& response, T...)> FuncType;
    FuncType func;
    
    Function()
    {
        
    }
    
    // Construct from a std::function
    Function(FuncType f) : func(f)
    {
    }

    // Assignment from a std::function to this type.
    void operator=(const FuncType& f)
    {
        this->func = f;
    }
    
    // () overload to pretend like a function)
    nxweb_result operator()(Json::Value& response, T... args)
    {
        if (func)
            return func(response, args...);
        
        return NXWEB_ERROR;
    }
    
};

typedef unordered_map<string, std::shared_ptr<BaseFunction> > ActionMap;


class CRPCServiceController 
{
protected:
    static std::mutex _lock;
    ActionMap actions;
    static ControllerMap cachedControllers;
    
public:
    CRPCServiceController();
    CRPCServiceController(const CRPCServiceController& jc) = delete;
    ~CRPCServiceController();

protected:
    
    template<typename T>
    void OkResponse(Json::Value& output, T result)
    {
        output["result"] = result;
    }
    
    void SECodeResponse(Json::Value& output, SE_CODE code, Json::Value data = Json::nullValue);

    void OkAppOperationResponse(Json::Value& response, SE_CODE errorCode = E_IncorrectOperation, Json::Value data = Json::nullValue, CSTR message = nullptr);
    
    void OkResponse(Json::Value& output);

    void OkResponse(Json::Value& output, const Json::Value& result);
    
    void InvalidJsonResponse(Json::Value& output);
    
    bool CheckJsonRpcRequestMessage(const Json::Value& request);
    
    nxweb_result ProcessAction(const Json::Value& request, Json::Value& response);
    
    void MethodNotFoundResponse(string methodName, Json::Value& response);

    bool IsMethodIdCorrect(const Json::Value& response, const string& id);
    
    void MethodIdIncorrect(Json::Value& response);
    
    void InsufficientParameters(Json::Value& output);
    
    void InvalidAppOperationResponse(Json::Value& response, SE_CODE errorCode = E_IncorrectOperation);

    void InvalidAppOperationResponse(Json::Value& response, const string& errorMessage);
    
    void ClearSession(Json::Value& response);
    
    RP<ISessionInterface> OpenOrCreateSession(const string& sessionId);
    RP<ISessionInterface> GetSession(const Json::Value& response);
    
public:

    virtual nxweb_result GenerateCacheKey
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );


    virtual nxweb_result OnSelect
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );

    virtual nxweb_result OnHeaders
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );

    virtual nxweb_result OnPostData
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );

    virtual nxweb_result OnPostDataComplete
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );


    virtual nxweb_result OnComplete
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );

    virtual nxweb_result OnError
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );

    virtual nxweb_result OnRequest
    (
        CHttpConnection& conn, 
        CHttpRequest& req,
        CHttpResponse& rsp
    );
    
    virtual NXR OnRequestResponse(CHttpContext& context);

    static void Register();

    static void UnRegister();
    

    bool TryController(const string& url, CRPCServiceController*& controller);

    template<typename T>
    void AddAction(const string& actionName, const T& action)
    {
        actions.insert(pair<string, std::shared_ptr<BaseFunction>>(actionName, std::shared_ptr<BaseFunction>(new T(action))));
    }
    
    template<typename... TArgs>
    nxweb_result CallAction(const string& actionName, Json::Value& response, const TArgs&... args)
    {
        typedef Function<TArgs...> actionType;
        ActionMap::const_iterator it = actions.find(actionName);
        if ( it != actions.end())
        {
            actionType* action = (actionType*)it->second.get();
            if (action)
                return (*action)(response, args...);
        }
        MethodNotFoundResponse(actionName, response);
        return NXWEB_OK;
    }
};


#endif /* CJSONCALLS_H */

