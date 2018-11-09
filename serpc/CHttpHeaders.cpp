/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpHeaders.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:36 AM
 */

#include "CHttpHeaders.h"

CHttpHeaders::CHttpHeaders(nxweb_http_header* rawHeader) : CThinMap(rawHeader)
{
    
}


CHttpHeaders::~CHttpHeaders()
{
    
}

