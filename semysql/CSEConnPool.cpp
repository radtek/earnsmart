/* 
 * File:   CSEConnPool.cpp
 * Author: santony
 * 
 * Created on August 15, 2012, 9:20 AM
 */

#include "CSEConnPool.h"
#include <unistd.h>

using namespace se::data;

std::mutex CSEConnPool::_mutex;

CSEConnPool::CSEConnPool(CSTR userId, CSTR password, CSTR db, CSTR serverHost) :
        _userId(userId), _password(password), _db(db), _serverHost(serverHost),
        _connInUse(0)
{
    
}

CSEConnPool::CSEConnPool(const CSEConnPool& orig) : 
        _userId(orig._userId), 
        _password(orig._password),
        _db(orig._db),
        _serverHost(orig._serverHost)
{
    
}

CSEConnPool::~CSEConnPool() 
{
    clear();
}

Connection* CSEConnPool::grab()
{
//    lock_guard<std::mutex> g(_mutex);
    return mysqlpp::ConnectionPool::grab();    
}

void CSEConnPool::release(const Connection* pc)
{
//    lock_guard<std::mutex> g(_mutex);
    mysqlpp::ConnectionPool::release(pc);
}

Connection* CSEConnPool::create()
{
//    lock_guard<std::mutex> l(_mutex);
    Connection* conn = new mysqlpp::Connection(
            _db.empty() ? 0 : _db.c_str(), 
            _serverHost.empty() ? 0 : _serverHost.c_str(), 
            _userId.empty() ? 0 : _userId.c_str(), 
            _password.empty() ? 0 : _password.c_str()
            );
    conn->set_option(new ReconnectOption(true));
    conn->set_option(new MultiResultsOption(true));
    return conn;
}

void CSEConnPool::destroy(Connection* cp)
{
//    lock_guard<std::mutex> g(_mutex);
    delete cp;
}

U32 CSEConnPool::max_idle_time()
{
    return 30; // seconds
}
