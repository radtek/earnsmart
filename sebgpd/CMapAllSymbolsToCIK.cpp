/* 
 * File:   CMapAllSymbolsToCIK.cpp
 * Author: santony
 * 
 * Created on October 1, 2012, 9:36 PM
 */

#include "IFunctions.h"
#include "../secommon.h"
#include "CMapAllSymbolsToCIK.h"


//using namespace log4cxx;



CMapAllSymbolsToCIK::CMapAllSymbolsToCIK(ISession* ps) : 
     CTimedFunction()
{
    _pSession = ps;
}

CMapAllSymbolsToCIK::~CMapAllSymbolsToCIK() 
{
}

void CMapAllSymbolsToCIK::run()
{
    BoolCompletion busy(_bIsBusy);
    RP<IXbrl> pXbrl = (IXbrl*) this->_pSession->get_Object(IXbrlName)._ptr;
    ErrorId err = pXbrl->UpdateCIKs(0);
}
