/* 
 * File:   CAnnFacade.h
 * Author: santony
 *
 * Created on November 21, 2013, 9:03 AM
 * 
 * A simplified facade for Artificial intelligence library in ubuntu.
 */

#ifndef CANNFACADE_H
#define	CANNFACADE_H

#include "semathapi.h"
#include "CAnnFacade.h"
#include <functional>
#include <vector>

namespace se
{
    namespace math
    {
        typedef std::vector<R4> TRAININGRECORD, PREDICTONRECORD;
        
        // CALLBACK function to request training record. 
        // first parameter is an zero-based index ending at numberOfTrainingRecords-1
        // provided in CascadeTrainOnData method.
        // next two parameters is a single input and out training data record.
        
        typedef std::function<void(U32, TRAININGRECORD const&, TRAININGRECORD const&)> TRAININGRECORDREQCB;
        typedef std::function<void(PREDICTONRECORD const&)> ANNRUNCB;
        typedef void (*CREATETRAINDATAFUNC)(U32, U32, U32, R4*, R4*);
        
        class CAnnFacade 
        {
            
        public:
            CAnnFacade();
            virtual ~CAnnFacade();
            
        private:
            CAnnFacade(const CAnnFacade& orig);
            
            static void NativeCBWrapper(
                CREATETRAINDATAFUNC createtraindatafunc,
                U32 nRecords, U32 nInputs, U32 nOutputs
            );
            
        public:
            
            // Cascaded training
            // This method will create a cascase ANN and train data based on 
            // data provided using the callback function cbGetTrainingRecord.
            // NumberOfTrainingRecords: How many training records to train ?
            // NumberOfInputs : How many input records of R4 types  ?
            // NumberOfOutputs: How many output records of R4 types ?
            
            ErrorId CascadeTrainOnData(
                    U32 NumberOfTrainingRecords,
                    U32 NumberOfInputs,
                    U32 NumberOfOutputs,
                    TRAININGRECORDREQCB cbGetTrainingRecord
                );
            
            ErrorId Run(PREDICTONRECORD const& inputValues, ANNRUNCB callback);

            ErrorId Run(UID trainingRecord, PREDICTONRECORD const& inputValues, ANNRUNCB callback);
            
        private:
            void* _fann;
            U32 _NumberOfInputs;
        public:
            const static I32 SIGMOID_SYMMETRIC;

        };
    }
}


#endif	/* CANNFACADE_H */

