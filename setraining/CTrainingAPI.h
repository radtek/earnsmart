/* 
 * File:   CEconAPI.h
 * Author: santony
 *
 * Created on November 18, 2013, 10:05 AM
 */

#ifndef CECONAPI_H
#define	CECONAPI_H


//#include <log4cxx/logger.h>
#include <functional>
#include <Poco/Mutex.h>

namespace se
{
    namespace training
    {
        class CTrainingAPI : public CObjectRoot, public ITrainingAPI
        {
        public:
            CTrainingAPI(ISession*);
            CTrainingAPI(const CTrainingAPI& orig);
            virtual ~CTrainingAPI();
            
            //virtual ErrorId get_InflationCPI(UID countryId, UID stateId, I16 year, I16 month, IROList<InflationCPIData> **ppOut);
            
            virtual ErrorId AnnTrainData(UID trainingId);
            virtual ErrorId AnnPredictResult(UID trainingId, ANNINPUTCALLBACK inputCallback, ANNOUTPUTCALLBACK outputCallback);
            virtual ErrorId get_TrainingRecords(UID trainingId, se::data::IStoreQueryResult** result);
            
        private:
            //log4cxx::LoggerPtr _logger;
            
        public:
            static Poco::Mutex _trainingLock;

        };
    }
}

#endif	/* CECONAPI_H */

