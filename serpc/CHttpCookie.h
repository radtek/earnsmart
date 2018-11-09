/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpCookie.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:34 AM
 */

#ifndef CHTTPCOOKIE_H
#define CHTTPCOOKIE_H

#include "CMapIterator.h"


class CHttpCookies : public CThinMap
{
public:
    CHttpCookies(nxweb_http_cookie* cookies);
    CHttpCookies(const CHttpCookies& orig) = delete;
    virtual ~CHttpCookies();
    
private:

};

#endif /* CHTTPCOOKIE_H */

