/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEconomicsController.h
 * Author: santony
 *
 * Created on December 17, 2016, 7:02 AM
 */

#ifndef CECONOMICSCONTROLLER_H
#define CECONOMICSCONTROLLER_H

#include "CRPCServiceController.h"

class CEconomicsController : public CRPCServiceController
{
public:
    CEconomicsController();
    CEconomicsController(const CEconomicsController& orig) = delete;
    virtual ~CEconomicsController();
private:
    
    NXR GetForcastMatrix(Json::Value& rsp, const Json::Value& req);
    

};

#endif /* CECONOMICSCONTROLLER_H */

