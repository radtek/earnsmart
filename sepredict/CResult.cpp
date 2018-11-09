/* 
 * File:   CResult.cpp
 * Author: santony
 * 
 * Created on May 14, 2015, 8:23 PM
 */

#include "sepredictinternal.h"
#include "CResult.h"

using namespace se::predict;

CResult::CResult(ISession* ps) : CObjectRoot(ps)
{
}

CResult::~CResult() {
}

OmniType& CResult::Value() 
{
    return _value;
}

