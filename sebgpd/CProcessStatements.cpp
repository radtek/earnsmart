/* 
 * File:   CProcessStatements.cpp
 * Author: aashok
 * 
 * Created on June 29, 2013, 7:04 AM
 */

#include "CProcessStatements.h"
#include "../seglobal/globallogger.h"

// -f processstatements

CProcessStatements::CProcessStatements(ISession* ps) : CTimedFunction()
{
    _pSession = ps;
}

CProcessStatements::~CProcessStatements() 
{
}

void CProcessStatements::run() 
{
    BoolCompletion busy(_bIsBusy);
    try
    {
        ErrorId ret ;
        RP<IXbrl> _pxbrl = (IXbrl*)_pSession->get_Object(IXbrlName)._ptr;
        ret = _pxbrl->ProcessStatements();
        //ret = _pxbrl->ProcessStatement(187899);
        if (E_NotFound == ret)
        {
            BERR << "No records in formrecs table to process. ";
            return;
        }
        if (ret != S_Success)
            BINF << "Error when invoking process statements method. Return code : " << ret;
        
        BINF << "Statement processing is completed by sebgpd thread.";
        
        return;
    }
    catch(std::exception& ex)
    {
        BINF << "Unhandled exception. Error : " << ex.what();
        return;
    }
}
