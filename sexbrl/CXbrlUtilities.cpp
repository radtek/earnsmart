/* 
 * File:   CXbrlUtilities.cpp
 * Author: santony
 * 
 * Created on April 2, 2014, 2:25 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlUtilities.h"
#include "../seglobal/exceptions.h"

#define SEDBCONN    CSEConnections::GetMain()

using namespace se::data;

namespace se
{
    namespace xbrl
    {

        string const CXbrlUtilities::GetInstanceTableName(U64 cik) 
        {
            CSmartConnection conn(SEDBCONN);
            return GetInstanceTableName(conn, cik);
        }

        string const CXbrlUtilities::GetInstanceCtxtTableName(U64 cik) 
        {
            CSmartConnection conn(SEDBCONN);
            return GetInstanceCtxtTableName(conn, cik);
        }
        
        void CXbrlUtilities::CreateInstTablesIfNotFound(ICIK cik)
        {
            CSmartConnection conn(SEDBCONN);
            auto q = conn->query();
            q << "call create_InstTables(" << cik << ");";
            q.exec();
            while (q.more_results())
                q.store_next();
        }

        
        U64 CXbrlUtilities::GetCikFromRecordId(U64 recordId)
        {
            CSmartConnection conn(SEDBCONN);
            return GetCikFromRecordId(conn, recordId);
        }
        
        const string CXbrlUtilities::GetInstanceTableName(CSmartConnection& conn, unsigned long cik)
        {
            Query q = conn->query();
            q << "SELECT `sa`.`get_FinInstanceTableName`(" << cik << ");";
            auto qs = q.store();
            if (qs.size() > 0)
                return qs[0][0].c_str();
            return "";
        }

        const string CXbrlUtilities::GetInstanceCtxtTableName(CSmartConnection& conn, unsigned long cik)
        {
            Query q = conn->query();
            q << "SELECT `sa`.`get_FinInstCtxtTableName` (" << cik << ");";
            auto qs = q.store();
            if (qs.size() > 0)
                return qs[0][0].c_str();
            return "";
}

        unsigned long CXbrlUtilities::GetCikFromRecordId(CSmartConnection& conn, unsigned long recordId)
        {
            Query q = conn->query();
            q << "SELECT cik FROM sa.se_edgar_formrecs where rec_id = " << recordId << ";";
            auto qs = q.store();
            if (qs.size() > 0)
                return (U64)qs[0][0];
            return 0;
        }
        
        long unsigned int CXbrlUtilities::PopulatePerformanceList(ISession* ps, const string& sql, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query(sql);
                return IStoreQueryResult::Create(ps, q.store(), ppResult);
            }
            CATCHCLAUSE("PopulatePerformanceList")
        }
        

        long unsigned int CXbrlUtilities::FilterPerformanceRatiosByRange(ISession* ps, short year, short quarter, unsigned long ratioId, double start, double end, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            if (year < 2000)
            {
               // LOGEP("Invalid year parameter. Must be a value above 2000");
                return E_InvalidArg;
            }
            
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }

            if (start > end)
            {
               // LOGEP("Invalid range parameters. start must be lower than end.");
                return E_InvalidArg;
            }
            
            ostringstream ostr;
            ostr << "SELECT cik, perf_id, year, quarter FROM sa.list_PerfRatioValuesSlim where";
            ostr << " year = " << year;
            ostr << " and quarter = " << quarter ;
            ostr << " and perf_id = " << ratioId;
            ostr << " and perf_value between " << start << " and " << end;
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);
            
        }
        

        long unsigned int CXbrlUtilities::FilterPerformanceLatestAnnualRatiosByRange(ISession* ps, unsigned long ratioId, double start, double end, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }

            if (start > end)
            {
               // LOGEP("Invalid range parameters. start must be lower than end.");
                return E_InvalidArg;
            }
            
            ostringstream ostr;
            ostr << "SELECT cik, perf_id, max(year) as year FROM sa.list_PerfRatioValuesSlim";
            ostr << " where perf_id = " << ratioId ;
            ostr << " and quarter = 0 ";
            ostr << " and perf_value between " << start << " and " << end;
            ostr << " group by cik, perf_id";
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);                    

        }

        long unsigned int CXbrlUtilities::FilterPerformanceLatestQuarterlyRatiosByRange(ISession* ps, unsigned long ratioId, double start, double end, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }

            if (start > end)
            {
               // LOGEP("Invalid range parameters. start must be lower than end.");
                return E_InvalidArg;
            }
            
            ostringstream ostr;
            ostr << "SELECT cik, perf_id, max(year) as year, max(quarter) as quarter";
            ostr << " FROM sa.list_PerfRatioValuesSlim";
            ostr << " where perf_id = " << ratioId;
            ostr << " and quarter <> 0 ";
            ostr << " and perf_value between " << start << " and " << end;
            ostr << " group by cik, perf_id";
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);                    

        }

        long unsigned int CXbrlUtilities::FilterPerformanceLatestAnnualRatiosByValue(ISession* ps, unsigned long ratioId, double value, double tolerance, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }
            
            if (tolerance > 100)
            {
               // LOGEP("Invalid tolerance parameter. Must be a value between 0 and 1 or between 0 and 100.");
                return E_InvalidArg;
            }
            
            R8 start, end;
            R8 toleratedvalue = (value * tolerance > 1 ? tolerance/100 : tolerance);
            start = value - toleratedvalue;
            end = value + toleratedvalue;

            ostringstream ostr;
            ostr << "SELECT cik, perf_id, max(year) as year FROM sa.list_PerfRatioValuesSlim";
            ostr << " where perf_id = " << ratioId ;
            ostr << " and quarter = 0 ";
            ostr << " and perf_value between " << start << " and " << end;
            ostr << " group by cik, perf_id";
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);                    
            
        }
        

        long unsigned int CXbrlUtilities::FilterPerformanceLatestQuarterlyRatiosByValue(ISession* ps, unsigned long ratioId, double value, double tolerance, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }
            
            if (tolerance > 100)
            {
               // LOGEP("Invalid tolerance parameter. Must be a value between 0 and 1 or between 0 and 100.");
                return E_InvalidArg;
            }
            
            R8 start, end;
            R8 toleratedvalue = (value * tolerance > 1 ? tolerance/100 : tolerance);
            start = value - toleratedvalue;
            end = value + toleratedvalue;

            ostringstream ostr;
            ostr << "SELECT cik, perf_id, max(year) as year, max(quarter) as quarter";
            ostr << " FROM sa.list_PerfRatioValuesSlim";
            ostr << " where perf_id = " << ratioId;
            ostr << " and quarter <> 0 ";
            ostr << " and perf_value between " << start << " and " << end;
            ostr << " group by cik, perf_id";
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);                    
            
            
        }




        long unsigned int CXbrlUtilities::FilterPerformanceRatiosByValue(ISession* ps, short year, short quarter, unsigned long ratioId, double value, double tolerance, IStoreQueryResult** ppResult) 
        {
            //LoggerPtr _logger = Logger::getLogger("se.xbrl.cxbrlutilities");
            
            if (year < 2000)
            {
               // LOGEP("Invalid year parameter. Must be a value above 2000");
                return E_InvalidArg;
            }
            
            if (ratioId == 0)
            {
               // LOGEP("Invalid ratioid parameter. Must be a value from ratios table.");
                return E_InvalidArg;
            }
            
            if (tolerance > 100)
            {
               // LOGEP("Invalid tolerance parameter. Must be a value between 0 and 1 or between 0 and 100.");
                return E_InvalidArg;
            }
            
            R8 start, end;
            R8 toleratedvalue = (value * tolerance > 1 ? tolerance/100 : tolerance);
            start = value - toleratedvalue;
            end = value + toleratedvalue;

            ostringstream ostr;
            ostr << "SELECT cik, perf_id, year, quarter FROM sa.list_PerfRatioValuesSlim where";
            ostr << " year = " << year;
            ostr << " and quarter = " << quarter ;
            ostr << " and perf_id = " << ratioId;
            ostr << " and perf_value between " << start << " and " << end;
            
            return PopulatePerformanceList(ps, ostr.str(), ppResult);
            
        }
    }
}