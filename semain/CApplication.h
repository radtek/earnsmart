/* 
 * File:   CApplication.h
 * Author: santony
 *
 * Created on November 6, 2012, 2:23 PM
 */

#ifndef CAPPLICATION_H
#define	CAPPLICATION_H

#include <mutex>
#include <unordered_map>

#include "semain.h"
#include "CSession.h"
#include "../seglobal/CTimerServices.h"

using namespace se;

typedef unordered_map<string, SP<ISession>> ActiveSessions; 

class CApplication : public CObjectRoot , public se::IAppEx
{
   
public:
    CApplication(ISession*);
    CApplication(const CApplication& orig)=delete;
    virtual ~CApplication();
    
    virtual SE_CODE Init();
    
    
    virtual SE_CODE get_Configurator(se::config::IConfigurator** ppOut) ;

    virtual RP<se::config::IConfigurator> get_Configurator();

    virtual SE_CODE CreateSession(CSTR userId, CSTR password, ISession** ppOut) ;

    virtual SE_CODE OpenSession(CSTR sessionId, ISession** ppOut) override;

    virtual SE_CODE CloseSession(ISession* pIn) ;
    
    virtual SE_CODE get_ThreadPool(se::threading::TPT pooltype, se::threading::IThreadPool** ppOut);
    
    RP<ISessionInterface> OpenSession(CSTR sessionId) override;

    ISessionInterface* GetSessionNoLock(const char* sessionId) override;

    
    RP<ISessionInterface> CreateSession(const char* userId, const char* pwd) override;
    
    RP<ISessionInterface> CreateAnonymousSession() override;


    long unsigned int CloseSession(CSTR sessionId) override;

    SE_CODE ResetSession(CSTR newUniqueId, ISession* pSession);
    
private:
    SE_CODE CacheRemoveIfSessionExpired(CSTR sessionId);
    
    void LoadTimerServices();
    
private:
    ActiveSessions _activeSessions;

public:
    static IApp* pOnlyApp;
    static std::recursive_mutex appLock;
    SP<se::config::IConfigurator> _pconfig;
    se::services::CTimerServices _timerServices;
    
    
};

#endif	/* CAPPLICATION_H */

