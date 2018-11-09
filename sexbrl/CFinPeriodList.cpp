/* 
 * File:   CFinPeriodList.cpp
 * Author: santony
 * 
 * Created on June 16, 2014, 8:22 PM
 */

#include "sexbrlinternal.h"
#include "CFinPeriodList.h"

namespace se
{
    namespace xbrl
    {

        CFinPeriodList::CFinPeriodList(ISession* ps) : CList<FINPRDDATA, IFinPeriodList>(ps)
        {
        }


        CFinPeriodList::~CFinPeriodList() 
        {
        }

        long unsigned int CFinPeriodList::get(DATAPERIOD period, OmniType* pOut) 
        {
            auto find = _pos.find(period);
            if (find != _pos.end())
            {
                *pOut = std::get<1>(_vector[find->second]);
                return S_OK;
            }
            return E_NotFound;
        }

        void CFinPeriodList::Add(FINPRDDATA item) 
        {
            _pos.insert(make_pair(std::get<0>(item), _vector.size()));
            CList<FINPRDDATA, IFinPeriodList>::Add(item);
        }
        
    }
}