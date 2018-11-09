/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEarnsmartController.h
 * Author: santony
 *
 * Created on March 8, 2016, 6:41 PM
 */

#ifndef CEARNSMARTCONTROLLER_H
#define CEARNSMARTCONTROLLER_H

#include "CRPCServiceController.h"

class CEarnsmartController : public CRPCServiceController
{
public:
    CEarnsmartController();
    CEarnsmartController(const CEarnsmartController& orig) = delete;
    virtual ~CEarnsmartController();
    
private:
  
    NXR GetAppData(Json::Value& response);
    
};

#endif /* CEARNSMARTCONTROLLER_H */

