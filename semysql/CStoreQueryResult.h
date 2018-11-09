/* 
 * File:   CStoreQueryResult.h
 * Author: santony
 *
 * Created on December 16, 2013, 10:41 AM
 */

#ifndef CSTOREQUERYRESULT_H
#define	CSTOREQUERYRESULT_H

#include "../external/mariadbpp/result.h"
#include "../seglobal/IObject.h"
#include "../seglobal/CObjectRoot.h"
#include "semysqlapi.h"

namespace se
{
    namespace data
    {
        
        class MSQLEXP CStoreQueryResult : public CObjectRoot, public IStoreQueryResult
        {
        public:
            CStoreQueryResult(ISession*);
            CStoreQueryResult(const CStoreQueryResult& orig);
            CStoreQueryResult(CStoreQueryResult&& orig);
            virtual ~CStoreQueryResult();
            virtual ErrorId Init(mysqlpp::StoreQueryResult& result);
            virtual mysqlpp::StoreQueryResult& get_Result();
            
        private:
            mysqlpp::StoreQueryResult _result;

        };

    }
}

#endif	/* CSTOREQUERYRESULT_H */

