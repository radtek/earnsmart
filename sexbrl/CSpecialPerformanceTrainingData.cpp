/* 
 * File:   CSpecialPerformanceTrainingData.cpp
 * Author: santony
 * 
 * Created on July 21, 2014, 4:09 PM
 */

#include "sexbrlinternal.h"
#include "CSpecialPerformanceTrainingData.h"
#include "CExplicitFormulaManager.h"
#include "../seglobal/CFunctionCache.h"
#include <string>
#include "../seglobal/exceptions.h"

#define CONN CSEConnections::GetMain()

namespace se
{
    namespace xbrl
    {

        CSpecialPerformanceTrainingData::CSpecialPerformanceTrainingData(ISession* ps, UID perfId) : _ps(ps), _perfId(perfId)
        {
            
        }

        CSpecialPerformanceTrainingData::~CSpecialPerformanceTrainingData() 
        {
            
        }

        long unsigned int CSpecialPerformanceTrainingData::Build(CIK cik, ITrainingTable** ppOut) 
        {
            return E_NotSupported;
        }
        
        long unsigned int CSpecialPerformanceTrainingData::Build(CIK cik, ITrainingTableList** ppOut) 
        {
            switch (_perfId)
            {
                case 26:    // competition ratios
                    return BuildRatio26(cik, ppOut);
            }
            return E_NotSupported;
        }


        long unsigned int CSpecialPerformanceTrainingData::BuildRatio26(CIK cik, ITrainingTableList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.specialtrainingdata");
            const string funcname = "se.xbrl.BuildRatio26";
            
            RTRY
            
                ITrainingTableList* p;
                if (!FC.TrySPGetFValue(funcname, (string)cik, p))
                {
                    std::lock_guard<std::mutex> g(_mtx);
                    if (!FC.TrySPGetFValue(funcname, (string)cik, p))
                    {
                        SP<IXbrl> pxbrl;
                        RETURNIFFAILED(XbrlOpen(_ps, &pxbrl));
                        CSmartConnection conn(CONN);
                        Query q = conn->query();
                        try
                        {
                            q << "call list_ActiveCompetitions(" << cik << ")";
                            auto sqr = q.store();
                            if (!sqr.size())
                                throw seexception(E_NoDataToReturn, "No complementory companies.");
                           // LOGI(_logger, "List of competition: " << sqr.size());
                            THROWIFFAILED(ITrainingTableList::Create(_ps, &p), "Training table creationg failed");
                            for (const Row& r : sqr)
                            {
                                const auto& str = (string)r["cik"];
                                if (str != (string)cik)
                                {
                                    SP<ITrainingTable> pt;
                                    auto ret = BuildCorelation(cik, (CSTR)r["cik"], &pt);
                                    if (ret != S_OK)
                                        continue;
                                    p->Add(pt);
                                }
                            }
                            while (q.more_results())
                                q.store_next();
                        }
                        catch(const std::exception& ex)
                        {
                            while (q.more_results())
                                q.store_next();
                           // LOGEP(ex.what());
                            return E_TrainingDataNoResult;
                        }
                        catch(...)
                        {
                            while (q.more_results())
                                q.store_next();
                           // LOGEP("Unknown exception.");
                            return E_TrainingDataNoResult;
                        }
                        FC.SPSetFValue(funcname, (string)cik, p, 3600);
                        *ppOut = p;
                        return S_OK;
                    }
                }
                p->AddRef();
                *ppOut = p;
                return S_OK;
                
            RCATCH("BuildRatio26")
        }

        long unsigned int CSpecialPerformanceTrainingData::BuildCorelation(CIK left, CIK right, ITrainingTable** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.specialtrainingdata");
            // formula used is
            // get the revenue growths of left and right companies
            // get the a-mean of left and right companies
            // for each row get a product of (leftR - leftA) * (rightR - rightA) (R = revenue growth, A means average for left and right)
            // add the result to the table for each period.
            // keep previous value which can be added for missing records.
            try
            {
                SP<IXbrl> pxbrl;
                RETURNIFFAILED(XbrlOpen(_ps, &pxbrl));
                SP<ITrainingTable> pleft, pright;
                RETURNIFFAILED(pxbrl->GetPerformanceTrainingData(left, 21, DATAPERIOD(), DATAPERIOD(), &pleft));
                RETURNIFFAILED(pxbrl->GetPerformanceTrainingData(right, 21, DATAPERIOD(), DATAPERIOD(), &pright));
                // left is what we use as base for looping
                if (!pleft->get_Count() || !pright->get_Count())
                    return E_NoDataToReturn;

                SP<ITrainingTable> ptbl;
                RETURNIFFAILED(ITrainingTable::Create(_ps, &ptbl));
                R8 lavg, ravg;
                pleft->get_GMean(false, &lavg);
                pright->get_GMean(false, &ravg);
                if (!lavg || std::isnan(lavg) || std::isinf(lavg) || !ravg || std::isnan(ravg) || std::isinf(ravg))
                    return E_AlgorithmError;
                // quarterly avg
                R8 lastval = 0;
                for (ITrainingRecord* pr : *pleft)
                {
                    DATAPERIOD dp = pr->get_Period();
                    SP<ITrainingRecord> prrec;
                    if (ISOK(pright->get_Record(dp, &prrec)) && prrec)
                    {
                        R8 lval = pr->get_Value();
                        R8 rval = prrec->get_Value();
                        R8 rel = (lval-lavg) * (rval - ravg);
                        ptbl->AddRecord(dp, rel);
                        lastval = rel;
                    }
                    else
                    {
                        if (lastval)
                            ptbl->AddRecord(dp, lastval);
                        else
                            ptbl->AddRecord(dp, OmniType::Empty());
                    }
                }
                
                // get average and update the profile
                R8 avg;
                ptbl->get_AMean(false, &avg);
                if (!std::isnan(avg) && !std::isinf(avg))
                    SaveCompetitionProfile(left, right, (avg<0.0?true:false));
                
                ostringstream ostr;
                ostr << "P_26-" << right ;
                ptbl->set_Name(ostr.str().c_str());
                ptbl.CopyTo(ppOut);
                return S_OK;
            }
            CATCHCLAUSE("BuildCorelation")
        }

        long unsigned int CSpecialPerformanceTrainingData::SaveCompetitionProfile(CIK primary, CIK secondary, bool IsComplementory) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.specialtrainingdata");

            RTRY

            CSmartConnection conn(CONN);
            Query q = conn->query();
            q << "REPLACE se_fin_perf_competionprofiles (primary_cik, secondary_cik, ComplOrSuppl) ";
            q << " VALUES (";
            q << primary << ", ";
            q << secondary << ", ";
            q << quote << (IsComplementory ? "C" : "S");
            q << ")";
            q.exec();
            return S_OK;
            
            RCATCH("SaveCompetitionProfile")
        }

    }
}