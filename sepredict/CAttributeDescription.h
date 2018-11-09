/* 
 * File:   CAttributeDescription.h
 * Author: santony
 *
 * Created on May 21, 2015, 8:05 PM
 */

#ifndef CATTRIBUTEDESCRIPTION_H
#define	CATTRIBUTEDESCRIPTION_H

#include "../seglobal/CList.h"
#include "sepredictapi.h"


namespace se
{
    namespace predict
    {
        template<typename TIBase>
        class CAttributeDescriptionBase : public CObjectRoot, public TIBase
        {
        public:
            CAttributeDescriptionBase(ISession* ps) : CObjectRoot(ps)
            {}
            
            CAttributeDescriptionBase(const CAttributeDescriptionBase& orig)  = delete;
            
            virtual ~CAttributeDescriptionBase(){}
            
            virtual TrainingDataContext get_Context() { return _ctxt;}
            
            virtual SE_CODE Init(U32 id, TrainingDataContext ctxt)
            {
                return Init(id, DATAPERIOD(), DATAPERIOD(), ctxt);
            }

            virtual SE_CODE Init(U32 id, const DATAPERIOD& dpStart, const DATAPERIOD& dpEnd, TrainingDataContext ctxt)
            {
                _set_Id(id);
                _ctxt = ctxt;
                _dpStart = dpStart;
                _dpEnd = dpEnd;
                return S_OK;
            }

            virtual void Serialize(ISerializedData<IString>* pIn){};
            
            virtual const DATAPERIOD& get_StartPeriod()
            {
                return _dpStart;
            }
            
            virtual void set_StartPeriod(const DATAPERIOD& dp)
            {
                _dpStart = dp;
            }
            
            virtual const DATAPERIOD& get_EndPeriod()
            {
                return _dpEnd;
            }
            
            virtual void set_EndPeriod(const DATAPERIOD& dp)
            {
                _dpEnd = dp;
            }
            
            virtual const TimeSeriesMetricType& get_Metric()
            {
                return _metric;
            }
            
            virtual void set_Metric(const TimeSeriesMetricType& metric)
            {
                _metric = metric;
            }
            
            
        protected:
            TrainingDataContext _ctxt;
            DATAPERIOD _dpStart, _dpEnd;
            TimeSeriesMetricType _metric = TimeSeriesMetricType::Standard;
        };


        class CAttributeDescription : public CAttributeDescriptionBase<IAttributeDescription>
        {
        public:
            CAttributeDescription(ISession*);
            CAttributeDescription(const CAttributeDescription& orig)  = delete;
            virtual ~CAttributeDescription();
            virtual SE_CODE Init(U32 id, TrainingDataContext ctxt);


        private:

        };
        
        class CAttributeDescriptionList : public CSPList<IAttributeDescriptionBase, IAttributeDescriptionList >
        {
        public:
            CAttributeDescriptionList(ISession* ps) : CSPList<IAttributeDescriptionBase, IAttributeDescriptionList >(ps){}
            virtual long unsigned int Filter(TrainingDataContext context, IAttributeDescriptionList* ppOut);

        };
        
        class CResultAttributeDescription : public CAttributeDescriptionBase<IResultAttribDescription>
        {
        public:
            CResultAttributeDescription(ISession*);
            CResultAttributeDescription(const CResultAttributeDescription& orig) = delete;
            virtual ~CResultAttributeDescription();
            virtual SE_CODE Init(U32 id, TrainingDataContext ctxt);
        private:

        };
        
    }
}


#endif	/* CATTRIBUTEDESCRIPTION_H */

