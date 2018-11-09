/* 
 * File:   CEconomyAPI.h
 * Author: santony
 *
 * Created on January 23, 2014, 9:31 AM
 */

#ifndef CECONOMYAPI_H
#define	CECONOMYAPI_H

#include "seeconomapi.h"
#include "../semysql/CStoreQueryResult.h"
#include <Poco/Mutex.h>

namespace se
{
    namespace economy
    {

        class CEconomyAPI : public CObjectRoot, public IEconomyApi
        {
        public:
            CEconomyAPI(ISession*);
            virtual ~CEconomyAPI();
            
            NamedObjectNamingType get_TypeId() override;

            
            virtual SE_CODE get_EconomicNewsFeeds(IEconomicNewsFeeds** ppOut);
            virtual SE_CODE ProcessEconomicNewsFeeds() ;

            virtual SE_CODE get_IndicatorCategory(UID id, IIndicatorCategory **ppOut) ;
            
            virtual SE_CODE get_IndicatorCategories(IIndicatorCategoryList **ppOut);
            
            virtual SE_CODE get_EconomicIndicator(UID id, IEconomyIndicator **ppOut) ;
            
            virtual SE_CODE set_EconomicIndicator(IEconomyIndicator *pInOut);
            
            virtual SE_CODE get_EconomicIndicators(
                IEconomyIndicatorList **ppOut,
                QUERYFUNC(IEconomyIndicator) criteria = nullptr
            );
            virtual EIVAL get_EconomicIndicatorLatestValue(UID id);
            virtual SE_CODE get_EconomicIndicatorDataList(
                UID eid, 
                se::data::IStoreQueryResult** ppOut,
                U32 instances = 20
            );

            virtual SE_CODE get_EconomicIndicatorTrainingTable(
                UID indicatorId, 
                I16 countryid, I16 stateid,
                DATAPERIOD from, DATAPERIOD to,
                se::training::ITrainingTable** ppOutTable);
            

            virtual long unsigned int get_WeightedLinearFitEquation(se::training::ITrainingTable* pTable, double* c0, double* c1, double* c00, double* c01, double* c11, double* chisq);

            virtual long unsigned int get_LinearFitEquation(se::training::ITrainingTable* pTable, double* c0, double* c1, double* c00, double* c01, double* c11, double* chisq);

            virtual long unsigned int SourceEconomicData(unsigned long eid);

            virtual long unsigned int SourceEconomicData();
            
        private:
            
            SE_CODE getIndicatorCategory(UID id, IIndicatorCategory **ppOut);
            SE_CODE getIndicator(UID id, IEconomyIndicator **ppOut);
            SE_CODE getIndicatorCategory(Row& row, IIndicatorCategory **p);
            SE_CODE getIndicator(Row& row, IEconomyIndicator **p);

            PERIOD ConvertPeriodFromString(CSTR periodname);
            
            SE_CODE GetConfigurator(se::config::IConfigurator** ppOut);
            
        private:
            //LoggerPtr _logger;
            
        public:
            static Poco::Mutex _lock;

        };
        
    }
}


#endif	/* CECONOMYAPI_H */

