/* 
 * File:   CBrowserSession.h
 * Author: santony
 *
 * Created on July 16, 2015, 7:59 PM
 */

#ifndef CBROWSERSESSION_H
#define	CBROWSERSESSION_H


/*
 * Type defines a user's session on the browser. 
 * A browser session is defined in this application as the time
 * a user at his browser spends without an interval not exceeding
 * the browsersession value set in the "browsersession" configuration
 * key in se.json file.
 * 
 * Each time a new http request is received, the session is extended
 * further by the value defined in "browsersession" period.
 * 
 * If no value is set in se.json file, a default of 20 mins is used.
 * 
 * There  is an strong relationship between this class with cppcms application and 
 * session_interface.
 * 
 * NOTE: To generate a session id in cppcms, add the below code in
 * application::init override.
 * 
 *     // dummy session initialization 
    if (!session().is_set("browsersession"))
        session().set("browsersession", "browsersession");

 * 
 */

#include <string>
#include <unordered_map>
#include "../seglobal/IObject.h"
#include <mutex>
#include "../seglobal/CTimer.h"
#include "../seadmin/seadminapi.h"
#include <cppcms/http_context.h>
#include "CSPSerializable.h"


using namespace std;
using namespace cppcms;
using namespace se;
using namespace se::admin;

typedef unordered_map<string, OmniType> SessionStorage;

struct TimedStorage : SessionStorage
{
    time_t timeOut = ::time(0);
    void ExtendLease();
    bool HasExpired(int period);
};

typedef unordered_map<string, TimedStorage> AllSessions;

struct BSKeys
{
    static const string UserSession;
    static const string WebSession;
    static const string UserId;
};

class CBrowserSession 
{
    
private:
    int _period = 0;
    AllSessions _allSessions;
    
    recursive_mutex _lock;
    OmniType _empty = OmniType::Empty();
    string _emptyStr;
    CTimer _timer;
    
    SP<IAdminAPI> _padmin;

public:
    CBrowserSession();
    CBrowserSession(const CBrowserSession& orig) = delete;
    virtual ~CBrowserSession();

    bool get_IsInit(session_interface& session);
    
    void Initialize(http::context& cctxt);

    void Clear(http::context& cctxt);
    void Clear(const string& sessionid);
    
    UID get_UserId(session_interface& session) ;
    
    void set_UserId(http::context& ctxt, UID uid) ;
    
    
    int get_Period() ;
    
    bool get_IsSessionValid(session_interface& session);
    
    bool get_HasExpired(session_interface& session);
    
    void set_Period(session_interface& session);

    // Returns OmniType::Empty() if not found
    const OmniType& get_Value(session_interface& session, const string& key);
    
    void set_Value(session_interface& session, const string& key, const OmniType& value);
    
    const OmniType& get_Value(const string& session, const string& key);
    
    void set_Value(const string& session, const string& key, const OmniType& value);

    const OmniType& get_Value(const string& key);
    
    void set_Value(const string& key, const OmniType& value);
    
    
    bool get_IsSet(session_interface& session, const string& key);

    bool get_IsSet(const string& sid, const string& key);

    bool get_IsSet(const string& key);

    
    // Use smart pointer SP as lhs variable.
    IAdminAPI* get_Admin();
    
    // Use smart pointer SP as lhs variable.
    IWebSession* get_WebSession(session_interface& session);
    
    bool get_IsAuthorized(session_interface& session);
    
    bool get_HasSession(session_interface& session, string* sessionid = nullptr);

    // set last visited url for the given session.
    void set_LastUrl(session_interface& session, string url);
    
    // get visited url by position. -1 means the most recently visited and -2 means next most recently visited and so on.
    const string& get_Url(session_interface& session, int position = -1);
    
    static SE_CODE BuildUserSession(http::context& cctxt, IUserSession*& pUserSession) ;


    // exposing lock mechanism
    void lock() { _lock.lock(); }
    bool try_lock() { _lock.try_lock(); }
    void unlock() { _lock.unlock(); }
    
private:
    void OnTimeout();
    
    void set_WebSession(session_interface& session, IWebSession* pWs);
    
};

#endif	/* CBROWSERSESSION_H */

