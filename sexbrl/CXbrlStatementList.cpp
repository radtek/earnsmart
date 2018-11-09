/* 
 * File:   CXbrlStatementList.cpp
 * Author: santony
 * 
 * Created on February 27, 2014, 6:28 AM
 */

#include "sexbrlinternal.h"
#include "CXbrlStatementList.h"
#include "CXbrlStatement.h"
#include "../seglobal/exceptions.h"

#define SEDBCONN        CSEConnections::GetMain()


CXbrlStatementList::CXbrlStatementList(ISession* ps) : CROSPList<IXbrlStatement, IXbrlStatementList>(ps),
    _ceilYear(0), _floorYear(2999)
{
    
}

CXbrlStatementList::~CXbrlStatementList() 
{
    this->_vector.clear();
    this->_map.clear();
}

long unsigned int CXbrlStatementList::Init(CIK cik) 
{
    _cik = cik;
    try
    {
        CSmartConnection conn(SEDBCONN);
        // get presentation tree
        Query q = conn->query();
        q << "call list_FinancialStatements(" << cik << ", 0);";
        StoreQueryResult sqr = q.store();
        try
        {
            SP<IXbrl> pxbrl;
            RETURNIFFAILED(XbrlOpen(get_Session(), &pxbrl));
            SP<IFinancialsConsolidated> pcon;
            RETURNIFFAILED(pxbrl->GetConsolidatedFinancials(cik, &pcon));
            for (const Row& r : sqr)
            {
                SP<IXbrlStatement> pst;
                THROWIFFAILED(CObject<CXbrlStatement>::Create(get_Session(), cik, pcon._ptr,  (UID)r["statement_id"], r["title"], &pst), "Failed to create statement instance.");
                Add(pst);
                _floorYear = min(_floorYear, pst->get_LowerYear());
                _ceilYear = max(_ceilYear, pst->get_HighestYear());
            }
            while (q.more_results()) q.store_next();
        }
        catch(const std::exception& ex)
        {
            while (q.more_results()) q.store_next();
            throw ex;
        }
        return S_OK;
    }
    CATCHCLAUSE("Init")  ;
}


