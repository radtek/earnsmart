/* 
 * File:   CSEConnPool.h
 * Author: santony
 *
 * Created on August 15, 2012, 9:20 AM
 */

#ifndef CSECONNPOOL_H
#define	CSECONNPOOL_H

#include "../external/mariadbpp/mysql++.h"
#include "../seglobaltypedefs.h"
#include <mutex>
#include "semysqlapi.h"

using namespace mysqlpp;
using namespace std;
/*
 * The database connection pool, configured to connect to the 
 * main "se" database.
 */

namespace se
{
    namespace data
    {

        class MSQLEXP CSEConnPool : public ConnectionPool
        {
            friend class CSEConnections;
            
        public:
            virtual ~CSEConnPool();

            virtual void release(const Connection* pc);
            virtual Connection* grab();

        protected:
            // protected constructor to prevent direction instantiation.
            CSEConnPool(CSTR userId, CSTR password, CSTR db, CSTR serverHost);
            CSEConnPool(const CSEConnPool& orig);

            // Superclass overrides
            virtual Connection* create();
            virtual void destroy(Connection* cp);
            virtual U32 max_idle_time();

        private:
            string _userId, 
                _password,
                _db,
                _serverHost;
            U32 _connInUse;
            static std::mutex _mutex;

        };


        
    }
}


#endif	/* CSECONNPOOL_H */

