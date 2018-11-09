/* 
 * File:   CPersistPerformanceData.h
 * Author: santony
 *
 * Created on February 20, 2014, 5:36 PM
 */

#ifndef CPERSISTPERFORMANCEDATA_H
#define	CPERSISTPERFORMANCEDATA_H

#include "CFinancialRatio.h"


namespace se
{
    namespace xbrl
    {
        class CPersistPerformanceData 
        {
        public:
            CPersistPerformanceData(ISession* ps);
            virtual ~CPersistPerformanceData();
            SE_CODE Run();
            SE_CODE RunForCik(CIK cik, UID perfId = 0);
            SE_CODE RunForCik(IFinancialsConsolidated* pcon, UID perfId = 0);
            
            static SE_CODE ProcessRatio(CSmartConnection& conn, CIK cik, IFinancialRatio* pratio);
            static void UpdatePerfValue(CSmartConnection& conn, UID key,  I16 year, I16 quarter, R8 newValue);
            static void UpdatePerfPredValue(CSmartConnection& conn, UID key,  I16 year, I16 quarter, R8 newValue);
            
        private:
            ISession* _ps;
        private:
            ////LoggerPtr _logger;

        };
    }
}


#endif	/* CPERSISTPERFORMANCEDATA_H */

