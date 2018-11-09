/* 
 * File:   CFinancialPeriod.h
 * Author: santony
 *
 * Created on July 18, 2014, 2:29 PM
 */

#ifndef CFINANCIALPERIOD_H
#define	CFINANCIALPERIOD_H

#include "CObjectRoot.h"
#include <tuple>
#include "../seglobal/CDate.h"
#include <map>

using namespace std;

namespace se
{
    
    class CFinancialPeriod : public CObjectRoot, public IFinancialPeriod
    {
        typedef std::tuple<CDate, CDate> StartAndEndDates;
        typedef std::map<FPRDS, StartAndEndDates> QuarterRange;
        typedef std::map<I16, QuarterRange> FiscalPeriods;

    public:
        CFinancialPeriod(ISession*);
        ~CFinancialPeriod();
        
        virtual const DATAPERIOD& get_LatestPeriod() ;

        virtual long unsigned int get_CompanyPeriod(const DATAPERIOD& AnyNaturalPeriod, DATAPERIOD* pCompanyPeriod);

        virtual long unsigned int get_NaturalPeriod(const DATAPERIOD& AnyCompanyPeriod, DATAPERIOD* pNaturalPeriod);
        
        virtual bool FindFiscalPeriod(const CDate& dtIn, DATAPERIOD* dpOut);

        virtual bool FindFiscalPeriod(const DATAPERIOD dpIn, DATAPERIOD* dpOut);

        void Clear();

        void SetPeriod(const CDate finyearend, FPRDS period, const CDate& startDate, const CDate& endDate);
    
    private:
        void CheckGaps();
        
    private:
        bool _bGapCheck = false;
        FiscalPeriods _periods;
        DATAPERIOD _recentPeriod;
        DATAPERIOD _companyPeriod;
    };
}
#endif	/* CFINANCIALPERIOD_H */

