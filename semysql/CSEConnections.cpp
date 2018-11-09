/* 
 * File:   CSEConnections.cpp
 * Author: santony
 * 
 * Created on August 15, 2012, 1:08 PM
 */

#include "../seglobaltypedefs.h"
#include "CSEConnections.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "../seglobal/globallogger.h"
#include <jsoncpp/json/json.h>

using namespace se::data;
using namespace boost;
using namespace boost::property_tree;

using namespace Json;

CSEConnPool* CSEConnections::_pMain = 0;
CSEConnPool* CSEConnections::_pBackup = 0;
CSEConnPool* CSEConnections::_pEodData = 0;
CSEConnPool* CSEConnections::_pAdmin = 0;

bool CSEConnections::InitConfig(CSTR configfilename)
{
    
    // load "databases" section
    Reader rdr;
    Value root;
    std::ifstream fs;
    fs.open((string)configfilename, std::ifstream::in);
    if (!rdr.parse(fs, root, false))
        return false;
    try
    {
        Value& dbs = root["databases"];
        Value& dbmain = dbs["main"];
        _pMain = new CSEConnPool
                (
                dbmain["userid"].asCString(), 
                dbmain["password"].asCString(),
                dbmain["db"].asCString(),
                dbmain["host"].asCString()
                );
        if (dbs.isMember("seeoddata"))
        {
            Value& dbbkp = dbs["seeoddata"];
            _pEodData = new CSEConnPool
                    (
                    dbbkp["userid"].asCString(), 
                    dbbkp["password"].asCString(),
                    dbbkp["db"].asCString(),
                    dbbkp["host"].asCString()
                    );
        }
        if (dbs.isMember("admin"))
        {
            Value& dbbkp = dbs["admin"];
            _pAdmin = new CSEConnPool
                    (
                    dbbkp["userid"].asCString(), 
                    dbbkp["password"].asCString(),
                    dbbkp["db"].asCString(),
                    dbbkp["host"].asCString()
                    );
        }
        if (dbs.isMember("backup"))
        {
            Value& dbbkp = dbs["backup"];
            _pBackup = new CSEConnPool
                    (
                    dbbkp["userid"].asCString(), 
                    dbbkp["password"].asCString(),
                    dbbkp["db"].asCString(),
                    dbbkp["host"].asCString()
                    );
        }
        
        fs.close();
    }
    catch (std::exception& ex)
    {
        return false;
    }
    
    return true;
    
}

CSEConnPool* CSEConnections::GetMain()
{
    return _pMain;
}

void CSEConnections::Shutdown()
{
    if (_pMain)
        delete _pMain;
}

CSEConnPool* CSEConnections::GetBackup()
{
    return _pBackup;
}

CSEConnPool* CSEConnections::GetEodData()
{
    return _pEodData;
}

CSEConnPool* CSEConnections::GetAdmin()
{
    return _pAdmin;
}

////////////////////////CSmartConnection////////////////////////////

std::mutex CSmartConnection::_mutex;
ZThread::ThreadLocal<Connection*> CSmartConnection::_tlsConnections;

CSmartConnection::CSmartConnection(CSEConnPool* pool) : _pool(pool), _pTrans(0), _iTransCount(0), _defaulttimeout(30)
{
    try
    {
        /*
        _pConn = _tlsConnections.get();
        if (!_pConn)
        {
            Poco::ScopedLock<Poco::Mutex> l(_mutex);
            if (!_pConn)
            {
                Connection::thread_start();
                _pConn = pool->grab();
                _tlsConnections.set(_pConn);
            }
        }
         */
        Connection::thread_start();
        _pConn = pool->grab();
    }
    catch(const std::exception& ex)
    {
        CSTR msg = ex.what();
        pool->remove(_pConn);
        _pConn = pool->safe_grab();
    }
}

CSmartConnection::~CSmartConnection()
{
    if (_iTransCount)
    {
        RollbackTransaction();
    }
    
    //Poco::ScopedLock<Poco::Mutex> l(_mutex);
    _pool->release(_pConn);
    Connection::thread_end();
}


Connection* CSmartConnection::operator ->() 
{
    return _pConn;
}

CSmartConnection::operator Connection*() 
{
    return _pConn;
}

void CSmartConnection::LockTables() 
{

}

void CSmartConnection::UnlockTables() 
{

}



void CSmartConnection::BeginTransaction() 
{
    if (!_iTransCount)
    {
        _pTrans = new Transaction(*_pConn);
    }
    _iTransCount++;
}

void CSmartConnection::RollbackTransaction() 
{
    if (_iTransCount)
    {
        if (_iTransCount == 1)
        {
            _pTrans->rollback();
            delete _pTrans;
            _pTrans = 0;
        }
        _iTransCount--;
    }
}

void CSmartConnection::CommitTransaction() 
{
    if (_iTransCount)
    {
        if (_iTransCount == 1)
        {
            _pTrans->commit();
            delete _pTrans;
            _pTrans = 0;
        }
        _iTransCount--;
    }
}

void CSmartConnection::set_ConnectionTimeout(int timeout) 
{
    //timeout = std::max(5, timeout);
    //_pConn->set_option(new ConnectTimeoutOption(timeout));
}

void CSmartConnection::RestoreTimeout() 
{
    //_pConn->set_option(new ConnectTimeoutOption(_defaulttimeout));
}


#include "../seglobal/CDirectory.h"
#include <boost/filesystem.hpp>
#include <openssl/pem.h>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace se::config;

const char* CSmartConnection::get_Key() 
{
    if (_key.empty())
    {
        string path = "./sgweb.key";
        // open file
        ifstream ipf(path);
        const string b = "-----BEGIN PRIVATE KEY-----";
        const string e = "-----END PRIVATE KEY-----";
        stringstream str;
        str << ipf.rdbuf();
        _key = str.str();
        // strip b and e 
        replace_all(_key, b, "");
        replace_all(_key, e, "");
        replace_all(_key, "\n", "");
        boost::trim(_key);
    }
    return _key.c_str();
}

bool CSmartConnection::get_HasData(const char* tableName, const char* condition) 
{
    if (!_pConn)
    {
        BERR << "DB connection not establised.";
        return false;
    }
    
    auto q = _pConn->query();
    q << "select 1 from " << tableName << " where " << condition;
    return q.store().size() > 1;
}

string CSmartConnection::get_ScalarValue(const char* tableName, CSTR column, const char* condition) 
{
    if (!_pConn)
    {
        BERR << "DB connection not establised.";
        return false;
    }
    auto q = _pConn->query();
    q << "select " << column << " from " << tableName << " where " << condition;
    auto store = q.store();
    if (store.size())
    {
        return store[0][0].c_str();
    }
    return "";
}
