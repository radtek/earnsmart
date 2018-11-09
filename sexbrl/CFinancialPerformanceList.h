/* 
 * File:   CFinancialPerformanceList.h
 * Author: santony
 *
 * Created on April 1, 2014, 12:19 AM
 */

#ifndef CFINANCIALPERFORMANCELIST_H
#define	CFINANCIALPERFORMANCELIST_H

#include "../seglobal/CROList.h"
#include "sexbrlapi.h"
#include <unordered_map>

namespace se
{
    namespace xbrl
    {
        typedef std::unordered_map<string, SIZE> CikIndex;
        
        class CFinancialPerformanceList : public CROSPList<IFinancialPerformance, IFinancialPerformanceList >
        {
        public:
            CFinancialPerformanceList(ISession*);
            CFinancialPerformanceList(const CFinancialPerformanceList& orig)=delete;
            virtual ~CFinancialPerformanceList();
           
            virtual long unsigned int FilterRatioByRange(short year, short quarter, unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut);
            virtual long unsigned int FilterLatestAnnualRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut);
            virtual long unsigned int FilterLatestQuarterlyRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut);
            virtual long unsigned int FilterRatioByValue(short year, short quarter, unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut);
            virtual long unsigned int FilterLatestAnnualRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut);
            virtual long unsigned int FilterQuarterlyRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut);
            
            virtual bool TryGet(CIK cik, IFinancialPerformance** ppOut);
           

            //add override
            virtual void Add(IFinancialPerformance* item);

        private:
            // map to index cik
            CikIndex _cikIndex;
        };
    }
}
#endif	/* CFINANCIALPERFORMANCELIST_H */

