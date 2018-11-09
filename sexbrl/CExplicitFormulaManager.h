/* 
 * File:   CExplicitFormulaManager.h
 * Author: santony
 *
 * Created on April 17, 2014, 12:47 AM
 */

#ifndef CEXPLICITFORMULAMANAGER_H
#define	CEXPLICITFORMULAMANAGER_H

#include <functional>
#include <Poco/Mutex.h>

namespace se
{
    namespace xbrl
    {
        class CExplicitFormulaManagerState : public CObjectRoot, public IObject
        {
        public:
            CExplicitFormulaManagerState(ISession* ps) : CObjectRoot(ps){}
            
            static SE_CODE Instance(ISession*, CExplicitFormulaManagerState** ppOut);
        };
        
        class CExplicitFormulaManager : public CObjectRoot, public IObject
        {
            typedef std::function< long unsigned int (CExplicitFormulaManagerState*, I16, I16, R8*) > PerformanceFunction;
            typedef std::map<std::string, PerformanceFunction > FuncMap;
            typedef std::map<UID, SP<IXbrlIdList> > XbrlIdMatches;
            
        public:
            CExplicitFormulaManager(ISession*);
            virtual ~CExplicitFormulaManager();
            
            SE_CODE Run(CExplicitFormulaManagerState*,  IFinancialsConsolidated*, std::string funcName, I16 year, R8* outValue);
            SE_CODE Run(CExplicitFormulaManagerState*, IFinancialsConsolidated*, std::string funcName, I16 year, I16 quarter, R8* outValue);
            
            static SE_CODE Instance(ISession*, CExplicitFormulaManager** ppOut);
            
        private:
            SE_CODE RevenueGrowth(CExplicitFormulaManagerState*, I16 year, I16 quarter, R8* outValue);
            SE_CODE QuarterRevenueWeight(CExplicitFormulaManagerState*, I16 year, I16 quarter, R8* outValue);
            SE_CODE CompetitionCorelation(CExplicitFormulaManagerState*, I16 year, I16 quarter, R8* outValue);
            void InitFunctions();
            
        private:
            IFinancialsConsolidated* _pcon;
            FuncMap _map;
            XbrlIdMatches _xmatches;
            
            static Poco::Mutex _lock;
        };
    }
}
#endif	/* CEXPLICITFORMULAMANAGER_H */

