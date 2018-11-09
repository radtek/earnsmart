/* 
 * File:   CXXXAlgoProcessBase.cpp
 * Author: santony
 * 
 * Created on May 27, 2015, 9:22 PM
 */

#include "sepredictinternal.h"
#include "CXXXAlgoProcessBase.h"

using namespace se::predict;


CXXXAlgoProcessBase::CXXXAlgoProcessBase(ISession* ps, IPredictor* predictor, ITrainingSet* tset) : CObjectRoot(ps)
{
    _predictor = predictor;
    _tset = tset;
}

CXXXAlgoProcessBase::~CXXXAlgoProcessBase() 
{
    
}

IResult* CXXXAlgoProcessBase::Run() 
{
    return _result;
}


CXXXAlgoProcessBase* CXXXAlgoProcessBase::Construct(IPredictor* predictor, ITrainingSet* tset) throw(const std::exception&)
{
    SP<CXXXAlgoProcessBase> base;
    PredictionAlgos algo = tset->get_Algo();
    switch (algo)
    {
        case PredictionAlgos::MultiVarLinearRegression:
            break;
            
        case PredictionAlgos::ArtificialNeuralNetwork:
            break;
    }
    return base;
}


