/* 
 * File:   CXbrlAccountMatcher.h
 * Author: santony
 *
 * Created on April 26, 2014, 7:20 PM
 */

#ifndef CXBRLACCOUNTMATCHER_H
#define	CXBRLACCOUNTMATCHER_H

#include "sexbrlapi.h"


namespace se
{
    namespace xbrl
    {

        class CXbrlAccountMatcher 
        {
        public:
            CXbrlAccountMatcher(ISession*, IFinancialsConsolidated*, UID matchId);
            ~CXbrlAccountMatcher();
            
            OmniType GetAccountValue(I16 year);
            OmniType GetAccountValue(I16 year, I16 quarter);
            SE_CODE GetAccountRecord(IXbrlRecord**);
            
        private:
            IFinancialsConsolidated* _pcon;
            UID _matchId;
            CSEConnPool* SEDBCONN;
            SP<IXbrlIdList> _xidlist;
            ISession* _ps;
        };
        
    }
}

#endif	/* CXBRLACCOUNTMATCHER_H */

