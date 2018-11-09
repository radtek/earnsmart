/* 
 * File:   CFinancialRatioList.cpp
 * Author: santony
 * 
 * Created on February 14, 2014, 5:01 PM
 */

#include "sexbrlinternal.h"
#include "CFinancialRatioList.h"
#include "CFinancialRatio.h"

namespace se
{
    namespace xbrl
    {


        CFinancialRatioList::CFinancialRatioList(ISession* ps) : CROSPList<IFinancialRatio, IFinancialRatioList>(ps)  
        {
        }


        CFinancialRatioList::~CFinancialRatioList() 
        {
            _m.clear();
        }

        long unsigned int CFinancialRatioList::get_Ratio(FNR ratio, IFinancialRatio** ppOut) 
        {
            auto f = _m.find(ratio);
            if (f != _m.end())
            {
                *ppOut = f->second;
                f->second->AddRef();
                return S_OK;
            }
            return E_NotFound;
        }

        void CFinancialRatioList::Add(IFinancialRatio* item) 
        {
            auto size = _vector.size();
            _vector.push_back(item);
            item->AddRef();
            
            _m.insert(make_pair(item->get_Id(), item));
        }
        
        void CFinancialRatioList::AddRange(IFinancialRatioList* pIn)
        {
            if (pIn)
            {
                for (IFinancialRatio* p : *pIn)
                {
                    Add(p);
                }
            }
        }
        
        ////////////////////////////// CFinancialRatioGroupList ///////////////////////////

        CFinancialRatioGroupList::CFinancialRatioGroupList(ISession* ps) : 
            CROSPList<IFinancialRatioGroup, IFinancialRatioGroupList>(ps)
        {
        }

        CFinancialRatioGroupList::~CFinancialRatioGroupList() 
        {
        }

        long unsigned int CFinancialRatioGroupList::get_Ratios(unsigned long groupId, IFinancialRatioList** ppOut) 
        {
            auto f = _gmap.find(groupId);
            if (f != _gmap.end())
            {
                f->second->get_Ratios(ppOut);
                return S_OK;
            }
            return E_NotFound;   
        }
        
        long unsigned int CFinancialRatioGroupList::get_JoinedList(IFinancialRatioList** ppOut) 
        {
            SP<CFinancialRatioList> p;
            RETURNIFFAILED(CObject<CFinancialRatioList>::Create(get_Session(), &p));
            SE_CODE r;
            for (auto pr : _gmap)
            {
                SP<IFinancialRatioList> pl;
                r = pr.second->get_Ratios(&pl);
                p->AddRange(pl._ptr);
            }
            p.CopyTo(ppOut);
        }
    }
}