/* 
 * File:   CBayesianDataProcessor.cpp
 * Author: santony
 * 
 * Created on August 24, 2015, 1:54 PM
 */

#include "sepredictinternal.h"
#include "CBayesianDataProcessor.h"

using namespace se::predict;

CBayesianDataProcessor::CBayesianDataProcessor(ISession* ps, IPredictor* predictor, ITrainingSet* tset) :
    CXXXAlgoProcessBase(ps, predictor, tset)
{
}

CBayesianDataProcessor::~CBayesianDataProcessor() 
{
}

IResult* CBayesianDataProcessor::Run() 
{

}

