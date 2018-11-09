/* 
 * File:   CSecurityPriceProcessor.cpp
 * Author: santony
 * 
 * Created on October 19, 2013, 6:58 PM
 */

#include "CSecurityPriceProcessor.h"
#include "../sesecurities/sesecuritiesapi.h"
#include "../semysql/CSEConnections.h"

using namespace se;
using namespace se::data;
using namespace se::securities;

#define SEDBCONN        CSEConnections::GetMain()

CSecurityPriceProcessor::CSecurityPriceProcessor(ISession* ps, int waitpersymbol) : 
        CTimedFunction(),
        //_logger(Logger::getLogger("sebgpd.securitypriceprocessor")),
        _waitpersymbol(waitpersymbol)
{
    _pSession=ps;
}

CSecurityPriceProcessor::~CSecurityPriceProcessor() 
{
}

void CSecurityPriceProcessor::run()
{
    BoolCompletion brun(_bIsBusy);
    
   // LOGIP("Beginning to run security pricing processor.");
    
    try
    {

        ErrorId ret;
        SP<ISecurityAPI> pApi;
        ret = SecOpen(_pSession, _pSession->get_Configs()->get_DefaultMarketDataProvider(), &pApi);
        if (FAILED(ret))
        {
           // LOGEP("Failed to open security api.");
            return;
        }
        ret = pApi->ProcessSecDailyPrice(_waitpersymbol);
        if (FAILED(ret))
        {
           // LOGEP("Failed to process daily or historical pricess. Please check logs and implementation// LOGic.");
            return;
        }
    }
    catch (std::exception& ex)
    {
       // LOGE(_logger, "Security pricing processor failed to run with an error message " << ex.what());
    }
}

