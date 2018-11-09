/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpContext.h
 * Author: santony
 *
 * Created on April 9, 2016, 12:46 PM
 */

#ifndef CHTTPCONTEXT_H
#define CHTTPCONTEXT_H

#include "CHttpConnection.h"
#include "CHttpRequest.h"
#include "CHttpResponse.h"


class CHttpContext 
{
private:
    CHttpConnection& _httpConn;
    CHttpRequest& _httpReq;
    CHttpResponse& _httpResp;
    
public:
    CHttpContext(CHttpConnection& connection, CHttpRequest request, CHttpResponse response);
    CHttpContext(const CHttpContext& orig);
    ~CHttpContext();

    CHttpConnection& Connection();
    CHttpRequest& Request();
    CHttpResponse& Response();

    string HostName();
    
};

#endif /* CHTTPCONTEXT_H */

