/* 
 * File:   CExplicitFormulaManager.cpp
 * Author: santony
 * 
 * Created on April 17, 2014, 12:47 AM
 */

#include "sexbrlinternal.h"
#include "CExplicitFormulaManager.h"

#include "../semysql/CSEConnections.h"
#include "../seglobal/CFunctionCache.h"
#include "CXbrl.h"
#include "CFinancialRatio.h"
#include "CPersistPerformanceData.h"
#include <Poco/ScopedLock.h>
#include "../external/mariadbpp/ssqls.h"
#include "../seglobal/exceptions.h"

using namespace se::data;
using namespace se::cache;
using namespace mysqlpp;

#define CONN CSEConnections::GetMain()

namespace se
{
    namespace xbrl
    {
        Poco::Mutex CExplicitFormulaManager::_lock;
        
        SE_CODE CExplicitFormulaManagerState::Instance(ISession* ps, CExplicitFormulaManagerState** ppOut)
        {
            return CObject<CExplicitFormulaManagerState>::Create(ps, ppOut);
        }

        SE_CODE CExplicitFormulaManager::Instance(ISession* ps, CExplicitFormulaManager** ppOut)
        {
            CExplicitFormulaManager* p;
            if (!FC.TrySPGetFValue("se.xbrl.CExplicitFormulaManager", p))
            {
                Poco::ScopedLock<Poco::Mutex> g(_lock);
                if (!FC.TrySPGetFValue("se.xbrl.CExplicitFormulaManager", p))
                {
                    RETURNIFFAILED(CObject<CExplicitFormulaManager>::Create(ps, &p));
                    FC.SPSetFValue("se.xbrl.CExplicitFormulaManager", p, 3600);
                    *ppOut = p;
                    return S_OK;
                }
            }
            p->AddRef();
            *ppOut = p;
            return S_OK;
        }

        CExplicitFormulaManager::CExplicitFormulaManager(ISession* ps) : CObjectRoot(ps)
        {
        }

        CExplicitFormulaManager::~CExplicitFormulaManager() 
        {
            
        }

        long unsigned int CExplicitFormulaManager::Run(CExplicitFormulaManagerState* s, IFinancialsConsolidated*  pcon, std::string funcName, I16 year, double* outValue) 
        {
            _pcon = pcon;
            return Run(s, pcon, funcName, year, 0, outValue);
        }

        long unsigned int CExplicitFormulaManager::Run(CExplicitFormulaManagerState* s, IFinancialsConsolidated* pcon, std::string funcName, I16 year, I16 quarter, double* outValue) 
        {
            _pcon = pcon;
            InitFunctions();
            FuncMap::iterator itfind;
            if ( (itfind = _map.find(funcName)) != _map.end() )
            {
                return itfind->second(s, year, quarter, outValue);
            }
            return E_NotFound;
        }
        
        void CExplicitFormulaManager::InitFunctions() 
        {
            if (!_map.size())
            {
                using namespace std::placeholders;
                PerformanceFunction func = std::bind(&CExplicitFormulaManager::RevenueGrowth, this, _1, _2, _3, _4) ;
                _map.insert(make_pair("RevenueGrowth", func ));
                func = std::bind(&CExplicitFormulaManager::QuarterRevenueWeight, this, _1, _2, _3, _4) ;
                _map.insert(make_pair("RevenueQuarterWeight", func ));
                func = std::bind(&CExplicitFormulaManager::CompetitionCorelation, this, _1, _2, _3, _4) ;
                _map.insert(make_pair("CompetitionCorelation", func ));
            }
        }

        
        ///////////////////////////////// Explicit Methods /////////////////////////////////////
/*
        long unsigned int CExplicitFormulaManager::QuarterRevenueWeight(CExplicitFormulaManagerState* s, short year, short quarter, double* outValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("CExplicitFormulaManager::QuarterRevenueWeight");
            
            // get record back
            OmniType orec = s->get_Property("R");
            if (orec.IsEmpty())
            {
                auto itfind = _xmatches.find(25);
                if (itfind == _xmatches.end())
                {
                    SP<IXbrlIdList> pxidlist;
                    SP<IXbrl> pxbrl;
                    RETURNIFFAILED(XbrlOpen(&pxbrl));
                    RETURNIFFAILED(pxbrl->GetMatchAccounts((UID)25, &pxidlist, _pcon->get_Cik()));
                    _xmatches.insert(make_pair(25, pxidlist));
                }            
                IXbrlIdList* pxidlist = _xmatches.at(25);
                SP<IXbrlRecord> prec;
                for(UID u : *pxidlist)
                {
                    if (ISOK(_pcon->get_Record(u, &prec)) && prec)
                    {
                        // ensure that we have the right node.
                        break;
                    }
                }
                
                orec = prec;
                s->set_Property("R", (IObject*)orec);
            }
            
            IXbrlRecord* prec = static_cast<IXbrlRecord*>((IObject*)orec);
            
            if (!prec)
                return E_NotFound;
            
            // this formula is restricted to full years. So we will grab CFinancialRatio from 
            // state object and set quarterly values directly.
            CFinancialRatio* pRatio = static_cast<CFinancialRatio*>((POINTER) s->get_Property("FinancialRatio"));
            
            typedef std::vector<I16> EmptyYears;
            I16 fyear = prec->get_LowerYear();
            I16 cyear = prec->get_HighestYear();
            R8 tq[4];
            int tc[4];
            memset(tq, 0, sizeof(R8) * 4);
            memset(tc, 0, sizeof(int) * 4);
            EmptyYears yearsempty;
            
            for (I16 y = fyear; y <= cyear; ++y)
            {
                SP<IXbrlField> pfld;
                RETURNIFFAILED(prec->get_Financials(y, &pfld));
                const OmniType& ofyv = pfld->get_FY();
                if (!ofyv.IsEmpty())
                {
                    // get quarters
                    R8 rqt = 0.0;
                    for (int q = 1; q <= 4; ++q)
                    {
                        if (y == cyear && 
                                q > _pcon->get_LatestQuarter())
                            goto STEP2;
                        
                        const OmniType& oqv = pfld->get_Q(q);
                        if (oqv.IsEmpty())
                        {
                            yearsempty.push_back(y);
                            goto END;
                        }
                        rqt = (R8)oqv/(R8)ofyv * 100;
                        pRatio->set_Value(y, q, rqt);
                        tq[q-1] += rqt;
                        tc[q-1] += 1;
                    }
                    pRatio->set_Value(y, 100.0);
                }
                else
                {
                    yearsempty.push_back(y);
                }
            END:
                continue;
            }
            
            yearsempty.push_back(cyear + 1);
            
        STEP2:
            
            for (int i=0; i!=4; ++i)
            {
                tq[i] = tq[i] / tc[i];
            }
            
            for (I16 emptyYear : yearsempty)
            {
                pRatio->set_Value(emptyYear, 100.0);
                for (int i=0; i != 4; ++i)
                {
                    pRatio->set_Value(emptyYear, i+1, tq[i]);
                }
            }
            
        RETURNFUNC:
            return S_CalcCompleted;
        }
*/
        // below is the alternative to the above using stddev
       long unsigned int CExplicitFormulaManager::QuarterRevenueWeight(CExplicitFormulaManagerState* s, short year, short quarter, double* outValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("CExplicitFormulaManager::QuarterRevenueWeight");
            
            // get record back
            OmniType orec = s->get_Property("R");
            if (orec.IsEmpty())
            {
                auto itfind = _xmatches.find(25);
                if (itfind == _xmatches.end())
                {
                    SP<IXbrlIdList> pxidlist;
                    SP<IXbrl> pxbrl;
                    RETURNIFFAILED(XbrlOpen(get_Session(),  &pxbrl));
                    RETURNIFFAILED(pxbrl->GetMatchAccounts((UID)25, &pxidlist, _pcon->get_Cik()));
                    _xmatches.insert(make_pair(25, pxidlist));
                }            
                IXbrlIdList* pxidlist = _xmatches.at(25);
                SP<IXbrlRecord> prec;
                for(UID u : *pxidlist)
                {
                    if (ISOK(_pcon->get_Record(u, &prec)) && prec)
                    {
                        // ensure that we have the right node.
                        break;
                    }
                }
                
                orec = prec;
                s->set_Property("R", (IObject*)orec);
            }
            
            IXbrlRecord* prec = static_cast<IXbrlRecord*>((IObject*)orec);
            
            if (!prec)
                return E_NotFound;
            
            // this formula is restricted to full years. So we will grab CFinancialRatio from 
            // state object and set quarterly values directly.
            CFinancialRatio* pRatio = static_cast<CFinancialRatio*>((POINTER) s->get_Property("FinancialRatio"));
            
            typedef std::vector<I16> EmptyYears;
            I16 fyear = prec->get_LowerYear();
            I16 cyear = prec->get_HighestYear();
            R8 tq[4];
            int tc[4];
            memset(tq, 0, sizeof(R8) * 4);
            memset(tc, 0, sizeof(int) * 4);
            EmptyYears yearsempty;
            
            for (I16 y = fyear; y <= cyear; ++y)
            {
                SP<IXbrlField> pfld;
                RETURNIFFAILED(prec->get_Financials(y, &pfld));
                const OmniType& ofyv = pfld->get_FY();
                if (!ofyv.IsEmpty())
                {
                    // get quarters
                    R8 rqt = 0.0;
                    for (int q = 1; q <= 4; ++q)
                    {
                        if (y == cyear && 
                                q > _pcon->get_LatestQuarter())
                            goto STEP2;
                        
                        const OmniType& oqv = pfld->get_Q(q);
                        if (oqv.IsEmpty())
                        {
                            yearsempty.push_back(y);
                            goto END;
                        }
                        rqt = (R8)oqv/(R8)ofyv * 100;
                        pRatio->set_Value(y, q, rqt);
                        tq[q-1] += rqt;
                        tc[q-1] += 1;
                    }
                    pRatio->set_Value(y, 100.0);
                }
                else
                {
                    yearsempty.push_back(y);
                }
            END:
                continue;
            }
            
            yearsempty.push_back(cyear + 1);
            
        STEP2:
            
            for (int i=0; i!=4; ++i)
            {
                tq[i] = tq[i] / tc[i];
            }
            
            for (I16 emptyYear : yearsempty)
            {
                pRatio->set_Value(emptyYear, 100.0);
                for (int i=0; i != 4; ++i)
                {
                    pRatio->set_Value(emptyYear, i+1, tq[i]);
                }
            }
            
        RETURNFUNC:
            return S_CalcCompleted;
        }
        
        long unsigned int CExplicitFormulaManager::RevenueGrowth(CExplicitFormulaManagerState* s, short year, short quarter, double* outValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("CExplicitFormulaManager::RevenueGrowth");
            
            // get record back
            OmniType orec = s->get_Property("R");
            if (orec.IsEmpty())
            {
                auto itfind = _xmatches.find(25);
                if (itfind == _xmatches.end())
                {
                    SP<IXbrlIdList> pxidlist;
                    SP<IXbrl> pxbrl;
                    RETURNIFFAILED(XbrlOpen(get_Session(), &pxbrl));
                    RETURNIFFAILED(pxbrl->GetMatchAccounts((UID)25, &pxidlist, _pcon->get_Cik()));
                    _xmatches.insert(make_pair(25, pxidlist));
                }            
                IXbrlIdList* pxidlist = _xmatches.at(25);
                SP<IXbrlRecord> prec;
                for(UID u : *pxidlist)
                {
                    if (ISOK(_pcon->get_Record(u, &prec)) && prec)
                    {
                        // ensure that we have the right node.
                        break;
                    }
                }
                
                orec = prec;
                s->set_Property("R", (IObject*)orec);
            }
            
            IXbrlRecord* prec = static_cast<IXbrlRecord*>((IObject*)orec);
            
            if (!prec)
                return E_NotFound;
            
            //SP<IXbrlRecord> prec;
            //RETURNIFFAILED(_pcon->get_Record(20629, &prec));

            // requirement is to get the difference between given year and the previous year (or quarters)
            // and get a percentage.
            SP<IXbrlField> pfld;
            if (quarter == 0)
            {
                RETURNIFFAILED(prec->get_Financials(year, &pfld));
                OmniType oyr1 = pfld->get_FY();
                
               // LOGI(_logger, "Revenue for " << year <<  oyr1);
                
                R8 yr1 = oyr1.IsEmpty() ? ER8 : (R8)oyr1;
                pfld.Clear();
                year -= 1;
                RETURNIFFAILED(prec->get_Financials(year, &pfld));
                
                OmniType oyr0 = pfld->get_FY();
                
               // LOGI(_logger, "Revenue for " << year <<  oyr0);
                
                R8 yr0 = oyr0.IsEmpty() ? ER8 : (R8)oyr0;
                if (oyr0.IsEmpty() && oyr1.IsEmpty())
                    *outValue = ER8;
                else if (oyr1.IsEmpty())
                    *outValue = ER8;
                else if (oyr0.IsEmpty())
                    *outValue = ER8;
                else
                    *outValue = 100 * (yr1 - yr0)/ yr0;
            }
            else
            {
                //bool isProgression = prec->get_IsProgression();
                RETURNIFFAILED(prec->get_Financials(year, &pfld));
                OmniType oq1 = pfld->get_Q(quarter);
                if (quarter == 1)
                {
                    year -= 1;
                    quarter = 4;
                }
                else
                    quarter -= 1;
                pfld.Clear();
                RETURNIFFAILED(prec->get_Financials(year, &pfld));
                OmniType oq0 = pfld->get_Q(quarter);
                
                R8 q1 = oq1.IsEmpty() ? ER8 : (R8)oq1;
                R8 q0 = oq0.IsEmpty() ? ER8 : (R8)oq0;
                if (oq0.IsEmpty() && oq1.IsEmpty())
                    *outValue = ER8;
                else if (oq1.IsEmpty())
                    *outValue = ER8;
                else if (oq0.IsEmpty())
                    *outValue = ER8;
                else
                    *outValue = 100 * (q1 - q0)/q0;
            }
            return S_OK;
        }
        
        sql_create_5(se_fin_perf_competioncovariance, 4, 5, 
                sql_bigint, primary_product_id, 
                sql_bigint, secondary_product_id, 
                sql_smallint, year,
                sql_smallint, quarter, 
                sql_decimal, variance);

        long unsigned int CExplicitFormulaManager::CompetitionCorelation(CExplicitFormulaManagerState*, short year, short quarter, double* outValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("CExplicitFormulaManager::CompetitionCorelation");
            
            // TODO : REMOVE THIS WHEN FUNCTION IS COMPLETED.
            return E_NotImpl;

            typedef std::map<DATAPERIOD, R8> Covar;
            typedef std::vector<se_fin_perf_competioncovariance> VarianceData;

            try
            {
                SP<IXbrl> pxbrl;
                RETURNIFFAILED(XbrlOpen(get_Session(), &pxbrl));

                CSmartConnection conn(CONN);
                Query q = conn->query();
                try
                {
                    auto fnormalize = [](Covar& data)
                    {
                    };

                    auto fsecnormalize = [](Covar& primdata, Covar& secdata)
                    {

                    };

                    CIK primcik = _pcon->get_Cik();
                    UID prodId = pxbrl->GetProductId(primcik);
                    CDate dtNow = CDate::Now();
                    q << "select * from se_fin_perf_competioncovariance ";
                    q << "where primary_product_id = " << prodId;
                    q << " and year = " << dtNow.get_Year();
                    q << " and quarter = " << dtNow.get_Quarter();
                    if (!q.store().size())
                    {

                        CPersistPerformanceData perfPersist(get_Session());
                        // run performance ratio persistance for cik
                        if (FAILED(perfPersist.RunForCik(primcik, 21)))
                        {
                           // LOGE(_logger, "Error while attempting to persist performance ratio 21 for CIK " << primcik);
                            return E_NoDataToReturn;
                        }

                        q = conn->query();
                        q << "call upd_LatestPerfRatioValuesCikParam(" << primcik << ")";
                        q.exec();
                        while (q.more_results())
                            q.store_next();

                        SP<se::training::ITrainingTable> pPrimTable;
                        RETURNIFFAILED(pxbrl->GetPerformanceTrainingData(primcik, 21, DATAPERIOD(), DATAPERIOD(), &pPrimTable));
                        R8 avg;
                        RETURNIFFAILED(pPrimTable->get_GMean(false, &avg));
                        Covar prmVar;
                        for (se::training::ITrainingRecord* p : *pPrimTable)
                        {
                            R8 val = (R8)p->get_Value();
                            prmVar.insert(make_pair(p->get_Period(), val - avg ));
                        }

                        fnormalize(prmVar);

                        q << "call list_CompetitionsSic(" << primcik << ")";
                        StoreQueryResult sqr = q.store();
                        while (q.more_results())
                            q.store_next();

                        for (const Row& r : q.store())
                        {
                            CIK seccik = (CSTR)r["cik"];
                            // run performance ratio persistance for cik
                            if (FAILED(perfPersist.RunForCik(seccik, 21)))
                            {
                               // LOGE(_logger, "Error while attempting to persist performance ratio 21 for CIK " << seccik);
                                continue;
                            }
                            q = conn->query();
                            q << "call upd_LatestPerfRatioValuesCikParam(" << seccik << ")";
                            q.exec();
                            while (q.more_results())
                                q.store_next();

                            SP<se::training::ITrainingTable> pSecTable;
                            RETURNIFFAILED(pxbrl->GetPerformanceTrainingData(seccik, 21, DATAPERIOD(), DATAPERIOD(), &pSecTable));
                            Covar secVar;
                            avg = 0.0;
                            RETURNIFFAILED(pSecTable->get_GMean(false, &avg));
                            for (se::training::ITrainingRecord* p : *pSecTable)
                            {
                                R8 val = (R8)p->get_Value();
                                secVar.insert(make_pair(p->get_Period(), val - avg));
                            }
                            fsecnormalize(prmVar, secVar);
                            VarianceData variances;
                            // record variance data 
                            for (auto pr : prmVar)
                            {
                                R8 priVal = pr.second;
                                const DATAPERIOD& dp = pr.first;
                                // grab data for the secondary company
                                R8 secVal = secVar.at(dp);
                                se_fin_perf_competioncovariance rec(prodId, (UID)r["product_id"], dp.Year, dp.Quarter, priVal * secVal);
                                variances.push_back(rec);
                            }

                            // insert
                            q = conn->query();
                            q.replace(variances.begin(), variances.end()).execute();
                        }                    
                    }
                    return S_CalcCompleted;
                }
                catch(...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
            }
            CATCHCLAUSE("CompetitionCorelation");
        }
    }
}