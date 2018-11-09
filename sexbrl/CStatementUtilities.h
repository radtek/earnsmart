/* 
 * File:   CStatementUtilities.h
 * Author: santony
 *
 * Created on September 29, 2013, 11:38 AM
 */

#ifndef CSTATEMENTUTILITIES_H
#define	CSTATEMENTUTILITIES_H

#include "sexbrlapi.h"

////using namespace log4cxx;

namespace se
{
    namespace xbrl
    {
        class CStatementUtilities 
        {
        public:
            CStatementUtilities(ISession*);
            CStatementUtilities(const CStatementUtilities& orig)=delete;
            virtual ~CStatementUtilities();

            DATAPERIOD DecipherDataPeriod(CDate& filingDate, XFT formType);
            ErrorId GetRecord(CSmartConnection &  conn, CIK cik, DATAPERIOD period, XFT formType, UID *pOutRecId);
            ErrorId GetStatementNames(CSmartConnection& conn, CIK cik, DATAPERIOD period, XFT formType, IROSPList<IXbrlStatement> **ppOut);
            ErrorId GetCikFromProductId(CSmartConnection& conn, UID ProductId, IString** ppOutCik);
            
        private:
            ISession* _ps ;
        };
    }
}

#endif	/* CSTATEMENTUTILITIES_H */

