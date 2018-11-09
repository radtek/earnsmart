/* 
 * File:   CFunctionHandler.cpp
 * Author: santony
 * 
 * Created on October 1, 2012, 8:49 PM
 */

#include "../secommon.h"
#include "CFunctionHandler.h"
#include "IFunctions.h"
#include "CMapAllSymbolsToCIK.h"
#include "CParseFormIndexFunction.h"
#include "CPriceHistoryProcessor.h"
#include "CProcessStatements.h"

CFunctionHandler::CFunctionHandler(CSTR funcName, CSTR funcArg) : 
    _funcName(funcName), 
        _funcArg(funcArg?funcArg:""),
        _bExecInSync(false),
        pFunc(NULL)
{
    
}

CFunctionHandler::CFunctionHandler(const CFunctionHandler& orig) 
{
    
}

CFunctionHandler::~CFunctionHandler() 
{
    delete pFunc;
}

void CFunctionHandler::Interrupt()
{
    if (pFunc)
        pFunc->Interrupt();
}

ErrorId CFunctionHandler::Execute()
{
    // disable next timer calls
    ErrorId retCode = E_UnknownError;
    if (!_bExecInSync) 
    {
        _bExecInSync = true;
        
        if (!pFunc)
        {
            if (_funcName == "mapsymboltocik")
            {
                //pFunc = new CMapAllSymbolsToCIK();
            }
            else if (_funcName == "processstatements")
            {
                //pFunc = new CProcessStatements();
            }
            else if (_funcName == "parseformindex")
            {
                //pFunc = new CParseFormIndexFunction(_funcArg.c_str());
            }
            else if (_funcName == "pricehistory")
            {
                //pFunc = new CPriceHistoryProcessor();
            }
        }
        if (pFunc)
        {
            retCode = pFunc->Execute();
        }
        _bExecInSync = false;
    }
    return retCode;
}


/////////////////////////////////////////////CTimedFunction////////////////////////////////////
CTimedFunction::CTimedFunction() : _bIsBusy(false), _intervalFreq(FREQ_Weekly)
{
    _clk = 0;
}

CTimedFunction::~CTimedFunction()
{
}

void CTimedFunction::SetInterval(const Value& config) 
{
    _config = config;
    const Value& interval = _config["interval"];
    if (interval["freq"].asString() == "daily")
        _intervalFreq = FREQ_Daily;
    else if (interval["freq"].asString() == "continuous")
        _intervalFreq  = FREQ_Continuous;
    else if (interval["freq"].asString() == "weekly")
        _intervalFreq = FREQ_Weekly;
    else if (interval["freq"].asString() == "monthly")
        _intervalFreq = FREQ_Monthly;
    else if (interval["freq"].asString() == "once")
        _intervalFreq = FREQ_Once;
}

bool CTimedFunction::HasExpired()     
{
    const Value& interval = _config["interval"];
    if (_intervalFreq == FREQ_Continuous)
    {
        double dFactor = atof(interval["factor"].asString().c_str());
        if (difftime(time(0), _clk) >= dFactor)
        {
            _clk = time(0);
            return true;
        }
    }
    else if (_intervalFreq == FREQ_Daily)
    {
        // check time.
        // parse to a time
        SP<IString> dtrun;
        CDate::Now().ToString(&dtrun);
        dtrun->Append((string(" " + interval["factor"].asString())).c_str());
        ptime timetorun(time_from_string(dtrun->get_Buffer()));
        ptime timenow(second_clock::local_time());
        if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
        {
            return true;
        }
    }
    else if (_intervalFreq == FREQ_Weekly)
    {
        SP<IString> today ;
        CDate dt = CDate::Now();
        dt.AsDayString(&today);
        //"factor" : "00:02:00",
        //"days" : "Saturday, Sunday"
        string days = interval["days"].asString();
        char_separator<char> sep(",","" , keep_empty_tokens);
        typedef tokenizer< char_separator<char> > Tokenizer;
        typedef vector<string> TokenVector;
        TokenVector vec;
        trim(days);
        // tokenize the days
        Tokenizer tok(days, sep);
        vec.assign(tok.begin(), tok.end());
        for (string s : vec)
        {
            trim(s);
            if (s == today->get_Buffer())
            {
                SP<IString> dtrun;
                dt.ToString(&dtrun);                    
                dtrun->Append((string(" " + interval["factor"].asString())).c_str());
                ptime timetorun(time_from_string(dtrun->get_Buffer()));
                ptime timenow(second_clock::local_time());
                if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
                {
                    SetBusy();
                    return true;
                }
            }
        }
    }
    else if (_intervalFreq == FREQ_Monthly)
    {
        // when could one of these values
        // first, last, mid, or a number for day (1, 2, ... 31)
        string when = interval["when"].asString();
        string stimetorun = interval["time"].asString();

        CDate dtNow = CDate::Now();

        SP<IString> dtrun;
        dtNow.ToString(&dtrun);
        dtrun->Append(" ");
        dtrun->Append(stimetorun.c_str());
        ptime timetorun(time_from_string(dtrun->get_Buffer()));

        if (when == "first")
        {
            if (dtNow.get_Day() == 1)
            {
                ptime timenow(second_clock::local_time());
                if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
                {
                    return true;
                }
            }
        }
        else if (when == "last")
        {
            if (dtNow.get_Day() == dtNow.get_LastDayOfMonth())
            {
                ptime timenow(second_clock::local_time());
                if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
                {
                    return true;
                }
            }
        }
        else if (when == "mid")
        {
            if (dtNow.get_Day() == dtNow.get_MidDayOfMonth())
            {
                ptime timenow(second_clock::local_time());
                if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
                {
                    return true;
                }
            }
        }
        else 
        {
            // try getting a number
            try
            {
                int day = strtol(when.c_str(), nullptr, 10);
                if (dtNow.get_Day() == day)
                {
                    ptime timenow(second_clock::local_time());
                    if (timenow >= timetorun && timenow < (timetorun + minutes(MINUTERANGE)))
                    {
                        return true;
                    }
                }
            }
            catch (...)
            {
                // 
                return false;
            }
        }
    }
    return false;
}

bool CTimedFunction::IsBusy() 
{
    return _bIsBusy;
}

void CTimedFunction::SetBusy() 
{
    _bIsBusy = true;
}

void CTimedFunction::UnsetBusy() 
{
    _bIsBusy = false;
}

FREQ CTimedFunction::GetFreq() 
{
    return _intervalFreq;
}
