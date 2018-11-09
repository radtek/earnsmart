/* 
 * File:   CFinancialsConsolidated.cpp
 * Author: santony
 * 
 * Created on January 27, 2014, 7:08 PM
 */

#include <boost/date_time/gregorian/greg_year.hpp>
#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#include "sexbrlinternal.h"
#include "CFinancialsConsolidated.h"
#include "../semain/semain.h"
#include "../seglobal/exceptions.h"
#include "../semysql/CSEConnections.h"
#include "CFinancialItem.h"
#include "../semysql/CMyqlUtilities.h"
#include "../seglobal/CQuarter.h"
#include "CFinancialPerformance.h"
#include "CFinancialRatio.h"
#include "CXbrl.h"
#include "CCaptureFiscalPeriods.h"

using namespace se::threading;
using namespace se::data;

#define SEDBCONN        se::data::CSEConnections::GetMain()

CFinancialsConsolidated::CFinancialsConsolidated(ISession* ps) : CObjectRoot(ps) ,
        _lowestYear(2999), _highestYear(0) //, _lastselectedyear(0)
{
    
}

CFinancialsConsolidated::~CFinancialsConsolidated() 
{
    _l.clear();
    _m.clear();
}

long unsigned int CFinancialsConsolidated::Init(CIK cik, bool fillEmptyValues) 
{
    /*
     * Idea:
     * 
     * A. Need a new CFinItemNode. This is comparable by (xbrlid, segmentid and dimesionid)
     * B. Keep a "set" of CFinItemNode using the comparable function with three keys mentioned above.
     * 
     * 1. Using CIK as key, get all the rec_ids from se_edgar_formrecs table (for 10-Q and 10-k done separately).
     * 2. Loop through each rec id from 1 and get a list of statements
     * 3. Loop through each statement_id and get a list of presentation records
     * 4. Create a boost structure to add edges from presentation table.
     * 5. Keep on adding edges without thinking about statement ids. Each edge 
     *          is a fully populated xbrl data node with value (CFinItemNode).
     * 6. As more and more edges are added, search for CFinItemNode in the 'set' defined in 'B' above
     *          to add financial year/monetary-num-str value.
     * 7. Now we will save the boost graph in the class.
     * 8. All external operations will to the graph data.
     * 
     * C. When getting financial values, since most companies doesn't provide last quarter, we need
     *          a method to substract 3rd quarter value from 10-K value at the year end.
     * D. An async operation (use our threadpool class) will send the data to the database for the 'CIK/PERIOD'
     */

    try
    {
        _cik = cik;
        
        THROWIFFAILED(CObject<CFinancialPeriod>::Create(get_Session(), &_fper), "Failed to construct financial period type");

        THROWIFFAILED(XbrlOpen(get_Session(), &pXapi), "Failed to open xbrl api handler.");

        THROWIFFAILED(pXapi->GetLatestFilingDates(
                _cik.c_str(), 
                &_latestAnnualFilingDate, 
                &_latestQuarterlyFilingDate), "Failed to get latest filings dates.");
        
       
        
        THROWIFFAILED(pXapi->GetFinancialYearEnd(cik, &_finyr), "Financial year end is not available.");
        
        SP<IXbrlDocTypeList> pDocTypes;
        THROWIFFAILED(pXapi->GetDocumentTypes(&pDocTypes), "Failed to get list of document types.");
        for (auto it = pDocTypes->begin(); it != pDocTypes->end(); ++it)
        {
            IXbrlDocType* dt = *it;
            if (dt->get_PeriodType() == PERIOD_Annually)
                docyearly.insert((int)dt->get_Id());
            else if (dt->get_PeriodType() == PERIOD_Quarterly)
                docquarterly.insert((int)dt->get_Id());
        }
        
        RETURNIFFAILED(CCaptureFiscalPeriods::Capture(cik, &_fper));
    
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "call list_Financials(" << cik << ")";
        try
        {
            HandleData(conn, q.store());
            while (q.more_results())
            {
                q.store_next();
            }
        }
        catch(...)      
        {                           
            while (q.more_results())
                q.store_next();
            throw ;
        }

        SetFinancialDates(conn);
        Reconcile(fillEmptyValues);

        return S_OK;
    }
    catch(const seexception& ex)      
    {                           
        string fname = "Init";
       // LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());      
        return ex.error();      
    }                           
    catch(const std::exception& ex)   
    {                           
        string fname = "Init";
       // LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());      
        return E_UnknownError;  
    }                           
    catch(...)                  
    {                           
        string fname = "Init";
       // LOGE(_logger, "(" << fname << ") : Unknown error reported.");                          
        return E_UnknownError;  
    }                           
    
}

CIK CFinancialsConsolidated::get_Cik() 
{
    return _cik.c_str();
}

const CDate& CFinancialsConsolidated::get_LatestAnnualFilingDate()
{
    return _latestAnnualFilingDate;
}

const CDate& CFinancialsConsolidated::get_LatestQuarterlyFilingDate()
{
    return _latestQuarterlyFilingDate;
}

void CFinancialsConsolidated::SetFinancialDates(CSmartConnection& conn)
{
    Query q = conn->query();
    q << "call get_FinYearEndCompleted(" << _cik << ")";
    StoreQueryResult sqr = q.store();
    if (sqr.size())
    {
        sql_date dt = sqr[0][0];
        _finyr.set_Day(dt.day());
        _finyr.set_Month(dt.month());        
        _finyr.set_Year(dt.year());
        while (q.more_results())
            q.store_next();
    }
    else
    {
        q = conn->query();
        q << "call get_FinYearEnd(" << _cik << ")";
        StoreQueryResult sqr = q.store();
        string fye = (string)sqr[0][0];
        // split string
        string fym = fye.substr(0, 2);
        string fyd = fye.substr(2);
        _finyr.set_Month(strtol(fym.c_str(), nullptr, 10));
        _finyr.set_Day(strtol(fyd.c_str(), nullptr, 10));
        CDate tmp;
        if (_latestAnnualFilingDate.IsEmpty() || _latestAnnualFilingDate.IsInvalid())
        {
            tmp.set_Day(_finyr.get_Day());
            tmp.set_Month(_finyr.get_Month());
            tmp.set_Year(_latestQuarterlyFilingDate.get_Year());
            if (tmp > _latestQuarterlyFilingDate)
                tmp = tmp.AddSubYears(-1);
            else
                tmp = tmp.AddSubYears(1);
        }
        else
        {
                _finyr.set_Year(_latestAnnualFilingDate.get_Year());
                if (_latestAnnualFilingDate < _finyr)
                {
                    tmp = _finyr.AddSubYears(-1);
                }
                else
                {
                    int numdays = abs((_finyr - _latestAnnualFilingDate).Day);
                    tmp = _latestAnnualFilingDate;
                    tmp = tmp.AddSubDays(0-numdays);
                }
        }
        _finyr.set_Year(tmp.get_Year());
        while (q.more_results())
            q.store_next();
    }    
}

void CFinancialsConsolidated::HandleData(CSmartConnection& conn, const StoreQueryResult& qr) 
{
    
    // Fields and indices.
    // form_type_id (0), xbrl_id(1), segment_id(2), contexttype(3), fye_month(4), fye_day(5), 
    // startdate(6), enddate(7), xbrl_name(8), monetaryvalue(9), numericvalue(10), stringvalue(11), 
    // xbrlvaluetype(12), segment_name(13), rec_id(14)
    
    //for (const Row& r : qr)
    UID prevrecid = 0;
    for (auto& r : qr)
    {
        CDate startDate, endDate;
        UID xid;
        UID segId = 0;
        I16 formtypeId;
        XPT contextType;
        OmniType ovalue;
        CSTR xbrlName = nullptr;
        int noOfMonths = -1;
        int numOfDays  = -1;
        FinItemNodeType* node = 0;
        FinItemNodeType* segNode = 0;
        UID recId;
        recId = r[14];
        
        try
        {
            
            formtypeId = r[0];
            xid = r[1];
            sql_bigint_null sgid =  r[2];
            if (!sgid.is_null)
                segId = sgid.data;
            
            
            
            contextType = (int)r[3] == 0 ? XPT_Instant : XPT_Duration;

            sql_date_null sd = r[6];
            sql_date_null ed = r[7];
            xbrlName = r[8];
            if (r[12]=="D")
            {
                ovalue = (R8)r[9];
            }
            else if (r[12]=="N")
            {
                ovalue = (I64)r[10];
            }
            else
            {
                ovalue = (CSTR)r[11];
            }
            
            if (sd.is_null)
            {
               // LOGE(_logger, "Ignorign a record without a valid startdate. CIK : " << _cik << " Form type: " << formtypeId << " and xbrlid : " << xid);
                continue;
            }

            CMyqlUtilities::PopulateDateTime(sd.data, startDate);
            CMyqlUtilities::PopulateDateTime(ed.data, endDate);
            
            auto dpdiff = (endDate - startDate);
            noOfMonths = dpdiff.Month;
            numOfDays = dpdiff.Day;
            
            int index;
            bool newlyAdded;
            node = &ManageFinItemNode(xid, index, newlyAdded);
            if (newlyAdded)
            {
                node->XbrlName = xbrlName;
                node->PeriodType = contextType;
            }
            
            if (segId)
            {
                segNode = &ManageFinItemNode(*node, segId, newlyAdded);
                if (newlyAdded)
                {
                    segNode->XbrlName = (CSTR)r[10];
                    segNode->PeriodType = contextType;
                }                    
            }
            
#ifdef __DEBUG__
                if (xid == 20629 && endDate.get_Year() == 2014 && endDate.get_Month() == 4 and endDate.get_Day() == 30)
                {
                    string s = "break";
                }
#endif
            
            
            
        }            
        catch (const std::exception& ex)
        {
           // LOGE(_logger, ex.what());
            continue;
        }
            
        // if num of days are between 85 and 95 or
        // 170 and 190, call quarterly function.
        if (numOfDays >= 70 and numOfDays <= 110) // ~3 months.
        {
            if (SetQuarterlyValue(*node, segId, startDate, endDate, noOfMonths, numOfDays, ovalue, contextType))
            {
                continue;
            }
        }

        if (numOfDays >= 111 && numOfDays <= 199) //~6 months
        {
            if (SetDurationValue(
                    *node, 
                    segId, 
                    FinItemNodeType::FirstHalf, 
                    startDate, 
                    endDate, 
                    noOfMonths, 
                    numOfDays, 
                    ovalue, 
                    contextType))
            {
                continue;
            }
        }

        if (numOfDays >= 200 && numOfDays <= 300) // ~9 months
        {

            if (SetDurationValue(
                    *node, 
                    segId, 
                    FinItemNodeType::FirstThreeQuarters, 
                    startDate, 
                    endDate, 
                    noOfMonths, 
                    numOfDays, 
                    ovalue, 
                    contextType))
            {
                continue;
            }
        }

        if (numOfDays >= 301 && numOfDays <= 370)
        {
            if (SetFullYearValue(*node, segId, startDate, endDate, noOfMonths, numOfDays, ovalue, contextType))
            {
                continue;
            }
        }

        if (docquarterly.find(formtypeId) != docquarterly.end() && (contextType == XPT_Instant || numOfDays <= 95) )
        {
            if (SetQuarterlyValue(*node, segId, startDate, endDate, noOfMonths, numOfDays, ovalue, contextType))
            {
                continue;
            }
        }

        if (docyearly.find(formtypeId) != docyearly.end())    // annual statement
        {
            if (numOfDays == 0)
            {
                if (SetFullYearValue(*node, segId, startDate, endDate, noOfMonths, numOfDays, ovalue, contextType))
                {
                    continue;
                }
            }
        }

        if (prevrecid != recId)
            prevrecid = recId;
    }
    //ProcessExceptionalRows(specialRows);
}

void CFinancialsConsolidated::SetYearRange(const DATAPERIOD& dp) 
{
    _lowestYear = std::min((I16)dp.Year, _lowestYear);
    _highestYear = std::max((I16)dp.Year, _highestYear);
    if (_ceilPer < dp)
        _ceilPer = dp;
}

short CFinancialsConsolidated::get_HighestYear() 
{
    return _highestYear;
}

short CFinancialsConsolidated::get_LowestYear() 
{
    return _lowestYear;
}

short CFinancialsConsolidated::get_LatestQuarter() 
{
    //return _ceilPer.Quarter;
    return this->_fper->get_LatestPeriod().Quarter;
}


void CFinancialsConsolidated::ProcessExceptionalRows(ExRows& rows) 
{
    try
    {
        for (auto& pair : rows)
        {
            
            const Row& r = pair.second;
            
            I16 formtypeId = r[0];
            CDate yearEnd;
            UID xid = r[1];
            yearEnd.set_Month((I16)r[3]);
            yearEnd.set_Day((I16)r[4]);
            sql_date_null sd = r[5];
            sql_date_null ed = r[6];
            CSTR xbrlName = r[7];
            double value = r[8];
            XPT contextType;
            if ((int)r[2] == 0)
                contextType = XPT_Instant;
            else
                contextType = XPT_Duration;
            
            
            CDate startDate, endDate;
            CMyqlUtilities::PopulateDateTime(sd.data, startDate);
            CMyqlUtilities::PopulateDateTime(ed.data, endDate);
            

            if (yearEnd.get_Day() == 0 && yearEnd.get_Month() == 0)
                yearEnd = _finyr;
            yearEnd.set_Year(endDate.get_Year());    //first set endDate's year. then we will do some analysis
            int fy = WhatIsFinEndYear(endDate, yearEnd);     // analysis will see if endDate-finYear no of is around 30 days.
            _finyr.set_Year(fy);
            yearEnd.set_Year(fy);
            if (yearEnd.IsInvalid())
                yearEnd = _finyr;

            
           // LOGI(_logger, "Form Type: " << formtypeId
//                    << " XbrlName: " << xbrlName
//                    << " Start Date: " << startDate << " End Date :" << endDate 
//                    << " and Year end: " << yearEnd 
//                    << " # of months between: " << pair.first 
//                    << " # of months from year end: " << (yearEnd - endDate).Month
//                    << " Value = " << std::setprecision(std::numeric_limits<double>::digits10) << value );
            
            if (pair.first > 6 && pair.first <= 12)
            {
                // set to yearly statements
                int index;
                bool newlyAdded;
                FinItemNodeType& node = ManageFinItemNode(xid, index, newlyAdded);
                if (newlyAdded)
                {
                    node.XbrlName = xbrlName;
                    node.PeriodType = contextType;
                }
                
                if (!node.GetValue(fy).IsEmpty())
                    node.SetValue(fy, value);
            }
            else if (pair.first > 12)
            {
                for (int i = startDate.get_Year() + 1; i <= endDate.get_Year(); ++i)
                {
                    int index;
                    bool newlyAdded;
                    FinItemNodeType& node = ManageFinItemNode(xid, index, newlyAdded);
                    if (newlyAdded)
                    {
                        node.XbrlName = xbrlName;
                        node.PeriodType = contextType;
                    }

                    if (!node.GetValue(i).IsEmpty())
                        node.SetValue(i, value);
                }
            }
            else if (formtypeId == 3 && pair.first == 1)
            {
                int index;
                bool newlyAdded;
                FinItemNodeType& node = ManageFinItemNode(xid, index, newlyAdded);
                if (newlyAdded)
                {
                    node.XbrlName = xbrlName;
                    node.PeriodType = contextType;
                }

                if (!node.GetValue(fy).IsEmpty())
                    node.SetValue(fy, value);
            }
        }
    }
    catch (const std::exception& ex)
    {
       // LOGE(_logger, ex.what());
    }
}


int CFinancialsConsolidated::WhatIsFinEndYear(CDate& endDate, CDate& yearEnd) 
{
    try
    {
        // financial end is positioned in the first quarter of the year
        // then that year is the end year.
        int numOfDays = abs((endDate - yearEnd).Day);
        if (numOfDays <= 30)        // we know that finyear is closer to endd_finyrate
            return endDate.get_Year();

        if (yearEnd.get_Month() == 12)
            return (int)endDate.get_Year();

        if (endDate.get_Month() > yearEnd.get_Month())
            return endDate.get_Year() + 1;
        else
            return (int)endDate.get_Year();
    }
    catch(const std::exception& ex)
    {
        throw ex;
    }

}

bool CFinancialsConsolidated::PeriodMapping(const CDate& endDate, DATAPERIOD* dpOut) 
{
    _fper->FindFiscalPeriod(endDate, dpOut);
    if (dpOut->IsEmpty())
    {
       // LOGEP("date returned is empty.");
#ifdef __DEBUG__
        _fper->FindFiscalPeriod(endDate, dpOut);
#endif
        return false;
    }
    
    return true;
    
}

bool CFinancialsConsolidated::SetFullYearValue(
    FinItemNodeType& node, UID segId, const CDate& /* startDate */, const CDate& endDate,
    int /* numMonths */, int /*numDays */, const OmniType& value, XPT /* contextType */) 
{
    try
    {
        DATAPERIOD dpOut;
        if (!PeriodMapping(endDate, &dpOut))
            return false;
        if (segId > 0)
        {
            bool bIsNew;
            FinItemNodeType& segNode = ManageFinItemNode(node, segId, bIsNew);
            segNode.SetValue(dpOut.Year, value);
            return true;
        }
        node.SetValue(dpOut.Year, value);
        SetYearRange(dpOut);
        return true;
    }
    catch(const std::exception& ex)
    {
       // LOGEP(ex.what());
        throw ex;
    }
}

bool CFinancialsConsolidated::SetDurationValue(
    FinItemNodeType& node, UID segId, FinItemNodeType::Duration duration, const CDate& /* startDate */, const CDate& endDate, 
    int /* numMonths */, int /* numDays */, const OmniType& value, XPT /* contextType */) 
{
    try
    {
        
        DATAPERIOD dpOut;
        if (!PeriodMapping(endDate, &dpOut))
            return false;

        if (segId > 0)
        {
            bool bIsNew;
            FinItemNodeType& segNode = ManageFinItemNode(node, segId, bIsNew);
            segNode.SetValue(dpOut.Year, duration, value);
        }
        else
        {
            node.SetValue(dpOut.Year, duration, value);
        }
                
        SetYearRange(dpOut);
        return true;
        
    }
    catch(const std::exception& ex)
    {
       // LOGEP(ex.what());
        throw ex;
    }
}

bool CFinancialsConsolidated::SetQuarterlyValue(
    FinItemNodeType& node, UID segId, const CDate& startDate, const CDate& endDate, 
    int /* numMonths */, int  /* numDays */, const OmniType& value, XPT /* contextType */) 
{
    DATAPERIOD dpOut;
    if (!PeriodMapping(endDate, &dpOut))
        return false;
    
    try
    {
        if (segId > 0)
        {
            bool bIsNew;
            FinItemNodeType& segNode = ManageFinItemNode(node, segId, bIsNew);
            segNode.SetValue(dpOut.Year, dpOut.Quarter, value);
        }
        else
        {
            node.SetValue(dpOut.Year, dpOut.Quarter, value);
        }        
        SetYearRange(dpOut);
        return !!dpOut.Quarter;
    }
    catch(const std::exception& ex)
    {
       // LOGEP(ex.what());
        throw ex;
    }
}

void CFinancialsConsolidated::Reconcile(bool fillemptyvalues) 
{
    //if (!fillemptyvalues) return;
    
    CDate dttmp = _finyr;
    for (FinItemNodeType& node : _l)
    {
        
        
        int observedType = node.PeriodType;
        short IsTotalingType = -1;  // not sure.    // 0=instant, 1 = period/totaling type.
        int goodquarters = 0;
        int goodyear = 0;
        double qtotal = 0.0;
        double dfyv = 0.0;
        
#ifdef __DEBUG__
        if (node.XbrlId == 20629)
        {
            string s = "break";
        }
#endif
        
        if (node.PeriodType == XPT_Duration)
        {
            // first pass. check for a good year with all quarter values and years
            
            for (int year = node.FloorYear; year <= node.CeilYear; ++year)
            {
                
#ifdef __DEBUG__
                if (node.XbrlId == 20629 && year == 2014)
                {
                    string s = "break";
                }
#endif
                
                const OmniType& fyv = node.GetValue(year);
                if (fyv.dt == DT_CSTR)
                {
                    goto END;
                }

                goodyear = 0;
                goodquarters = 0;
                qtotal = 0.0;
                dfyv = 0.0;
                for (int q=1; q<=4; ++q)
                {
                    const OmniType& qval = node.GetValue(year, q);
                    if (!qval.IsEmpty())
                    {
                        ++goodquarters;
                        if (qval.dt == DT_R8)
                            qtotal += (double)qval;
                        else
                            qtotal += (I64)qval;

                        if (q == 4)
                            node.IsProgression = fyv == qval;
                    }
                }
                

                
                if (goodquarters == 4 && !node.GetValue(year).IsEmpty())
                {
                    if (fyv.dt == DT_R8)
                        dfyv = (double)fyv;
                    else
                        dfyv = (I64)fyv;

                    IsTotalingType = 0;
                    goodyear = year;
                    // check if the fy value is instant or sum
                    if (qtotal != 0)
                    {
                        double ratio = dfyv / qtotal;
                        if (ratio >= .95 && ratio <= 1.05)
                        {
                            IsTotalingType = 1;
                            goto PASS2;
                        }
                    }
                }
                else
                {
                    //TODO additional examination using approach like
                    // HY = Q1+Q2 or not ?
                    // 3Q = Q1+Q2+Q3 or 
                    // HY = FY - (Q4 + Q2)
                    // 3Q = FY - (Q4)
                    
                    const OmniType& hyval = node.GetValue(year, FinItemNodeType::FirstHalf);
                    const OmniType& tqval = node.GetValue(year, FinItemNodeType::FirstThreeQuarters);
                    if (!hyval.IsEmpty())   //HY = Q1+Q2 or not ?
                    {
                        if (hyval.dt == DT_R8)
                        {
                            double dhy = (double)hyval;
                            if (dhy == (double)node.GetValue(year, 1) + (double)node.GetValue(year, 2))
                            {
                                IsTotalingType=1;
                                goto PASS2;
                            }
                            else if (dhy == (double)node.GetValue(year, 2))
                            {
                                IsTotalingType = 0;
                                goto PASS2;
                            }
                        }
                        else
                        {
                            double dhy = (I64)hyval;
                            if (dhy == (double)(I64)node.GetValue(year, 1) + (double)(I64)node.GetValue(year, 2))
                            {
                                IsTotalingType=1;
                                goto PASS2;
                            }
                            else if (dhy == (double)(I64)node.GetValue(year, 2))
                            {
                                IsTotalingType = 0;
                                goto PASS2;
                            }
                        }
                    }
                    else if (!tqval.IsEmpty())  //3Q = Q1+Q2+Q3 or not?
                    {
                        double dtqv = (I64)tqval;
                        if (dtqv == (double)(I64)node.GetValue(year, 1) + (double)(I64)node.GetValue(year, 2) + (double)(I64)node.GetValue(year, 3))
                        {
                            IsTotalingType=1;
                            goto PASS2;
                        }
                        else if (dtqv == (double)(I64)node.GetValue(year, 3))
                        {
                            IsTotalingType = 0;
                            goto PASS2;
                        }
                    }
                }
//                switch (IsTotalingType)
//                {
//                    case 0:
//                        observedType |= (int)XPT_Instant;
//                        break;
//                    case 1:
//                        observedType |= (int)XPT_Duration;
//                        break;
//                }
//                node.PeriodType = (XPT) observedType;
            }
        }
        
        
    PASS2:
        // 2nd pass
        for (int year = node.FloorYear; year <= node.CeilYear; ++year)
        {

#ifdef __DEBUG__
                if (node.XbrlId == 20629 && year == 2014)
                {
                    string s = "break";
                }
#endif
            
            
                
            dttmp.set_Year(year);
            XPT pt = node.PeriodType;
            if (dttmp <= _finyr)
            {
                if (pt == XPT_Duration)
                {
                    OmniType fyv = node.GetValue(year);
                    if (fyv.dt == DT_CSTR)
                        continue;
                    else if (fyv.dt == DT_NONE)
                    {
                        if (IsTotalingType == 1 &&  goodquarters == 4 && qtotal)
                        {
                            node.SetValue(year, qtotal);
                            fyv = node.GetValue(year);
                        }
                    }
                    
                    switch (fyv.dt)
                    {
                    case DT_R8:
                    {
                        OmniType vforq[4];
                        if (!fyv.IsEmpty())
                        {
                            if ( (vforq[2] = node.GetValue(year, 3)).IsEmpty() )
                            {
                                const OmniType& f3qv = node.GetValue(year, FinItemNodeType::FirstThreeQuarters);
                                if (!f3qv.IsEmpty())
                                {
                                    if (IsTotalingType == 1)
                                    {
                                        if ((double)fyv != 0)
                                        {
                                            vforq[2] = (double)fyv - (double)f3qv;
                                        }
                                    }
                                    else if (IsTotalingType == 0)
                                    {
                                        vforq[2] = f3qv;
                                    }
                                }
                            }

                            if ( (vforq[1] = node.GetValue(year, 2)).IsEmpty() )
                            {
                                if (!node.GetValue(year, 1).IsEmpty())
                                {
                                    const OmniType& f2hv = node.GetValue(year, FinItemNodeType::FirstHalf);
                                    if (!f2hv.IsEmpty())
                                    {
                                        if (IsTotalingType == 1)
                                        {
                                            double df2hv = f2hv;
                                            double df1qv = node.GetValue(year, 1);
                                            vforq[1] = df2hv - df1qv;
                                        }
                                        else if (IsTotalingType == 0)
                                        {
                                            vforq[1] = f2hv;
                                        }
                                    }
                                }
                            }

                            // arrange four quarters now.
                            vforq[0] = node.GetValue(year, 1);
                            vforq[3] = node.GetValue(year, 4);
                            if (vforq[3].IsEmpty())
                            {
                                if (IsTotalingType == 1)
                                {
                                    if (!vforq[2].IsEmpty() && !vforq[1].IsEmpty() && !vforq[0].IsEmpty())
                                    {
                                        double dfyv = fyv;
                                        vforq[3] = dfyv - ((double)vforq[2] + (double)vforq[1] + (double)vforq[0]);
                                    }
                                }
                                else if (IsTotalingType == 0)
                                {
                                    vforq[3] = fyv;
                                }
                            }
                        }
                        
                        // try to fix 'a' missing quarter if FY is available.
                        if (IsTotalingType == 1 && goodquarters != 4)
                        {
                            double qtotal = 0;
                            int availcount = 0;
                            int missingquarter;
                            for (int q = 1; q<=4; ++q)
                            {
                                const OmniType& qval = vforq[q];
                                if (!qval.IsEmpty())
                                {
                                    ++availcount;
                                    qtotal += (double)qval;
                                }
                                else
                                {
                                    missingquarter = q;
                                }
                            }

                            if (availcount>=4)
                            {
                                if (fyv.IsEmpty())
                                {
                                    node.SetValue(year, qtotal);
                                }
                            }
                            else if (availcount > 2 && availcount < 4)
                            {
                                vforq[missingquarter] = (double)fyv - qtotal;
                                continue;
                            }
                        }
                        else if (IsTotalingType == 1 && node.GetValue(year, 1).IsEmpty())
                        {
                            const OmniType& qlast4val = node.GetValue(year-1, 4);
                            node.SetValue(year, 1, qlast4val );
                        }
                        else
                        {
                            if (IsTotalingType == 0 && fyv.IsEmpty())
                            {
                                node.SetValue(year, vforq[4]);
                            }
                        }
                        
                        const OmniType fullyearvalue = node.GetValue(year);
                        // at last, simply set 3rd, hy and 9months values if those positions are mepty
                        if (vforq[1].IsEmpty() && !vforq[0].IsEmpty() && !node.GetValue(year, FinItemNodeType::FirstHalf).IsEmpty())
                        {
                            vforq[1] = node.GetValue(year, FinItemNodeType::FirstHalf) - vforq[0];
                        }
                        if (vforq[2].IsEmpty() && !node.GetValue(year, FinItemNodeType::FirstThreeQuarters).IsEmpty() && !vforq[0].IsEmpty() && !vforq[1].IsEmpty())
                        {
                            vforq[2] = node.GetValue(year, FinItemNodeType::FirstThreeQuarters) - (vforq[1] + vforq[0]);
                        }
                        if (vforq[3].IsEmpty() && !fullyearvalue.IsEmpty() && !node.GetValue(year, FinItemNodeType::FirstThreeQuarters).IsEmpty())
                        {
                            const OmniType& f3q = node.GetValue(year, FinItemNodeType::FirstThreeQuarters);
                            vforq[3] = fullyearvalue - f3q;
                        }
                        else if (
                                vforq[3].IsEmpty() && 
                                !fullyearvalue.IsEmpty() && 
                                !vforq[0].IsEmpty() &&  
                                !vforq[1].IsEmpty() &&  
                                !vforq[2].IsEmpty()
                                )
                        {
                            vforq[3] = fullyearvalue - ( vforq[0] + vforq[1] + vforq[2] );
                        }
                        for (int i=0; i<4; ++i)
                            node.SetValue(year, i+1, vforq[i]);
                            
                    }
                        break;
                    case DT_I64:
                    {
                        if (node.GetValue(year, 4).IsEmpty())
                        {
                            node.SetValue(year, 4, fyv);
                        }
                        if (node.GetValue(year, 3).IsEmpty())
                        {
                            const OmniType& v = node.GetValue(year, FinItemNodeType::FirstThreeQuarters).IsEmpty();
                            if (!v.IsEmpty())
                                node.SetValue(year, 3, v);
                        }
                        if (node.GetValue(year, 2).IsEmpty())
                        {
                            const OmniType& v = node.GetValue(year, FinItemNodeType::FirstHalf).IsEmpty();
                            if (!v.IsEmpty())
                                node.SetValue(year, 2, v);
                        }
                    }
                        break;
                    }
                }
                else if (pt == XPT_Instant)
                {
                    
#ifdef __DEBUG__                
                if (node.XbrlId == 18733 && year == 2013)
                {
                    string b = "b";
                }
#endif
                    
                    // if quarter 4 value is set and not FY then Q4->FY
                    // if FY is set and Q4 not set then FY -> Q4
                    if (!node.GetValue(year).IsEmpty() && node.GetValue(year, 4).IsEmpty())
                        node.SetValue(year, 4, node.GetValue(year));
                    else if (node.GetValue(year).IsEmpty() && !node.GetValue(year, 4).IsEmpty())
                        node.SetValue(year, node.GetValue(year, 4));
                }
            }
            else
            {
                if (pt == XPT_Duration)
                {
                    if (node.GetValue(year, 1).IsEmpty())
                        node.SetValue(year, 1, node.GetValue(year-1, 4));
                }
            }
        }
        
    END:
        continue;
    }
}

void CFinancialsConsolidated::Print() 
{
    for (FinItemNodeType& node : _l)
    {
       // LOGI(_logger, "AccountId: " << node.XbrlId << ", Account Name: " << node.XbrlName);
        for (auto pair : node.NodeValue)
        {
           // LOGI(_logger, "Year : " << 2000 + pair.first);
           // LOGI(_logger, "Yearly Value: " << pair.second[1][0]);
            for (int i = 0; i != 4; ++i)
            {
               // LOGI(_logger, "Q" << i+1 << " Value: " << pair.second[0][i]);
            }
            for (int i = 0; i != 2; ++i)
            {
               // LOGI(_logger, "H" << i+1 << " Value: " << pair.second[2][i]);
            }
        }
        
    }
}

FinItemNodeType& CFinancialsConsolidated::ManageFinItemNode(FinItemNodeType& pnode, unsigned long xid, bool& newlyCreated) 
{
    FinItemNodeType::SEGMENTVALUES::iterator it ;
    if ( ( it = pnode.SegValue.find(xid) ) == pnode.SegValue.end() )
    {
        FinItemNodeType node(xid);
        node.pConsolidated = this;
        auto pair = pnode.SegValue.insert(make_pair(xid, node));
        newlyCreated = true;
        return pair.first->second;
    }
    else
    {
        newlyCreated = false;
        return it->second;
    }
}


FinItemNodeType& CFinancialsConsolidated::ManageFinItemNode(UID xid, int& index, bool& newlyCreated)
{
    FinItemListIndex::const_iterator it ;
    if ( (it =_m.find(xid)) == _m.end() )
    {
        index = _l.size();
        FinItemNodeType node(xid);
        node.pConsolidated = this;
        _l.push_back(node);
        _m.insert(make_pair(xid, index));
        newlyCreated = true;
    }
    else
    {
        index = it->second;
        newlyCreated = false;
    }
    return _l[index];
}


void CFinancialsConsolidated::Serialize(ISerializedData<IString>* pIn) 
{
    
}

void CFinancialsConsolidated::SubmitToDb() 
{
    class CTask : public CObjectRoot, public ITask
    {
        SP<CFinancialsConsolidated> _o;
        
    public:
        CTask(ISession* ps) : CObjectRoot(ps)
        {
        }
        
        SE_CODE Init(CFinancialsConsolidated* o)
        {
            _o = 0;
            return S_OK;
        }
        
        virtual void Execute()
        {
            
        }
        
        virtual ~CTask()
        {
            
        }
    };
    
    
    SP<IApp> pApp;
    THROWIFFAILED(SEOpenApp(&pApp), "Failed to create application instance.");
    SP<IThreadPool> ptp;
    THROWIFFAILED(pApp->get_ThreadPool(TPT_ParallelPool, &ptp), "Failed to create threadpool instance.");
    SP<ITask> task;
    THROWIFFAILED(CObject<CTask>::Create(get_Session(), this, &task), "Failed to create a task instance.");
    ptp->RunTask(task);
}

const CDate& CFinancialsConsolidated::get_FinancialYearEnd() 
{
    return _finyr;
}

long unsigned int CFinancialsConsolidated::get_Records(std::initializer_list<unsigned long> xids, IXbrlRecordList** ppOut) 
{
    SP<IXbrl> papi;
    RETURNIFFAILED(XbrlOpen(get_Session(), &papi));
    
    SP<CXbrlRecordList> pList;
    RETURNIFFAILED(CObject<CXbrlRecordList>::Create(get_Session(), this, &pList));
    SP<IString> plabel;
    if (!xids.size())
    {
        // return all;
        for (auto it = _l.begin(); it != _l.end(); ++it)
        {
            const FinItemNodeType& n = (*it);
            papi->get_Label(&plabel, n.XbrlId, _cik.c_str());
            SP<IXbrlRecord> pRec;
            CObject<CXbrlRecord>::Create(get_Session(), n, (plabel ? plabel->get_Buffer() : ""), &pRec);
            plabel->Clear();
            pList->Add(pRec);
        }
    }
    else
    {
        for (UID xid : xids)
        {
            auto it = _m.find(xid);
            if (it != _m.end())
            {
                const FinItemNodeType& n = _l[it->second];
                SP<IXbrlRecord> pRec;
                papi->get_Label(&plabel, n.XbrlId, _cik.c_str());
                CObject<CXbrlRecord>::Create(get_Session(), n, (plabel ? plabel->get_Buffer() : ""), &pRec);
                plabel->Clear();
                pList->Add(pRec);
            }
        }
    }
    pList.CopyTo(ppOut);
    return S_Success;
}

bool CFinancialsConsolidated::get_HasRecord(unsigned long xbrlId) 
{
    return !(_m.find(xbrlId) == _m.end());
}


long unsigned int CFinancialsConsolidated::get_Record(unsigned long xbrlId, IXbrlRecord** ppOut) 
{
    auto it = _m.find(xbrlId);
    if (it == _m.end())
    {
       // LOGI(_logger, "XbrlId Record " << xbrlId << "Not found.");
        return E_NotFound;
    }
    SP<IXbrl> papi;
    RETURNIFFAILED(XbrlOpen(get_Session(), &papi));
    const FinItemNodeType& n = _l[it->second];
   // LOGI(_logger, "XbrlId Record " << xbrlId << " is found.Returning xbrl record instance.");
    SP<IString> plabel;
    papi->get_Label(&plabel, n.XbrlId, _cik.c_str());
    return CObject<CXbrlRecord>::Create(get_Session(), n, (plabel ? plabel->get_Buffer():"") , ppOut);
}

long unsigned int CFinancialsConsolidated::get_Record(unsigned long xbrlId, unsigned long segmentId, IXbrlRecord** ppOut) 
{
    
    if (!segmentId)
        return get_Record(xbrlId, ppOut);
    
    auto it0 = _m.find(xbrlId);
    if (it0 == _m.end())
        return E_NotFound;
    // look for segmetn
    const FinItemNodeType& n = _l[it0->second];
    auto it1 = n.SegValue.find(segmentId);
    if (it1 == n.SegValue.end())
        return E_NotFound;
    
    SP<IXbrl> papi;
    RETURNIFFAILED(XbrlOpen(get_Session(), &papi));
    SP<IString> plabel;
    papi->get_Label(&plabel, n.XbrlId, _cik.c_str());
    return CObject<CXbrlRecord>::Create(get_Session(), it1->second, (plabel ? plabel->get_Buffer():""), ppOut);
}


long unsigned int CFinancialsConsolidated::get_Performance(IFinancialPerformance** ppOut) 
{
    return CObject<CFinancialPerformance>::Create(get_Session(), this, ppOut);
}

long unsigned int CFinancialsConsolidated::get_PerformanceRatio(unsigned long perfId, IFinancialRatio** ppOut) 
{
    return CObject<CFinancialRatio>::Create(get_Session(), perfId, this, ppOut);
}

long unsigned int CFinancialsConsolidated::get_FiscalPeriodMappings(IFinancialPeriod** ppOut) 
{
    _fper.CopyTo(ppOut);
    return _fper ? S_OK : S_False;
}

long unsigned int CFinancialsConsolidated::get_MatchedAccountRecord(unsigned long matchId, IXbrlRecord** ppOut) 
{
    SP<IXbrl> px;
    RETURNIFFAILED(XbrlOpen(get_Session(), &px));
    return dynamic_cast<CXbrl*>(px._ptr)->_GetMatchedAccountValue(this, matchId, ppOut);
}



#ifdef __DEBUG__

void CFinancialsConsolidated::_AddRef() 
{
    CObjectRoot::_AddRef();
   // LOGE(_logger, "Adding Reference to " << _rc);
}

void CFinancialsConsolidated::_DecRef() 
{
    auto decval = _rc-1;
   // LOGE(_logger, "Decreasing Reference to " << decval);
    if (!decval)
    {
        //raise(SIGTRAP);
        string s = "testbreak";
    }
    CObjectRoot::_DecRef();
}



#endif