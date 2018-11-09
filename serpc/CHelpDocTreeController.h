/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTreeController.h
 * Author: santony
 *
 * Created on April 27, 2016, 8:26 AM
 */

#ifndef CHELPDOCTREECONTROLLER_H
#define CHELPDOCTREECONTROLLER_H

#include "CRPCServiceController.h"


class CHelpDocTreeController  : public CRPCServiceController
{
public:
    CHelpDocTreeController();
    CHelpDocTreeController(const CHelpDocTreeController& orig) = delete;
    virtual ~CHelpDocTreeController();
private:
    
    NXR GetDocTree(Json::Value& response, string docId, string subLevels);
    NXR SaveDocItem(Json::Value& response, const Json::Value& docinfo);
    NXR DelDocTree(Json::Value& response, string docId, string agreedToDelChildren);
};

#endif /* CHELPDOCTREECONTROLLER_H */

