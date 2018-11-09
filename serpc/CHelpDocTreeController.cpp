/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTreeController.cpp
 * Author: santony
 * 
 * Created on April 27, 2016, 8:26 AM
 */

#include "internal.h"
#include "CHelpDocTreeController.h"

using namespace se;
using namespace se::admin;

CHelpDocTreeController::CHelpDocTreeController()
{
    this->AddAction("getdoctree", Function<string, string>(bind(&CHelpDocTreeController::GetDocTree, this, _1, _2, _3)));
    this->AddAction("savedocitem", Function<const Json::Value&>(bind(&CHelpDocTreeController::SaveDocItem, this, _1, _2)));
    this->AddAction("deldocitem", Function<string, string>(bind(&CHelpDocTreeController::DelDocTree, this, _1, _2, _3)));
}

CHelpDocTreeController::~CHelpDocTreeController()
{
}

NXR CHelpDocTreeController::DelDocTree(Json::Value& response, string docId, string agreedToDelChildren) 
{
    if (!IsMethodIdCorrect(response, "802"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    
    RP<IHelpDocTree> p = GetSession(response)->get_AdminApi()->get_HelpDocTree();
    if (p)
    {
        auto r = p->del_Doc(strtol(docId.c_str(), nullptr, 10), agreedToDelChildren=="false" ? false : true );
        if (FAILED(r))
        {
            InvalidAppOperationResponse(response, r);
        }
        else
        {
            OkResponse(response);
        }
    }
    else
    {
        InvalidAppOperationResponse(response, "Failed to fetch help tree data pointer.");
    }
    return N_OK;
}


NXR CHelpDocTreeController::GetDocTree(Json::Value& response, string docId, string subLevels)
{
    
    if (!IsMethodIdCorrect(response, "800"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    
    RP<IHelpDocTree> p = GetSession(response)->get_AdminApi()->get_HelpDocTree();
    if (p)
    {
        Json::Value result;
        SE_CODE retCode ;
        if (docId == "0")
            p->get_RootDoc(result, strtol(subLevels.c_str(), nullptr, 10));
        else
            p->get_Doc((UID)strtol(docId.c_str(), nullptr, 10), result, strtol(subLevels.c_str(), nullptr, 10));
        
        if (FAILED(retCode))
        {
            InvalidAppOperationResponse(response, retCode);
        }
        else
        {
            BERR << result;
            
            OkResponse(response, result);
        }
    }
    else
    {
        InvalidAppOperationResponse(response, "Failed to fetch help tree data pointer.");
    }
    return N_OK;
}

NXR CHelpDocTreeController::SaveDocItem(Json::Value& response, const Json::Value& docinfo)
{
    if (!IsMethodIdCorrect(response, "801"))
    {
        MethodIdIncorrect(response);
        return N_OK;
    }
    
    Json::Value data = docinfo;
    RP<IHelpDocTree> p = GetSession(response)->get_AdminApi()->get_HelpDocTree();
    if (p)
    {
        BDBG << docinfo ;
        auto retCode = p->set_Doc(data); 
        if (FAILED(retCode))
        {
            InvalidAppOperationResponse(response, retCode);
        }
        else
        {
            Json::Value newId ;
            newId["docid"] = data["docid"];
            BDBG << newId;
            OkResponse(response, newId);
        }
    }
    else
    {
        InvalidAppOperationResponse(response, "Failed to fetch help tree data pointer.");
    }
    return N_OK;
}
