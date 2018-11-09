/* 
 * File:   CXbrlJsonOutputs.cpp
 * Author: santony
 * 
 * Created on February 4, 2014, 2:30 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlJsonOutputs.h"
#include "../seglobal/exceptions.h"
#include <string>
#include <jsoncpp/json/json.h>

using namespace Json;

CXbrlJsonOutputs::CXbrlJsonOutputs(ISession* ps) : CObjectRoot(ps)  
{
}

CXbrlJsonOutputs::~CXbrlJsonOutputs() {
}

long unsigned int CXbrlJsonOutputs::get_YearlyData(CIK cik, std::initializer_list<unsigned long> xids, std::ostream &outputstream) 
{
    SP<IXbrl> papi;
    RETURNIFFAILED(XbrlOpen(get_Session(), &papi));
    SP<IFinancialsConsolidated> pcon;
    RETURNIFFAILED(papi->GetConsolidatedFinancials(cik, &pcon));
    SP<IXbrlRecordList> pxrl;
    RETURNIFFAILED(pcon->get_Records(xids, &pxrl));
    Value root;
    Value& inner = root[0];
    inner[0] = "year";
    int i=1;
    stringstream str;
    for (auto it = pxrl->begin(); it != pxrl->end(); ++it)
    {
        IXbrlAbstractRecord* paxr = *it;
        if (!paxr->get_IsAbstract())
        {
            IXbrlRecord* pxr = static_cast<IXbrlRecord*>(paxr);
            string name = pxr->get_XbrlLabel();
            inner[i] = name;
            int j=1;
            for (I16 year = pxrl->get_LowerYear(); year <= pxrl->get_HighestYear(); ++year)
            {
                SP<IXbrlField> pfld;
                RETURNIFFAILED(pxr->get_Financials(year, &pfld));
                if (year == pxrl->get_HighestYear() && (double)pfld->get_FY() == 0)
                    break;
                Value& values = root[j++];
                str.str("");
                str.clear();
                str << std::round(year);
                values[0] = str.str();
                values[i] = (double)pfld->get_FY();
            }
            i++;
        }
    }
    outputstream << root;
    return S_OK;
}

