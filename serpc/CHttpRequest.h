/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpRequest.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:17 AM
 */

#ifndef CHTTPREQUEST_H
#define CHTTPREQUEST_H

#include "CRequest.h"
#include "CHttpHeaders.h"
#include "CHttpCookie.h"
#include "CHttpParameters.h"

typedef nxweb_http_request RawHttpRequest;

class CHttpRequest //: public CRequest
{
private:
    RawHttpRequest* _rawReq;
    CHttpHeaders _headers;
    CHttpCookies _cookies;
    CHttpParameters _parameters;
    
public:
    CHttpRequest(RawHttpRequest* rawReq);
    CHttpRequest(const CHttpRequest& orig);
    ~CHttpRequest();
    
    CSTR Method() const;
    CSTR Uri() const;
    CSTR HttpVersion() const;
    CSTR Host() const;
    CSTR Cookie() const;
    CSTR UserAgent() const;
    CSTR ContentType() const;
    CSTR Content() const;
    SIZE ContentLength() const;
    SIZE ContentReceived() const;
    CSTR TransferEncoding() const;
    CSTR AcceptEncoding() const;
    CSTR Range() const;
    CSTR PathInfo() const;
    const CHttpHeaders& Headers() ;
    const CHttpCookies& Cookies() ;
    const CHttpParameters& Parameters() ;
    CSTR SessionCookie();
    
    RawHttpRequest* RawData() const;
    
private:

};

#endif /* CHTTPREQUEST_H */

