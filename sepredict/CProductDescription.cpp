/* 
 * File:   CProductDescription.cpp
 * Author: santony
 * 
 * Created on May 9, 2015, 10:35 PM
 */

#include "sepredictinternal.h"
#include "CProductDescription.h"

using namespace se::predict;

CProductDescription::CProductDescription(ISession* ps) : CObjectRoot(ps) 
{
    
}

CProductDescription::~CProductDescription() 
{
    
}

long unsigned int CProductDescription::Init(unsigned long Id, ProductDescriptionType type, TrainingDataContext ctxt) 
{
    set_Id(Id);
    _type = type;
    _ctxt = ctxt;
    return S_OK;
}

TrainingDataContext CProductDescription::get_Context() 
{
    return _ctxt;
}

void CProductDescription::set_Context(TrainingDataContext value) 
{
    _ctxt = value;
}


ProductDescriptionType CProductDescription::get_Type() {
    return _type;
}

void CProductDescription::set_Type(ProductDescriptionType type) {
    _type = type;
}


SE_CODE PREDICTEXP IProductDescription::Create(ISession* ps, UID id, ProductDescriptionType type, TrainingDataContext ctxt, IProductDescription** ppOut)
{
    if (id < 0) return E_InvalidArg;
    return CObject<CProductDescription>::Create(ps, id, type, ctxt, ppOut);
}

