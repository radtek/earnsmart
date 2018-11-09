/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPositions.cpp
 * Author: santony
 * 
 * Created on September 28, 2016, 8:40 PM
 */

#include "internal.h"
#include "CPositions.h"

CPositions::CPositions() 
{  
}

CPositions::CPositions(ISession* ps) : CJsonObject<se::admin::IPositions>(ps)
{
    
}

CPositions::~CPositions() 
{
    
}

long unsigned int CPositions::Init(Json::Value& rootData) 
{
    return CJsonObject<se::admin::IPositions>::Init(rootData);
}


