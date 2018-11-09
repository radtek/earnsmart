/* 
 * File:   CXbrlUtilities.h
 * Author: santony
 *
 * Created on April 2, 2014, 2:25 PM
 */

#ifndef CXBRLUTILITIES_H
#define	CXBRLUTILITIES_H

#include "../semysql/CStoreQueryResult.h"
using namespace se::data;

namespace se
{
    namespace xbrl
    {
        struct CXbrlUtilities 
        {
            
            static string const GetInstanceTableName(U64 cik);
            static string const GetInstanceCtxtTableName(U64 cik);
            
            static void CreateInstTablesIfNotFound(ICIK cik);
            
            static U64 GetCikFromRecordId(U64 recordId);

            static string const GetInstanceTableName(CSmartConnection& conn, U64 cik);
            static string const GetInstanceCtxtTableName(CSmartConnection& conn, U64 cik);
            static U64 GetCikFromRecordId(CSmartConnection& conn, U64 recordId);
            
            static SE_CODE FilterPerformanceRatiosByRange(ISession*, I16 year, I16 quarter, UID ratioId, R8 start, R8 end, IStoreQueryResult** ppResult);
            static SE_CODE FilterPerformanceRatiosByValue(ISession*, I16 year, I16 quarter, UID ratioId, R8 value, R8 tolerance, IStoreQueryResult** ppResult);
            
            static SE_CODE FilterPerformanceLatestAnnualRatiosByRange(ISession*, UID ratioId, R8 start, R8 end, IStoreQueryResult** ppResult);
            static SE_CODE FilterPerformanceLatestQuarterlyRatiosByRange(ISession*, UID ratioId, R8 start, R8 end, IStoreQueryResult** ppResult);
            
            static SE_CODE FilterPerformanceLatestAnnualRatiosByValue(ISession*, UID ratioId, R8 value, R8 tolerance, IStoreQueryResult** ppResult);
            static SE_CODE FilterPerformanceLatestQuarterlyRatiosByValue(ISession*, UID ratioId, R8 value, R8 tolerance, IStoreQueryResult** ppResult);
            
        private:
            static SE_CODE PopulatePerformanceList( ISession*, const string& sql, IStoreQueryResult** ppOut);
            
        };
    }
}
#endif	/* CXBRLUTILITIES_H */

