/* 
 * File:   CEconomincallyAdjustedPrices.cpp
 * Author: santony
 * 
 * Created on November 18, 2013, 11:20 AM
 */

#include "internal.h"
#include "CEconomincallyAdjustedPrices.h"

using namespace se::securities;

const SIZE EconomicallyAdjustedPrices_ArraySize = 2;   // for none, and inflation. increment for others.

CEconomincallyAdjustedPrices::CEconomincallyAdjustedPrices(ISession* ps) : CObjectRoot(ps) 
{
    _v = new R8[EconomicallyAdjustedPrices_ArraySize];
}

CEconomincallyAdjustedPrices::~CEconomincallyAdjustedPrices() 
{
    delete[] _v;
}


R8 CEconomincallyAdjustedPrices::get_Value(EAM economicAdjustor) 
{
    SIZE index;
    switch (economicAdjustor)
    {
        case EAM_None:
            index = 0;
            break;
        case EAM_Inflation:
            index = 1;
            break;
        default :
            index = 0;
            break;
    }
    return _v[index];
}

void CEconomincallyAdjustedPrices::set_Value(EAM economicAdjustor, R8 value)
{
    SIZE index;
    switch (economicAdjustor)
    {
        case EAM_None:
            index = 0;
            break;
        case EAM_Inflation:
            index = 1;
            break;
        default :
            index = 0;
            break;
    }
    _v[index] = value;
}
