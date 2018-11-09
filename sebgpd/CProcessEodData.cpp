/* 
 * File:   CProcessEodData.cpp
 * Author: santony
 * 
 * Created on September 30, 2015, 1:15 PM
 */

#include "CProcessEodData.h"
#include "../seglobal/exceptions.h"
#include <sstream>
#include "../seglobal/globallogger.h"


CProcessEodDataQuotes::CProcessEodDataQuotes(ISession* ps, const string& exchanges)  : CTimedFunction()
{
    _pSession = ps;
    CSTR tok = 0;
    char* sTypes = const_cast<char*>(exchanges.c_str());
    tok = strtok(sTypes, ",");
    while (tok != 0)
    {
        _exchanges.push_back(tok);
        tok = strtok(0, ",");
    }
}

CProcessEodDataQuotes::~CProcessEodDataQuotes() 
{
    
}

void CProcessEodDataQuotes::run() 
{
    BoolCompletion busy(_bIsBusy);
    try
    {
        
        if (!pEodData)
        {
            pEodData.Assign((IEODData*)_pSession->get_Object(IEODDataName)._ptr);
        }
        SE_CODE rCode;
        
        // authenticate and open session
        SP<IWSClientSession> psession;
        THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
        rCode = psession->PublishCommonData();
        if (FAILED(rCode))
        {
            BERR << "Failed to publish common eod data. Check logs for details.";
            return ;
        }
        
        psession.Clear();
        
        for (string exch : _exchanges)
        {
            std::transform(exch.begin(), exch.end(), exch.begin(), ::tolower);
            // authenticate and open session
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            BINF << "Attemping to publish data for exchange " << exch;
            
            rCode = psession->UploadSymbols(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish symbols for exchange " << exch << ". Check logs for details.";

            psession.Clear();
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UpdateFundamentals(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish fundamentals for exchange " << exch << ". Check logs for details.";

            psession.Clear();
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UpdateTechnicals(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish technicals for exchange " << exch << ". Check logs for details.";
            psession.Clear();

            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UploadExchangeQuotesCurrent(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish technicals for exchange " << exch << ". Check logs for details.";
            psession.Clear();
            
        }
        
    }
    catch(const std::exception& ex)
    {
        BERR << "run method failed with error: " << ex.what();
        return;
    }

    BINF << "Successfully completed processing EODData.";
    
    return;
}



CProcessEodDataHistory::CProcessEodDataHistory(ISession* ps, const string& exchanges)  : CTimedFunction()
{
    _pSession = ps;
    CSTR tok = 0;
    char* sTypes = const_cast<char*>(exchanges.c_str());
    tok = strtok(sTypes, ",");
    while (tok != 0)
    {
        _exchanges.push_back(tok);
        tok = strtok(0, ",");
    }
}

CProcessEodDataHistory::~CProcessEodDataHistory() 
{
    
}

void CProcessEodDataHistory::run() 
{
    BoolCompletion busy(_bIsBusy);
    try
    {
        
        if (!pEodData)
        {
            pEodData.Assign( (IEODData*)_pSession->get_Object(IEODDataName)._ptr );
        }
        SE_CODE rCode;
        
        // authenticate and open session
        SP<IWSClientSession> psession;
        THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
        rCode = psession->PublishCommonData();
        if (FAILED(rCode))
        {
            BERR << "Failed to publish common eod data. Check logs for details.";
            return ;
        }
        
        psession.Clear();
        
        for (string exch : _exchanges)
        {
            std::transform(exch.begin(), exch.end(), exch.begin(), ::tolower);
            // authenticate and open session
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            BINF << "Attemping to publish data for exchange " << exch;
            
            rCode = psession->UploadSymbols(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish symbols for exchange " << exch << ". Check logs for details.";

            psession.Clear();
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UpdateFundamentals(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish fundamentals for exchange " << exch << ". Check logs for details.";

            psession.Clear();
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UpdateTechnicals(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish techinicals for exchange " << exch << ". Check logs for details.";
            psession.Clear();
            
            THROWIFFAILED( pEodData->OpenDefault(&psession), "Failed to create session with eoddata. Please check config." );
            rCode = psession->UploadSymbolsEODHistoryByExchange(exch);
            if (FAILED(rCode))
                BERR << "Failed to publish UploadSymbolsEODHistory";
        }

        
    }
    catch(const std::exception& ex)
    {
        BERR << "run method failed with error: " << ex.what();
        return;
    }

    BINF << "Successfully completed processing EODData.";
    
    return;
}
