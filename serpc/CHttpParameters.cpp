/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpParameters.cpp
 * Author: santony
 * 
 * Created on March 5, 2016, 12:14 PM
 */

#include "CHttpParameters.h"

CHttpParameters::CHttpParameters(nxweb_http_parameter* parameters) : CThinMap(parameters)
{
    
}

CHttpParameters::~CHttpParameters() {
}

