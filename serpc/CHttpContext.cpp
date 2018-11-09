/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpContext.cpp
 * Author: santony
 * 
 * Created on April 9, 2016, 12:46 PM
 */

#include "CHttpContext.h"

CHttpContext::CHttpContext(CHttpConnection& connection, CHttpRequest request, CHttpResponse response) :
    _httpConn(connection), _httpReq(request), _httpResp(response)
{
    
}

CHttpContext::CHttpContext(const CHttpContext& orig) : 
    _httpConn(orig._httpConn), _httpReq(orig._httpReq), _httpResp(orig._httpResp)
{
    
}

CHttpContext::~CHttpContext() 
{
    
}

CHttpConnection& CHttpContext::Connection() 
{
    return _httpConn;
}

CHttpRequest& CHttpContext::Request() 
{
    return _httpReq;
}

CHttpResponse& CHttpContext::Response() 
{
    return _httpResp;
}

string CHttpContext::HostName() 
{
    string s ;
    if (_httpConn.IsSecure())
        s = "https://";
    else
        s = "http://";
    s += _httpReq.Host() + (string)"/";
    
    return s;
}



