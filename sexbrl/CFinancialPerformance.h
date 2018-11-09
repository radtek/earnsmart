/* 
 * File:   CFinancialPerformance.h
 * Author: santony
 *
 * Created on February 14, 2014, 2:36 PM
 */

#ifndef CFINANCIALPERFORMANCE_H
#define	CFINANCIALPERFORMANCE_H

#include "CFinancialRatioList.h"
#include "CFinancialRatio.h"
#include <unordered_map>
#include "CExplicitFormulaManager.h"

namespace se
{
    namespace xbrl
    {
        typedef std::unordered_map<FNR, R8> FINRATIOMAP;
        
        class CFinancialPerformance : public CObjectRoot, public IFinancialPerformance
        {
        public:
            CFinancialPerformance(ISession*);
            CFinancialPerformance(const CFinancialPerformance& orig)=delete;
            virtual ~CFinancialPerformance();
            SE_CODE Init(IFinancialsConsolidated* pConsolidated);
            SE_CODE Init(CIK cik, bool OnlyPubliclyDisplayed);
            virtual double get_Ratio(FNR ratio);
            virtual SE_CODE get_Ratio(FNR ratio, IFinancialRatio** ppOut) ;
            virtual long unsigned int get_Ratios(IFinancialRatioList**);
            virtual long unsigned int get_RatioGroups(IFinancialRatioGroupList**);
            virtual CIK get_CIK() ;
            virtual void Serialize(ISerializedData<IString>* pIn);

        private:
            SE_CODE BuildRatios();
            SE_CODE BuildRatiosFromCache(bool OnlyPubliclyDisplayed = true);
            
        private:
            SP<IFinancialsConsolidated> _pCon;
            SP<CFinancialRatioList> _pRatios;
            SP<CFinancialRatioGroupList> _pRatioInGroups;
            std::string _cik ;
            ////LoggerPtr _logger;
        };
    }
}


#endif	/* CFINANCIALPERFORMANCE_H */

