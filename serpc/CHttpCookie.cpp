/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpCookie.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:34 AM
 */

#include "CHttpCookie.h"

CHttpCookies::CHttpCookies(nxweb_http_cookie* cookies) : CThinMap(cookies)
{
}

CHttpCookies::~CHttpCookies()
{
}

