/* 
 * File:   CSecurityCharts.cpp
 * Author: santony
 * 
 * Created on October 20, 2015, 10:27 PM
 */

#include "CSecurityCharts.h"

CSecurityCharts::CSecurityCharts() 
{
}

CSecurityCharts::~CSecurityCharts() 
{
}

void CSecurityCharts::set_Title(const string& title)
{
    CMaster::set_Title(CMaster::get_Title() + " | Investment Products | <SYMBOL:EXCHANGE> | Charts");
}


