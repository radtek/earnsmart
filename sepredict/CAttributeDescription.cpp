/* 
 * File:   CAttributeDescription.cpp
 * Author: santony
 * 
 * Created on May 21, 2015, 8:05 PM
 */

#include "sepredictinternal.h"
#include "CAttributeDescription.h"
#include "CTimeSeriesBuilder.h"

using namespace se::predict;


CAttributeDescription::CAttributeDescription(ISession* ps) : CAttributeDescriptionBase<IAttributeDescription>(ps) 
{
}

CAttributeDescription::~CAttributeDescription() 
{
}

SE_CODE CAttributeDescription::Init(U32 id, TrainingDataContext ctxt)
{
    return CAttributeDescriptionBase::Init(id, ctxt);
}


CResultAttributeDescription::CResultAttributeDescription(ISession* ps ) : CAttributeDescriptionBase<IResultAttribDescription>(ps) 
{
}

CResultAttributeDescription::~CResultAttributeDescription() 
{
}

SE_CODE CResultAttributeDescription::Init(U32 id, TrainingDataContext ctxt)
{
    return CAttributeDescriptionBase::Init(id, ctxt);
}

SE_CODE PREDICTEXP IAttributeDescription::Create(ISession* ps, U32 uid, TrainingDataContext ctxt, IAttributeDescription** ppOut)
{
    return CObject<CAttributeDescription>::Create(ps, uid, ctxt, ppOut);
}

SE_CODE PREDICTEXP IResultAttribDescription::Create(ISession* ps, U32 uid, TrainingDataContext ctxt, IResultAttribDescription** ppOut)
{
    return CObject<CResultAttributeDescription>::Create(ps, uid, ctxt, ppOut);
    
}


//////////////////////// Base methods ///////////////////////////
SE_CODE PREDICTEXP IAttributeDescriptionBase::BuildTimeSeries(ISession* ps, IAttributeDescriptionBase* attribDesc, se::training::ITrainingTable** ppOut)
{
    if (!attribDesc)
        return E_InvalidArg;
    
    try 
    {
        return CTimeSeriesBuilderBase::GetTimeSeriesTable(ps, attribDesc, ppOut);
    }
    catch (const std::exception& ex)
    {
        //BOOSTER_ERROR("IAttributeDescriptionBase::BuildTimeSeries") << "A builder is not found to build the time series for attribute " << attribDesc->get_Id();
        return E_NoDataToReturn;
    }
}

//////////////////////// Attribute list//////////////////////

long unsigned int CAttributeDescriptionList::Filter(TrainingDataContext context, IAttributeDescriptionList* ppOut) 
{

}
