/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpHeaders.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:36 AM
 */

#ifndef CHTTPHEADERS_H
#define CHTTPHEADERS_H

#include "CHttpConnection.h"
#include "CHttpCookie.h"
#include "CHttpParameters.h"
#include "CMapIterator.h"



class CHttpHeaders : public CThinMap
{
private:
    
public:
    CHttpHeaders(nxweb_http_header* rawHeader);
    CHttpHeaders(const CHttpHeaders& orig) = delete;
    virtual ~CHttpHeaders();
    
    
    
private:

};

#endif /* CHTTPHEADERS_H */

