/* 
 * File:   CStoreQueryResult.cpp
 * Author: santony
 * 
 * Created on December 16, 2013, 10:41 AM
 */

#include "../seglobal/Object.h"
#include "CStoreQueryResult.h"
#include "CSEConnections.h"


namespace se
{
    namespace data
    {
        #define SEDBCONN    CSEConnections::GetMain()

        SE_CODE MSQLEXP IStoreQueryResult::Create(ISession* ps, mysqlpp::StoreQueryResult&& result, IStoreQueryResult** ppOut) 
        {
            return CObject<CStoreQueryResult>::Create(ps, result, ppOut);
        }

        SE_CODE MSQLEXP IStoreQueryResult::GetDataTable(ISession* ps, const char* query, IStoreQueryResult** ppOut)
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query(query);
                return CObject<CStoreQueryResult>::Create(ps, q.store(), ppOut);
            }
            catch(...)
            {
                return E_DBQueryException;
            }
        }
        
        
        CStoreQueryResult::CStoreQueryResult(ISession* ps): CObjectRoot(ps) 
        {
            
        }

        CStoreQueryResult::CStoreQueryResult(const CStoreQueryResult& orig) : CObjectRoot(orig)
        {
            _result = orig._result;
        }
        
        CStoreQueryResult::CStoreQueryResult(CStoreQueryResult&& orig):CObjectRoot(orig)
        {
            _result = std::move(orig._result);
        }

        CStoreQueryResult::~CStoreQueryResult() 
        {
        }
        
        ErrorId CStoreQueryResult::Init(mysqlpp::StoreQueryResult& result)
        {
            _result = std::move(result);
            return S_Success;
        }
        
        mysqlpp::StoreQueryResult& CStoreQueryResult::get_Result()
        {
            return _result;
        }

    }
}
