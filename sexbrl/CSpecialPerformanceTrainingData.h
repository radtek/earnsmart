/* 
 * File:   CSpecialPerformanceTrainingData.h
 * Author: santony
 *
 * Created on July 21, 2014, 4:09 PM
 */

#ifndef CSPECIALPERFORMANCETRAININGDATA_H
#define	CSPECIALPERFORMANCETRAININGDATA_H

#include <mutex>

namespace se
{
    namespace xbrl
    {

        class CSpecialPerformanceTrainingData 
        {
        public:
            CSpecialPerformanceTrainingData(ISession*, UID perfId);
            virtual ~CSpecialPerformanceTrainingData();
            
            SE_CODE Build(CIK cik, ITrainingTable** ppOut);
            
            SE_CODE Build(CIK cik, ITrainingTableList** ppOut);
        
        private:
            SE_CODE BuildRatio26(CIK cik, ITrainingTableList** ppOut);
            SE_CODE BuildCorelation(CIK left, CIK right, ITrainingTable** ppOut);
            
            
            // method for saving competitors with valid data. 
            SE_CODE SaveCompetitionProfile(CIK primary, CIK secondary, bool IsComplementory);
        private:
            std::mutex _mtx;
            UID _perfId;
            ISession* _ps;
        };

    }
    
}

#endif	/* CSPECIALPERFORMANCETRAININGDATA_H */

