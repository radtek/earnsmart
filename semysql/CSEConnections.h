/* 
 * File:   CSEConnections.h
 * Author: santony
 *
 * Created on August 15, 2012, 1:08 PM
 */

#ifndef CSECONNECTIONS_H
#define	CSECONNECTIONS_H

#include "CSEConnPool.h"
#include <map>
#include <zthread/ThreadLocal.h>



/*
 * Error codes are defined the following way in a 32-bit number (HRESULT)
 * 
 *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   
 *-------------------------------------------------------------------------
 *  E| |C|     | T | |     Facility    |               Code                |
 * 
 * Facility code is 1
 * T = {
 *      00 = Error
 *      01 = Warning
 *      10 = Information
 *      11 = Debug
 * }
 * 
 */

#define E_DBHostUnreachable                     0x80010000      // db server cannot be reached.
#define E_DBQueryException                      0x80010001      // query exception.
#define E_DBUpdateError                         0x80010002      // failed to execute an insert, update or delete statement.
//#define S_SampleInfo                            0x02010000      // a sample info message
//#define W_SampleWarning                         0x01010000      // a sample warning
//#define D_SampleDebug                           0x03010000      // a sample debug message.


using namespace std;

namespace se
{
    namespace data
    {
        class MSQLEXP CSEConnections 
        {
        private:
            static CSEConnPool* _pMain ;
            static CSEConnPool* _pBackup;
            static CSEConnPool* _pEodData;
            static CSEConnPool* _pAdmin;
        
        public:
            static bool InitConfig(CSTR configfilename);
            static CSEConnPool* GetMain() ;
            static CSEConnPool* GetBackup();
            static CSEConnPool* GetEodData();
            static CSEConnPool* GetAdmin();
            static void Shutdown();
                
        };
        
        
        /*
         * Use this class type at the function stack level
         * to automatically allocate and release mysql db connection
         * from/to connection pool.
         * ALERT: this class in not thread-safe at present.
         */
        class MSQLEXP CSmartConnection 
        {
            CSEConnPool* _pool;
            Connection* _pConn;
            Transaction* _pTrans;
            int _iTransCount;
            string _key;
            static std::mutex _mutex;
            static ZThread::ThreadLocal<Connection*> _tlsConnections;
            int _defaulttimeout;
            
        public:
            CSmartConnection(CSEConnPool* pool);

            ~CSmartConnection();
            
            Connection* operator->();
            
            operator Connection* ();
            
            void LockTables();
            
            void UnlockTables();
            
            void BeginTransaction();
            
            void RollbackTransaction();
            
            void CommitTransaction();
            
            void set_ConnectionTimeout(int timeout);
            
            void RestoreTimeout();
            
            CSTR get_Key();
        
            bool get_HasData(CSTR tableName, CSTR condition);
            
            string get_ScalarValue(CSTR tableName, CSTR column, CSTR condition);
            
        };
        
#define ROLLBACKCATCHCLAUSE(fname)             \
        catch(seexception& ex)      \
        {                           \
            conn.RollbackTransaction(); \
            BERR << "Reported std error as " << ex.what();\
            return ex.error();      \
        }                           \
        catch(std::exception& ex)   \
        {                           \
            conn.RollbackTransaction(); \
            BERR << "Reported std error as " << ex.what();\
            return E_UnknownError; \
        }                           \
        catch(...)                  \
        {                           \
            conn.RollbackTransaction(); \
            BERR << "Unknown error reported." ;\
            return E_UnknownError;  \
        }                           
        
    }
}


#endif	/* CSECONNECTIONS_H */

