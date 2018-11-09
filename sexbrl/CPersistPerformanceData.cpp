/* 
 * File:   CPersistPerformanceData.cpp
 * Author: santony
 * 
 * Created on February 20, 2014, 5:36 PM
 */
#include <limits>

#include "sexbrlinternal.h"
#include "CPersistPerformanceData.h"
#include "../seglobal/exceptions.h"
#include "../semain/semain.h"
#include <Poco/Semaphore.h>
#include <Poco/Condition.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

#define SECONN CSEConnections::GetMain()

#define SINGLETHREAD

namespace se
{
    namespace xbrl
    {


        CPersistPerformanceData::CPersistPerformanceData(ISession* ps) :
            _ps(ps)
        {
            
        }

        CPersistPerformanceData::~CPersistPerformanceData() 
        {
            
        }
        
        struct RunCikTask : public CObjectRoot, public se::threading::ITask
        {
            string _cik;
            CPersistPerformanceData* _this;
            Poco::Semaphore* _sph;

            ~RunCikTask()
            {
                _sph->set();
            }
            

            virtual long unsigned int Init(const string& cik, Poco::Semaphore* sm, CPersistPerformanceData* thisptr)
            {
                _cik = cik;
                _this = thisptr;
                _sph = sm;
                return S_OK;
            }

            
            virtual void Execute()
            {
                _this->RunForCik(_cik.c_str());
            }

        };

#ifndef SINGLETHREAD
        
        SE_CODE CPersistPerformanceData::Run() 
        {
            try
            {
                
                const string findexmarker  = "performancelastindex.txt";
                // open file for writing last successful index
                string line;
                std::ifstream fin(findexmarker);
                if (fin.is_open())
                {
                    getline(fin, line);
                    fin.close();
                }
                
                int startPosition = 0;
                if (!line.empty())
                    startPosition = strtol(line.c_str(), nullptr, 10);
                
                std::ofstream fout(findexmarker);
                if (!fout.is_open())
                {
                   // LOGEP("performancelastindex.txt file cannot be opened. Cannot write last successful index.");
                }
                
                
                SP<IApp> pApp;
                RETURNIFFAILED(SEOpenApp(&pApp));
                SP<se::threading::IThreadPool> pThp;
                RETURNIFFAILED(pApp->get_ThreadPool(se::threading::TPT_ParallelPool, &pThp));
               // LOGIP("Beginning to process performance data for all companies.");
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "select * from list_ListedCompanies order by product_symbol";
                StoreQueryResult sqr = q.store();
                
                // create an instance of semaphore to allow five tasks to run at any time.
                Poco::Semaphore *sph = new Poco::Semaphore(3, 3);
                for (int i=startPosition; i != sqr.size(); ++i)
                {
                    // record current index
                    fout.seekp(0, ios::beg);
                    fout << i << endl;
                    const Row& r = sqr[i];
                    string cik = (CSTR)r["cik"];
                    {
                        SP<RunCikTask> task;
                        CObject<RunCikTask>::Create(get_Session(), cik, sph, this, &task);
                        pThp->RunTask(task);
                    }
                    sph->wait();
                }
                
                sleep(10);

                delete sph;
                
                // delete the
                fout.close();
                remove(findexmarker.c_str());
                
               // LOGIP("Successfully completed performance data processing for all companies.");
                return S_OK;
            }
            CATCHCLAUSE("Run")
        }

#else
        
        SE_CODE CPersistPerformanceData::Run() 
        {
            try
            {
               // LOGIP("Beginning to process performance data for all companies.");
                
                const string findexmarker  = "performancelastindex.txt";
                // open file for writing last successful index
                string line;
                std::ifstream fin(findexmarker);
                if (fin.is_open())
                {
                    getline(fin, line);
                    fin.close();
                }
                
                int startPosition = 0;
                if (!line.empty())
                    startPosition = strtol(line.c_str(), nullptr, 10);
                
                std::ofstream fout(findexmarker);
                if (!fout.is_open())
                {
                   // LOGEP("performancelastindex.txt file cannot be opened. Cannot write last successful index.");
                }
                
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "select * from list_ListedCompanies order by product_symbol";
                StoreQueryResult sqr = q.store();
                //for (const Row& r : sqr)
                for (int i=startPosition; i != sqr.size(); ++i)
                {
                    // record current index
                    fout.seekp(0, ios::beg);
                    fout << i << endl;
                    
                    const Row& r = sqr[i];
                   // LOGI(_logger, "Ratio values are being calculated for " << r["product_symbol"]);
                    string cik = (CSTR)r["cik"];
                    SE_CODE retCode= RunForCik(cik.c_str());
                    if (FAILED(retCode))
                    {
                       // LOGE(_logger, "Failed persist performance ratios for CIK " << cik << "/Symbol " << r["product_symbol"]);
                    }
                    else
                    {
                       // LOGI(_logger, "Ratio values are calculated for " << r["product_symbol"]);
                    }
                }
               // LOGIP("Successfully completed performance data processing for all companies.");
                
                // delete the
                fout.close();
                remove(findexmarker.c_str());
                
                return S_OK;
            }
            CATCHCLAUSE("Run")
        }
        
#endif

        long unsigned int CPersistPerformanceData::RunForCik(IFinancialsConsolidated* pcon, unsigned long perfId) 
        {
            RTRY
            
                CIK cik = pcon->get_Cik();
                    
               // LOGI(_logger, "Beginning to process and save performance data for " << cik);
                if (!perfId)
                {
                    SP<IFinancialPerformance> pfin;
                    SE_CODE retCode = pcon->get_Performance(&pfin);
                    if (FAILED(retCode))
                    {
                       // LOGE(_logger, "Failed to obtain financial performance instance for cik " << cik);
                        return retCode;
                    }
                
                    SP<IFinancialRatioList> pratios;
                    retCode = pfin->get_Ratios(&pratios);
                    if (FAILED(retCode))
                    {
                       // LOGE(_logger, "Failed to obtain financial ratio list for cik " << cik);
                        return retCode;
                    }

                   // LOGI(_logger, "Received a list of performance ratio values from performance/consolidated financial instance for cik " << cik);

                    /* things to do here */
                    /*
                     * 1. Run the below loop
                     * 2. Check to see if a record exist for CIK + perf_id in table se_fin_perf_cik_ratio
                     * 3. if not, insert a record and obtain primary key
                     * 4. if yes, then capture the primary key.
                     * 5. now using primary key from step 3 or 4, look for a record in se_fin_perf_ratio_values for 
                     *      each year and year-quarter combination.
                     * 6. if a record exists, update the preformance value.
                     * 7. if not, insert records for all year and year-quarter combinations.
                     * 8 .task completed.
                     */
                    CSmartConnection conn(SECONN);
                    for (IFinancialRatio* pratio : *pratios)
                    {
                        ProcessRatio(conn, cik, pratio);
                       // LOGI(_logger, "Completed with persisting perfratio " << pratio->get_Id() << " for cik " << cik);
                    }
                   // LOGI(_logger, "Completed with persisting perfratios for cik " << cik);
                    return S_OK;
                }
                else
                {
                    SP<IFinancialRatio> pratio;
                    RETURNIFFAILED(pcon->get_PerformanceRatio(perfId, &pratio));
                    CSmartConnection conn(SECONN);
                    return ProcessRatio(conn, cik, pratio);
                }            
            
            RCATCH("RunForCik(ifinancialconsolidated, perfid)")
        }

        
        long unsigned int CPersistPerformanceData::RunForCik(CIK cik, UID perfId) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CPersistPerformanceData::RunForCik(cik, perfid)");
            try
            {
                SP<IXbrl> pxbrl;
                THROWIFFAILED(XbrlOpen(_ps,  &pxbrl), "Failed to open xbrl api");
                SP<IFinancialsConsolidated> pcon;
                LOGRETURNIFFAILED(pxbrl->GetConsolidatedFinancials(cik, &pcon, true), "Error getting consolidated finance instance.");
                return RunForCik(pcon, perfId);
            }
            //CATCHCLAUSE("RunForCik")
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Error persisting perf-ratios for CIK " << cik << ". Reported error : " << ex.what());
                return E_DBUpdateError;
            }
        }

        long unsigned int CPersistPerformanceData::ProcessRatio(CSmartConnection& conn, CIK cik, IFinancialRatio* pratio) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CPersistPerformanceData::ProcessRatio");
            
            try
            {
                Query q = conn->query();
                q << "select * from  se_fin_perf_cik_ratio where cik = " << cik << " and perf_id = " << pratio->get_Id();
                StoreQueryResult sqr = q.store();
                UID key = 0;
                if (sqr.size())
                {
                     key = sqr[0]["cik_perf_id"];
                   // LOGI(_logger, "Has a record in se_fin_perf_cik_ratio table for cik " << cik << " and ratio id " << pratio->get_Id());
                }
                else
                {
                    q = conn->query();
                    q << "insert into se_fin_perf_cik_ratio (cik, perf_id) values ( " << cik << ", " << pratio->get_Id() << ")";
                    q.execute();
                    key = q.insert_id();
                   // LOGI(_logger, "Inserted a new record in se_fin_perf_cik_ratio table for cik " << cik << " and ratio id " << pratio->get_Id());
                }
#ifdef __DEBUG__
                if (pratio->get_Id() == 21)
                {
                    string s = "break";
                }
#endif
               // LOGI(_logger, "Going to insert/update of performance ratio values in se_fin_perf_ratio_values for cik " << cik << " and ratio id " << pratio->get_Id());
                I16 ceilYear = pratio->get_CeilYear();
                for (I16 year = pratio->get_FloorYear(); year <= ceilYear; ++year)
                {

#ifdef __DEBUG__
                if (year == 2014)
                {
                    string s = "break";
                }
                
                if (pratio->get_Id() == 21 && year == 2014)
                {
                    string s = "break";
                }
#endif

                    R8 dValue = pratio->get_Value(year);
                    I16 lastQ = pratio->get_LastReportedQuarter();
                    UpdatePerfValue(conn, key, year, 0, dValue);
                    for (I16 q = 1; q <= 4; ++q)
                    {
                        if (year == ceilYear && q > lastQ)
                            break;
                        
                        dValue = pratio->get_Value(year, q);
                        UpdatePerfValue(conn, key, year, q, dValue);
                    }
                }
               // LOGI(_logger, "Completed insert/update of performance ratio values in se_fin_perf_ratio_values for cik " << cik << " and ratio id " << pratio->get_Id());
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Error : " << ex.what());
                return E_DBUpdateError;
            }
        }
        

        void CPersistPerformanceData::UpdatePerfValue(CSmartConnection& conn, UID key, short year, short quarter, double newValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CPersistPerformanceData::UpdatePerfValue");

           // LOGI(_logger, "UpdatePerfValue: new value is " << newValue << " for year " << year << " and quarter " << quarter);
#ifdef __DEBUG__
           // if (newValue == ER8 || newValue < -1000000000 || newValue > 1000000000 )
               // LOGIP("This value will be rejected and won't be saved in the db.");
#endif
            Query q = conn->query();
            q << "select perf_value from se_fin_perf_ratio_values where cik_perf_id = " << key ;
            q << " and year = " << year << " and quarter = " << quarter;
           // LOGI(_logger, q.str());
            StoreQueryResult sqr1 = q.store();
            if (sqr1.size())
            {
                const Row& rw = sqr1[0];
                sql_double_null rvalue = rw[0];
                R8 dvalue = rvalue.is_null ? dvalue = 0.0 : rvalue.data;
                
                // round values to 4 decimals
                I64 ival = dvalue * pow(10, 2);
                I64 inval = newValue * pow(10, 2);
                if (ival != inval)
                {
                    q = conn->query();
                    if (newValue != ER8 && (newValue > -1000000000 && newValue < 1000000000) )
                    {
                        q << "update se_fin_perf_ratio_values set perf_value = " << newValue ;
                        q << " where cik_perf_id = " << key ;
                        q << " and year = " << year << " and quarter = " << quarter;
                       // LOGI(_logger, q.str());
                    }
                    else
                    {
//                        q << "delete from se_fin_perf_ratio_values where cik_perf_id = " << key;
//                        q << " and year = " << year << " and quarter = " << quarter;
                        q << "update se_fin_perf_ratio_values set perf_value = NULL" ;
                        q << " where cik_perf_id = " << key ;
                        q << " and year = " << year << " and quarter = " << quarter;
                       // LOGI(_logger, q.str());
                    }
                    q.execute();
                }
            }
            else
            {
                if (newValue != ER8 && (newValue > -1000000000 && newValue < 1000000000) )
                {
                    q = conn->query();
                    q << "insert into se_fin_perf_ratio_values (cik_perf_id, year, quarter, perf_value) " ;
                    q << "values (" << key << ", " << year << ", " << quarter << ", " << newValue << ")";
                   // LOGI(_logger, q.str());
                    q.execute();
                }
            }
        }
        
        void CPersistPerformanceData::UpdatePerfPredValue(CSmartConnection& conn, UID key, short year, short quarter, double newValue) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CPersistPerformanceData::UpdatePerfPredValue");

           // LOGI(_logger, "UpdatePerfPredValue: new value is " << newValue << " for year " << year << " and quarter " << quarter);
#ifdef __DEBUG__
            //if (newValue == ER8 || newValue < -1000000000 || newValue > 1000000000 )
               // LOGIP("This value will be rejected and won't be saved in the db.");
#endif
            Query q = conn->query();
            q << "select perf_pred_value from se_fin_perf_ratio_values where cik_perf_id = " << key ;
            q << " and year = " << year << " and quarter = " << quarter;
           // LOGI(_logger, q.str());
            StoreQueryResult sqr1 = q.store();
            if (sqr1.size())
            {
                const Row& rvalue = sqr1[0];
                sql_double_null dvalue = rvalue[0];
                if (dvalue.is_null)
                {
                    if (newValue != ER8 && (newValue > -1000000000 && newValue < 1000000000) )
                    {
                        q = conn->query();
                        q << "update se_fin_perf_ratio_values set perf_pred_value = " << newValue ;
                        q << " where cik_perf_id = " << key ;
                        q << " and year = " << year << " and quarter = " << quarter;
                       // LOGI(_logger, q.str());
                        q.execute();
                    }
                }
                else
                {
                    // round values to 4 decimals
                    I64 ival = dvalue.data * pow(10, 2);
                    I64 inval = newValue * pow(10, 2);
                    if (ival != inval)
                    {
                        q = conn->query();
                        if (newValue != ER8 && (newValue > -1000000000 && newValue < 1000000000) )
                        {
                            q << "update se_fin_perf_ratio_values set perf_pred_value = " << newValue ;
                            q << " where cik_perf_id = " << key ;
                            q << " and year = " << year << " and quarter = " << quarter;
                           // LOGI(_logger, q.str());
                        }
                        else
                        {
                            q << "update se_fin_perf_ratio_values set perf_pred_value = null ";
                            q << " where cik_perf_id = " << key ;
                            q << " and year = " << year << " and quarter = " << quarter;
                           // LOGI(_logger, q.str());
                        }
                        q.execute();
                    }
                }
            }
            else
            {
                if (newValue != ER8 && (newValue > -1000000000 && newValue < 1000000000) )
                {
                    q = conn->query();
                    q << "insert into se_fin_perf_ratio_values (cik_perf_id, year, quarter, perf_pred_value) " ;
                    q << "values (" << key << ", " << year << ", " << quarter << ", " << newValue << ")";
                   // LOGI(_logger, q.str());
                    q.execute();
                }
            }
        }        
    }
}

