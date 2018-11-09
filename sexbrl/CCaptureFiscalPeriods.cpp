/* 
 * File:   CCaptureFiscalPeriods.cpp
 * Author: santony
 * 
 * Created on August 22, 2014, 7:28 PM
 */

#include "sexbrlinternal.h"
#include "CCaptureFiscalPeriods.h"
#include "../seglobal/CFinancialPeriod.h"
#include "../semysql/CMyqlUtilities.h"

using namespace se;
using namespace se::data;

#define SEDBCONN        se::data::CSEConnections::GetMain()

long unsigned int CCaptureFiscalPeriods::Capture(CIK cik, IFinancialPeriod** ppOut) 
{
    
    struct FinDates
    {
        I16 DocTypeId;
        CDate FinYearEnd;
        FPRDS FiscalPeriod;
        CDate StartDate;
        CDate EndDate;
        I16 NumberOfDays;
        bool IsQ4Provided;
        
        void Clear()
        {
            NumberOfDays = DocTypeId = 0;
            StartDate = EndDate = FinYearEnd = CDate::Empty();
            FiscalPeriod = FPRDS::Q1;
            IsQ4Provided = true;
        }
    };
    
    CSmartConnection conn(SEDBCONN);
    auto q = conn->query();
    try
    {
        q << "call list_FinYearEndHistory(" << cik << ")";
        SP<CFinancialPeriod> _fper;
        RETURNIFFAILED(CObject<CFinancialPeriod>::Create(nullptr, &_fper));
        FinDates fdFirst, fdFirstFY;
        fdFirst.DocTypeId = fdFirstFY.DocTypeId = 0;
        FPRDS lstfprd;
        FinDates fd;
        for (const Row& r : q.store())
        {
            fd.Clear();
            fd.IsQ4Provided = false;
            fd.DocTypeId = r[1];
            sql_date sdt = r[2];
            CMyqlUtilities::PopulateDateTime(sdt, fd.FinYearEnd);
            string sfper = (CSTR)r[3];
            if (sfper == "FY")
                fd.FiscalPeriod = FPRDS::FY;
            else if (sfper == "Q1")
                fd.FiscalPeriod = FPRDS::Q1;
            else if (sfper == "Q2")
                fd.FiscalPeriod = FPRDS::Q2;
            else if (sfper == "Q3")
                fd.FiscalPeriod = FPRDS::Q3;
            else if (sfper == "Q4")
            {
                fd.FiscalPeriod = FPRDS::Q4;
                fd.IsQ4Provided = true;
            }
            else if (sfper == "HY")
                fd.FiscalPeriod = FPRDS::HY;

            if (lstfprd == fd.FiscalPeriod && (int)lstfprd < 4)
            {
                fd.FiscalPeriod = (FPRDS)((int)fd.FiscalPeriod+1);
            }

            sdt = r[4];
            CMyqlUtilities::PopulateDateTime(sdt, fd.StartDate);
            sdt = r[5];
            CMyqlUtilities::PopulateDateTime(sdt, fd.EndDate);
            fd.NumberOfDays = r[7]; // index 6 is filedate

            auto fp = fd.FiscalPeriod;
            if (fp == FPRDS::FY)
                fp = FPRDS::Q4;

            _fper->SetPeriod(fd.FinYearEnd, fp, fd.StartDate, fd.EndDate);

            if (!fdFirst.DocTypeId)
            {
                fdFirst = fd;
            }

            if (!fdFirstFY.DocTypeId and fd.FiscalPeriod == FPRDS::FY)
            {
                fdFirstFY = fd;
            }

            lstfprd = fd.FiscalPeriod;
        }

        // extend financial periods upto year 2000
        int fiscalq = (int)fdFirst.FiscalPeriod;
        CDate endDate = fdFirst.EndDate;
        while (--fiscalq > 0)
        {
            FPRDS fq = (FPRDS)fiscalq;
            CDate startDate = fdFirst.StartDate;
            endDate = endDate.AddSubQuarters(-1);
            if ( (endDate - startDate).Day < 85 )
            {
                startDate = endDate.AddSubQuarters(-1);
            }
            _fper->SetPeriod(fdFirst.FinYearEnd, fq, startDate, endDate);
        }

        CDate yearEnd = fdFirstFY.FinYearEnd;
        CDate startDate = fdFirstFY.StartDate;
        endDate = fdFirstFY.EndDate;
        do 
        {
            yearEnd = yearEnd.AddSubYears(-1);
            startDate = startDate.AddSubYears(-1);
            endDate = endDate.AddSubYears(-1);
            fiscalq = (int) FPRDS::Q4;
            _fper->SetPeriod(yearEnd, FPRDS::Q4, startDate, endDate);
            while (--fiscalq > 0)
            {
                FPRDS fq = (FPRDS)fiscalq;
                CDate sd = startDate;
                endDate = endDate.AddSubQuarters(-1);
                if ( (endDate - sd).Day < 85 )
                {
                    sd = endDate.AddSubQuarters(-1);
                }
                _fper->SetPeriod(yearEnd, fq, sd, endDate);
            }            
        } while (yearEnd.get_Year() > 2000);

        // Add one extra financial year into the future.
        fiscalq = (int)fd.FiscalPeriod;
        CDate ed = fd.EndDate;
        while (++fiscalq <= (int)FPRDS::Q4)
        {
            ed = ed.AddSubQuarters(1);
            FPRDS per = (FPRDS)fiscalq;
            _fper->SetPeriod(fd.FinYearEnd, per, fd.StartDate, ed);
        }

        while (q.more_results())
            q.store_next();    

        _fper.CopyTo(ppOut);
        return S_OK;
    }
    catch(...)
    {
        while (q.more_results())
            q.store_next();    

        ////LoggerPtr logger = Logger::getLogger("se.xbrl.capturefiscalperiods");
       // LOGE(logger, "Failed to capture fiscal periods for cik " << cik );
        return E_DBQueryException;
    }
}
