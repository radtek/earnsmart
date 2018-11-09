/* 
 * File:   CXbrlAccountMatcher.cpp
 * Author: santony
 * 
 * Created on April 26, 2014, 7:20 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlAccountMatcher.h"
#include "../seglobal/exceptions.h"
#include "../semain/semain.h"

namespace se
{
    namespace xbrl
    {

        CXbrlAccountMatcher::CXbrlAccountMatcher(ISession* ps, IFinancialsConsolidated* pcon, unsigned long matchId) : 
            _matchId(matchId),
            _ps(ps)
        {
            assert(ps != nullptr);
            
            SEDBCONN = CSEConnections::GetMain();
            _pcon = pcon;
            auto pxbrl = ((ISessionInterface*)ps)->get_XbrlApi();
            THROWIFFAILED(pxbrl->GetMatchAccounts(_matchId, &_xidlist, _pcon->get_Cik()), "Failed to get matched xbrl ids.");
            
        }

        OmniType CXbrlAccountMatcher::GetAccountValue(short year) 
        {
            return GetAccountValue(year, 0);
        }

        long unsigned int CXbrlAccountMatcher::GetAccountRecord(IXbrlRecord** ppOut) 
        {
            for (UID xid : *_xidlist)
            {
                if (ISOK(_pcon->get_Record(xid, ppOut)))
                    return S_OK;
            }
            return E_NotFound;
        }


        OmniType CXbrlAccountMatcher::GetAccountValue(short year, short quarter) 
        {
            
            SP<IXbrlRecord> precord;
            for (UID xid : *_xidlist)
            {
                if (ISOK(_pcon->get_Record(xid, &precord)))
                    break;
            }
            
            if (!precord)
                return ER8;
            
            SP<IXbrlField> pfield;
            if (FAILED(precord->get_Financials(year, &pfield)))
            {
                return (OmniType)ER8;
            }                
            
            if (!quarter)
                return pfield->get_FY();
            
            return pfield->get_Q(quarter);
        }

        CXbrlAccountMatcher::~CXbrlAccountMatcher() 
        {
        }

    }
}
