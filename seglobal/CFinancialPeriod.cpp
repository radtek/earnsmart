/* 
 * File:   CFinancialPeriod.cpp
 * Author: santony
 * 
 * Created on July 18, 2014, 2:29 PM
 */

#include "seglobalinternal.h"
#include "CFinancialPeriod.h"
#include "CDate.h"
#include "CQuarter.h"
#include "Object.h"
#include <cmath>
#include "../seglobal/exceptions.h"
#include <boost/range/adaptors.hpp>
#include <algorithm>

using namespace std;

namespace se
{
    CFinancialPeriod::CFinancialPeriod(ISession* ps):CObjectRoot(ps)
    {

    }

    CFinancialPeriod::~CFinancialPeriod() 
    {
        Clear();
    }
    
    void CFinancialPeriod::SetPeriod(const CDate finyearend, FPRDS period, const CDate& startDate, const CDate& endDate) 
    {
        QuarterRange* cor;
        FiscalPeriods::iterator ittop;
        ittop = _periods.find(finyearend.get_Year());
        if (ittop == _periods.end())
        {
            ittop = _periods.insert(make_pair(finyearend.get_Year(), QuarterRange())).first;
        }
        cor = &(ittop->second);
        QuarterRange::const_iterator itq = cor->find(period);
        if (itq == cor->end())
        {
            auto se = std::make_tuple(startDate, endDate);
            cor->insert(make_pair(period, se));
            auto dp = endDate.ToPeriodForm();
            if (dp > this->_recentPeriod)
                _recentPeriod = dp;
        }        
    }

    void CFinancialPeriod::CheckGaps() 
    {
        if (!_bGapCheck)
        {
            
            FiscalPeriods pcpy;
            //pcpy = std::move(_periods); 
            for (auto& pr : boost::adaptors::reverse(pcpy))
            {
               I16 yr = pr.first;
               QuarterRange& r = pr.second;
               
            }
            _bGapCheck = true;
        }
    }


    void CFinancialPeriod::Clear() 
    {
        _periods.clear();
    }

    bool CFinancialPeriod::FindFiscalPeriod(const DATAPERIOD dpIn, DATAPERIOD* dpOut)
    {
        CheckGaps();
        // start with one year before, year and next year
        I16 start = dpIn.Year - 1;
        I16 end = dpIn.Year + 1;
        for (auto i = start; i <= end; ++i)
        {
            FiscalPeriods::const_iterator find = _periods.find(i);
            if (find != _periods.end())
            {
                // scan the quarter ranges
                for (auto pr : find->second)
                {
                    StartAndEndDates se = pr.second;
                    auto sd = get<0>(se);
                    auto ed = get<1>(se);
                    if (dpIn.Quarter >= sd.get_Quarter() && dpIn.Quarter <= ed.get_Quarter())
                    {
                        dpOut->Year = i;
                        dpOut->Quarter = (I16)pr.first;
                        return true;
                    }
                }
            }
        }
        return false;        
    }
    
    bool CFinancialPeriod::FindFiscalPeriod(const CDate& dt, DATAPERIOD* dpOut) 
    {
        CheckGaps();
        I16 start = dt.get_Year() - 1;
        I16 end = dt.get_Year() + 1;
        for (auto i = start; i <= end; ++i)
        {
            FiscalPeriods::const_iterator find = _periods.find(i);
            if (find != _periods.end())
            {
                // scan the quarter ranges
                for (auto pr : find->second)
                {
                    StartAndEndDates se = pr.second;
                    auto sd = get<0>(se);
                    auto ed = get<1>(se);
                    if (dt >= sd && dt <= ed)
                    {
                        dpOut->Year = i;
                        dpOut->Quarter = (I16)pr.first;
                        return true;
                    }
                }
            }
        }
        return false;  
    }

    long unsigned int CFinancialPeriod::get_CompanyPeriod(const DATAPERIOD& AnyNaturalPeriod, DATAPERIOD* pCompanyPeriod) 
    {
        CheckGaps();
        //TODO if month is not given but quarter and year is given, then
        // we have search the map with matching date (for that quarter) in nattoco map
        if (AnyNaturalPeriod.Year && AnyNaturalPeriod.Quarter)
        {
            I16 start = AnyNaturalPeriod.Year - 1;
            I16 end = AnyNaturalPeriod.Year + 1;
            for (auto i = start; i <= end; ++i)
            {
                FiscalPeriods::const_iterator find = _periods.find(i);
                if (find != _periods.end())
                {
                    // scan the quarter ranges
                    for (auto pr : find->second)
                    {
                        StartAndEndDates se = pr.second;
                        auto sd = get<0>(se);
                        auto ed = get<1>(se);
                        if (ed.get_Year() == AnyNaturalPeriod.Year)
                        {
                            if (AnyNaturalPeriod.Quarter <= ed.get_Quarter())
                            {
                                pCompanyPeriod->Year = i;
                                pCompanyPeriod->Quarter = (I16) pr.first;
                                return S_OK;
                            }
                        }
                    }
                }
            }
            return S_False;        
        }
    }
    
    long unsigned int CFinancialPeriod::get_NaturalPeriod(const DATAPERIOD& AnyCompanyPeriod, DATAPERIOD* pNaturalPeriod) 
    {
        CheckGaps();
        //TODO if month is not given but quarter and year is given, then
        // we have search the map with matching date (for that quarter) in cotonat map
        if ( AnyCompanyPeriod.Year && AnyCompanyPeriod.Quarter )
        {
            FiscalPeriods::const_iterator find = _periods.find(AnyCompanyPeriod.Year);
            if (find != _periods.end())
            {
                auto& rng = find->second;
                auto it = rng.find((FPRDS) AnyCompanyPeriod.Quarter);
                if (it != rng.end())
                {
                    StartAndEndDates se = it->second;
                    auto sd = get<0>(se);
                    auto ed = get<1>(se);
                    pNaturalPeriod->Year = ed.get_Year();
                    pNaturalPeriod->Quarter = ed.get_Quarter();
                    return S_OK;
                }
            }
        }
        return S_False;
    }

    const DATAPERIOD& CFinancialPeriod::get_LatestPeriod() 
    {
        if (_companyPeriod.IsEmpty())
        {
            auto ret = get_CompanyPeriod(_recentPeriod, &_companyPeriod);
            if (FAILED(ret))
                throw seexception(ret, "Failed to convert recent period to company period.");
        }
        return _companyPeriod;
    }

    
    
    SE_CODE GBLEXP IFinancialPeriod::Create(ISession* ps, IFinancialPeriod** ppOut)
    {
        return CObject<CFinancialPeriod>::Create(ps, ppOut);
    }
}