/* 
 * File:   CApplication.cpp
 * Author: santony
 * 
 * Created on November 6, 2012, 2:23 PM
 */

#include <uuid/uuid.h>

#include "semaininternal.h"
#include "CApplication.h"
#include "CSession.h"

#include "CConfigurator.h"
#include "CSession.h"
#include "../seglobal/CFunctionCache.h"
#include "../seglobal/CThreadPool.h"
#include "../seglobal/CDirectory.h"
#include "../seglobal/globallogger.h"
#include "../seadmin/CPurgeUserSessions.h"

using namespace se::cache;
using namespace se::admin;

IApp* CApplication::pOnlyApp = 0;

std::recursive_mutex CApplication::appLock;

CApplication::CApplication(ISession* ps) : CObjectRoot(ps)
{

}

CApplication::~CApplication()
{
    // free function cache.
    se::cache::CFunctionCache::Free();
}

ErrorId CApplication::Init()
{
    // initialize funciton cache.
    se::cache::CFunctionCache::Init();
    this->LoadTimerServices();
    return S_Success;
}

RP<se::config::IConfigurator> CApplication::get_Configurator()
{
    if (!_pconfig)
    {
        std::lock_guard<std::recursive_mutex> g(appLock);
        if (!_pconfig)
            if (FAILED(CObject<CConfigurator>::Create(get_Session(), &_pconfig)))
                throw seexception("configurator not created.");
    }
    return _pconfig;
}

ErrorId CApplication::get_Configurator(se::config::IConfigurator** ppOut)
{
    if (!_pconfig)
    {
        std::lock_guard<std::recursive_mutex> g(appLock);
        if (!_pconfig)
            RETURNIFFAILED(CObject<CConfigurator>::Create(get_Session(), &_pconfig));
    }
    _pconfig.CopyTo(ppOut);
    return S_OK;
}

ErrorId CApplication::CreateSession(CSTR userId, CSTR password, ISession** ppOut)
{
    if (!userId || !strlen(userId) || !password || !strlen(password))
    {
        return E_SecurityAuthorization;
    }
    
    std::lock_guard<std::recursive_mutex> g(appLock);
    SP<ISession> pSession;
    auto retCode = CObject<CSession>::Create(nullptr, this, userId, password, &pSession);
    if (ISOK(retCode))
    {
        _activeSessions.insert(make_pair((string)pSession->get_UniqueId(), pSession));
        pSession.CopyTo(ppOut);
    }
    return retCode;
}

long unsigned int CApplication::CacheRemoveIfSessionExpired(const char* sessionId) 
{
    ISessionInterface* pSession = static_cast<ISessionInterface*>(get_Session());
    auto ret = pSession->get_AdminApi()->get_HasUserSessionExpired(sessionId);
    if (ret == E_SessionExpired || FAILED(ret))
    {
        std::lock_guard<std::recursive_mutex> g(appLock);
        // remove session from cache
        return CloseSession(sessionId);
    }
    return ret;
}


long unsigned int CApplication::OpenSession(CSTR sessionId, ISession** ppOut)
{
    string sSessionId = sessionId;
    std::lock_guard<std::recursive_mutex> g(appLock);
    auto find = _activeSessions.find(sessionId);
    if (find != _activeSessions.end())
    {
        find->second.CopyTo(ppOut);
        return S_OK;
    }
    
    // attempt to load an existing session from database using the same session id
    
    uuid_t uuId;
    if (sSessionId.empty() || 0 != uuid_parse(sSessionId.c_str(), uuId))
    {
        uuid_clear(uuId);
        uuid_generate_time_safe(uuId);
        char buffer[0xff];
        uuid_unparse_lower(uuId, buffer);
        sSessionId = buffer;
    }
    // chek the sessionid
    SP<ISession> pSession;
    RETURNIFFAILED(CObject<CSession>::Create(nullptr, this, sSessionId.c_str(), &pSession));
    // add it to local cache.
    _activeSessions.insert(make_pair(sSessionId, pSession));
    pSession.CopyTo(ppOut);
    return S_OK;
}

ISessionInterface* CApplication::GetSessionNoLock(const char* sessionId) 
{
    auto find = _activeSessions.find(sessionId);
    if (find != _activeSessions.end())
    {
        return static_cast<ISessionInterface*>(find->second._ptr);
    }
    return nullptr;
}


ErrorId CApplication::CloseSession(ISession* pIn)
{

    if (pIn)
    {
        std::lock_guard<std::recursive_mutex> g(appLock);
        auto find = _activeSessions.find((string)pIn->get_UniqueId());
        if (find != _activeSessions.end())
        {
            _activeSessions.erase(find);
            return S_OK;
        }
    }
    return S_False;
}

RP<ISessionInterface> CApplication::OpenSession(CSTR sessionId)
{
    if (!sessionId)
    {
        seexception ex(E_SessionUnknown);
        ex << "Failed to open session with error: " << E_SessionUnknown << ": " << get_CodeMessage(E_SessionUnknown);
        throw ex;
    }

    std::lock_guard<std::recursive_mutex> g(appLock);

    SP<ISession> ps;
    auto ret = OpenSession(sessionId, &ps);
    if (FAILED(ret))
    {
        seexception ex(ret);
        ex << "Failed to open session with error: " << ret << ": " << get_CodeMessage(ret);
        throw ex;
    }
    RP<ISessionInterface> pp = static_cast<ISessionInterface*>(ps._ptr);
    return pp;
}

RP<ISessionInterface> CApplication::CreateSession(const char* userId, const char* pwd)
{
    SP<ISession> ps;
    std::lock_guard<std::recursive_mutex> g(appLock);
    auto ret = CreateSession(userId, pwd, &ps);
    if (FAILED(ret))
    {
        seexception ex(ret);
        ex << "Failed to create session with error: " << ret << ": " << get_CodeMessage(ret);
        throw ex;
    }
    RP<ISessionInterface> pp = static_cast<ISessionInterface*>(ps._ptr);
    return pp;
}

RP<ISessionInterface> CApplication::CreateAnonymousSession()
{
    // create a new session in the database and return newly created session id.
    std::lock_guard<std::recursive_mutex> g(appLock);
    SP<ISession> ps;
    auto ret = CObject<CSession>::Create(nullptr, this, &ps);
    if (FAILED(ret))
    {
        seexception ex(ret);
        ex << "Failed to create session with error: " << ret << ": " << get_CodeMessage(ret);
        throw ex;
    }
    RP<ISessionInterface> pp = static_cast<ISessionInterface*>(ps._ptr);
    return pp;
}

SE_CODE CApplication::CloseSession(CSTR sessionId)
{
    std::lock_guard<std::recursive_mutex> g(appLock);
    auto find = _activeSessions.find(sessionId);
    if (find != _activeSessions.end())
    {
        SP<ISession> ps;
        ps = find->second;
        return CloseSession(ps);
    }
    return S_False;
}

SE_CODE CApplication::ResetSession(const char* newUniqueId, ISession* pSession)
{
    if (pSession)
    {
        std::lock_guard<std::recursive_mutex> g(appLock);
        const string oldSessionId = (string)pSession->get_UniqueId();
        BDBG << "OldSessionId: " << oldSessionId;
        auto find = _activeSessions.find(oldSessionId);
        if (find != _activeSessions.end())
        {
            SP<ISession> ps;
            ps = find->second;
            ((CSession*)ps._ptr)->_uniqueId = newUniqueId;
            // remove session entry
            _activeSessions.erase(find);
            // now open session method below will attempt to add this session to the cache.
            return S_OK;
        }
    }
    return OpenSession(newUniqueId, &pSession);
}

void CApplication::LoadTimerServices()
{
    // Auto purge old sessions
    CPurgeUserSessions* purger = new CPurgeUserSessions(this);
    _timerServices.Add(purger);
    _timerServices.Init();
}

// global 
SP<se::threading::CPThreadPool> _gpthreadpool;
SP<se::threading::CSThreadPool> _gsthreadpool;
SP<se::threading::CCThreadPool> _gcthreadpool;

ErrorId CApplication::get_ThreadPool(se::threading::TPT pooltype, se::threading::IThreadPool** ppOut)
{
    if (pooltype == se::threading::TPT_ParallelPool)
    {
        if (!_gpthreadpool)
        {
            std::lock_guard<std::recursive_mutex> g(appLock);
            if (!_gpthreadpool)
            {
                RETURNIFFAILED(CObject<se::threading::CPThreadPool>::Create(get_Session(), &_gpthreadpool));
            }
        }
        _gpthreadpool.CopyTo(ppOut);
    }
    else if (pooltype == se::threading::TPT_ConcurrentExecution)
    {
        if (!_gcthreadpool)
        {
            std::lock_guard<std::recursive_mutex> g(appLock);
            if (!_gcthreadpool)
            {
                RETURNIFFAILED(CObject<se::threading::CCThreadPool>::Create(get_Session(), &_gcthreadpool));
            }
        }
        _gcthreadpool.CopyTo(ppOut);
    }
    else
    {
        if (!_gsthreadpool)
        {
            std::lock_guard<std::recursive_mutex> g(appLock);
            if (!_gsthreadpool)
            {
                RETURNIFFAILED(CObject<se::threading::CSThreadPool>::Create(get_Session(), &_gsthreadpool));
            }
        }
        _gsthreadpool.CopyTo(ppOut);
    }
    return S_Success;
}

