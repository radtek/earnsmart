/* 
 * File:   CTimeSeriesBuilder.h
 * Author: santony
 *
 * Created on June 20, 2015, 11:42 AM
 */

#ifndef CTIMESERIESBUILDER_H
#define	CTIMESERIESBUILDER_H

#include "sepredictapi.h"
#include <unordered_map>


namespace se
{
    namespace predict
    {
        class CCustomDataTimeSeriesBuilder;
        
        typedef std::unordered_map< UID, CCustomDataTimeSeriesBuilder* > CustomTimeSeriesBuilders;
        typedef std::tuple<UID, TimeSeriesMetricType> TimeSeriesParams;
        class CTimeSeriesBuilderBase 
        {
        public:
            CTimeSeriesBuilderBase(ISession*);
            CTimeSeriesBuilderBase(ISession*, IAttributeDescriptionBase*);
            CTimeSeriesBuilderBase(const CTimeSeriesBuilderBase& orig) = delete;
            virtual ~CTimeSeriesBuilderBase();
            
            IAttributeDescriptionBase* get_Attrib();
            
            void set_Attrib(IAttributeDescriptionBase*);
            
            static std::shared_ptr<CTimeSeriesBuilderBase> Build(ISession*, IAttributeDescriptionBase*);
            
            static SE_CODE GetTimeSeriesTable(ISession*, IAttributeDescriptionBase* pIn, se::training::ITrainingTable** ppOut);
            
            virtual se::training::ITrainingTable* get_Table(  ) = 0;
            
        private:
            static CustomTimeSeriesBuilders _customBuilders;
        protected:
            ISession* _ps;
            
        protected:
            SP<IAttributeDescriptionBase> _pattrib;
        };
        
        
        // General economic data timeseries builder
        class CEconomicDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CEconomicDataTimeSeriesBuilder(ISession* ps, IAttributeDescriptionBase*);
            virtual ~CEconomicDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table(  );

        };
        
        // general sector performance timeseries builder
        class CSectorDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CSectorDataTimeSeriesBuilder(ISession* ps, IAttributeDescriptionBase*);
            virtual ~CSectorDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table(  );

        };

        // general industry performance timeseries builder
        class CIndustryDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CIndustryDataTimeSeriesBuilder(ISession* ps, IAttributeDescriptionBase*);
            virtual ~CIndustryDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table( );

        };

        // general firm performance timeseries builder (could be firm's revenue, expense, netincome, etc)
        class CFirmDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CFirmDataTimeSeriesBuilder(ISession* ps, IAttributeDescriptionBase*);
            virtual ~CFirmDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table(  );

        };

        // general security price timeseries builder
        class CSecurityDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CSecurityDataTimeSeriesBuilder(ISession* ps, IAttributeDescriptionBase*);
            virtual ~CSecurityDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table(  );

            
        };
        
        // custom timeseries builder
        class CCustomDataTimeSeriesBuilder : public CTimeSeriesBuilderBase
        {
        public:
            CCustomDataTimeSeriesBuilder(ISession* ps);
            virtual ~CCustomDataTimeSeriesBuilder();

            virtual se::training::ITrainingTable* get_Table(  );

        };
        


    }
}
#endif	/* CTIMESERIESBUILDER_H */

