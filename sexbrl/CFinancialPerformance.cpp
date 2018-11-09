/* 
 * File:   CFinancialPerformance.cpp
 * Author: santony
 * 
 * Created on February 14, 2014, 2:36 PM
 */

#include "sexbrlinternal.h"
#include "CFinancialPerformance.h"
#include "CFinancialRatioList.h"
////#include <log4cxx/logger.h>
#include "../seglobal/exceptions.h"
#include "CXbrlCalculator.h"
#include "CFinancialRatio.h"
#include <boost/regex.hpp>

////using namespace log4cxx;


namespace se
{
    namespace xbrl
    {
       CFinancialPerformance::CFinancialPerformance(ISession* ps) : CObjectRoot(ps)
        {
           
        }


        CFinancialPerformance::~CFinancialPerformance() 
        {
        }

        long unsigned int CFinancialPerformance::Init(IFinancialsConsolidated* pConsolidated) 
        {
            _pCon.Assign(pConsolidated);
            _cik = _pCon->get_Cik();
            return BuildRatios();
        }

        SE_CODE CFinancialPerformance::Init(CIK cik, bool OnlyPubliclyDisplayed)
        {
            _cik = cik;
            return BuildRatiosFromCache(OnlyPubliclyDisplayed);
        }

        double CFinancialPerformance::get_Ratio(FNR ratio) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.financialperformacne.get_ratio");
            if (SUCCEEDED(BuildRatios()))
            {
                SP<IFinancialRatio> pfr;
                if (SUCCEEDED(_pRatios->get_Ratio(ratio, &pfr)))
                {
                    return pfr->get_Value();
                }
            }
           // LOGW(_logger, "No formula defined for the performance ratio #" << ratio << ". Build ratio call failed.");
            return 0.0;
        }

        long unsigned int CFinancialPerformance::BuildRatiosFromCache(bool OnlyPubliclyDisplayed) 
        {
            if (_pRatios && _pRatioInGroups)
                return S_OK;
            
            _pRatios.Clear();
            _pRatioInGroups.Clear();
            
            CSmartConnection conn(CSEConnections::GetMain());
            Query q = conn->query();
            //q << "select * from list_PerfRatioValues where cik = " << _cik << " order by group_id, perf_id, year, quarter";
            q << "SELECT a.*, c.group_name FROM se_fin_perf_ratios a ";
            q << "inner join sa.se_fin_perf_cik_ratio b on (a.perf_id = b.perf_id) ";
            q << "inner join se_fin_perf_ratio_groups c on (a.group_id = c.group_id) ";
            q << "where b.cik = " << _cik ;
            if (OnlyPubliclyDisplayed)
                q << " and perf_publicdisplay = '1' ";
            q << " order by group_id, perf_id ";
            StoreQueryResult sqr = q.store();
            if (!sqr.size())
            {
                LOGRETURNIFFAILED(E_NotFound, "Performance ratios not avialable for this company..");
            }
            LOGRETURNIFFAILED(CObject<CFinancialRatioList>::Create(get_Session(), &_pRatios), "Failed to create ratio list instance.");
            // also create the grouped ratio stuff
            LOGRETURNIFFAILED(CObject<CFinancialRatioGroupList>::Create(get_Session(), &_pRatioInGroups), "Failed to create ratio list group instance.");

            UID lastGroupId = -1;    // the sql is ordered first by group id in the stored procedure.
            SP<CFinancialRatioGroup> pgrp;
            for (const Row& row : sqr)
            {
                UID groupId = row["group_id"];
                if (lastGroupId != groupId)
                {
                    pgrp.Clear();
                    RETURNIFFAILED(CObject<CFinancialRatioGroup>::Create(get_Session(), &pgrp));
                    _pRatioInGroups->Add(pgrp);
                    pgrp->set_Id(groupId);
                    pgrp->set_Name(row["group_name"]);
                }
                
                UID perfid = row["perf_id"];
                SP<IFinancialRatio>pfr;
                if (ISOK(CObject<CFinancialRatio>::Create(get_Session(), perfid, _cik.c_str(), &pfr)))
                {
                    _pRatios->Add(pfr);
                    pgrp->Add(pfr._ptr);
                }
                lastGroupId = groupId;
            }
            
           // LOGI(_logger, "There are " << _pRatios->get_Count() << " ratios for " << _cik);
            
            return S_OK;
        }

        
        SE_CODE CFinancialPerformance::BuildRatios() 
        {
            if (_pRatios)
                return S_OK;
            
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.financialperformacne.buildratios");
            // get sic code
            try
            {
                CSmartConnection conn(CSEConnections::GetMain());
                Query q = conn->query();
                try
                {
                    q << "call list_AllowedPerformanceFormulae(" << _pCon->get_Cik() << ")";
                   // LOGIP(q.str());
                    StoreQueryResult sqr = q.store();
                    // get all ratios
                    if (!sqr.size())
                        THROWIFFAILED(E_NoDataToReturn, "No records found in se_fin_perf_ratios.");

                    THROWIFFAILED(CObject<CFinancialRatioList>::Create(get_Session(), &_pRatios), "Failed to create ratio list instance.");
                    SE_CODE retCode;
                    for (const Row& row : sqr)
                    {
                        UID perfid = row["perf_id"];
                        SP<IFinancialRatio>pfr;
                        retCode = CObject<CFinancialRatio>::Create(get_Session(), perfid, _pCon, &pfr);
                        if (ISOK(retCode))
                            _pRatios->Add(pfr);
                        else
                        {
                           // LOGE(_logger, "Fatal exception while calculating performance ratio " << perfid << " for cik " << _pCon->get_Cik());
                        }
                    }
                    
                    while (q.more_results())
                        q.store_next();
                    
                }
                catch(...)
                {
                    if (q)
                    {
                        while (q.more_results())
                            q.store_next();
                    }
                    throw;
                }
                
                return S_OK;
            }
            CATCHCLAUSE("CFinancialPerformance::BuildRatios");
        }

        long unsigned int CFinancialPerformance::get_Ratio(FNR ratio, IFinancialRatio** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.financialperformacne.get_ratio");
            if (SUCCEEDED(BuildRatios()))
            {
                return _pRatios->get_Ratio(ratio, ppOut);
            }
            return E_NotFound;
        }
        

        CIK CFinancialPerformance::get_CIK() 
        {
            return _cik.c_str();
        }

        long unsigned int CFinancialPerformance::get_Ratios(IFinancialRatioList** ppOut) 
        {
            _pRatios.CopyTo(ppOut);
           // LOGI(_logger, "There are " << _pRatios->get_Count() << " ratios for " << _cik);
            return S_OK;
        }

        long unsigned int CFinancialPerformance::get_RatioGroups(IFinancialRatioGroupList** ppOut) 
        {
            _pRatioInGroups.CopyTo(ppOut);
            return S_OK;
        }

        void CFinancialPerformance::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->Add("CIK", _cik.c_str());
            pIn->Add("Ratio Groups", _pRatioInGroups);
        }
    }
}