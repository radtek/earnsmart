/* 
 * File:   CAnnFacade.cpp
 * Author: santony
 * 
 * Created on November 21, 2013, 9:03 AM
 */

#include "CAnnFacade.h"
#include <fann.h>
#include <fann_cascade.h>


using namespace se;

namespace se
{
    namespace math
    {
        
        const I32 CAnnFacade::SIGMOID_SYMMETRIC = FANN_SIGMOID_SYMMETRIC;


        CAnnFacade::CAnnFacade() : _fann(0), _NumberOfInputs(0)
        {
            
        }

        CAnnFacade::CAnnFacade(const CAnnFacade& orig) 
        {
        }

        CAnnFacade::~CAnnFacade() 
        {
            fann_destroy((struct fann*)_fann);
        }

        
        ErrorId CAnnFacade::CascadeTrainOnData
            (
                U32 NumberOfTrainingRecords,
                U32 NumberOfInputs,
                U32 NumberOfOutputs,
                TRAININGRECORDREQCB cbGetTrainingRecord
            )
        {
            _NumberOfInputs = NumberOfInputs;
            R4 *InputData, *OutputData;
            
            auto func = 
                [&](U32 recIndex, U32 numInputs, U32 numOutputs, R4* inputData, R4* outputData)
                    {
                        TRAININGRECORD vecInputs(numInputs), vecOutputs(numOutputs);
                        //inputData = InputData;
                        //outputData = OutputData;
                    };
            
                    );
                    
            _fann = (struct fann*) fann_create_shortcut(2, fann_num_input_train_data(trainData), fann_num_output_train_data(trainData));
            // train data
            fann_cascadetrain_on_data((struct fann*)_fann, trainData, 50000, 100, 0.0001);

            fann_destroy_train(trainData);
            return S_Success;
        }
        
        ErrorId CAnnFacade::Run(PREDICTONRECORD const& inputValues, ANNRUNCB callback)
        {
            
            return S_Success;
        }
        
        ErrorId CAnnFacade::Run(UID trainingRecord, PREDICTONRECORD const& inputValues, ANNRUNCB callback)
        {
            return E_ANNTRAINEDRECORDNOTFOUND;
        }
        
        void CAnnFacade::NativeCBWrapper(CAnnFacade* thisinstance, CREATETRAINDATAFUNC createtraindatafunc)
        {
            fann_train_data* trainData = fann_create_train_from_callback(
                    NumberOfTrainingRecords, 
                    NumberOfInputs, 
                    NumberOfOutputs,
                    createtraindatafunc);
        }
    }
}