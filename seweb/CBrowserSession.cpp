/* 
 * File:   CBrowserSession.cpp
 * Author: santony
 * 
 * Created on July 16, 2015, 7:59 PM
 */

#include "CAppBase.h"
#include <cppcms/session_interface.h>
#include <cppcms/json.h>
#include <langinfo.h>
#include "CBrowserSession.h"
#include "../sewebmodels/CSessionManageMethods.h"
#include "../seglobal/CList.h"
#include "../semain/semain.h"

const string BSKeys::WebSession = "WS";
const string BSKeys::UserSession = "US";
const string BSKeys::UserId = "UID";

const string GLOBALSESSION = "2298b9ee-622e-4895-99bf-18d1262d0cfd";

CBrowserSession::CBrowserSession() : _timer(1, 60) 
{
    _timer.SetState(this);
    _timer.SetCallback([](CTimer const* timer, LPVOID state)
    {
        static_cast<CBrowserSession*>(state)->OnTimeout();
    });
    _timer.StartTimer();
}

CBrowserSession::~CBrowserSession() 
{
    _timer.StopTimer();
    OnTimeout();
}

bool CBrowserSession::get_IsInit(session_interface& session) 
{
    return get_WebSession(session) != nullptr;
}


void CBrowserSession::Initialize(http::context& ctxt) 
{
    session_interface& session = ctxt.session();
    set_Period(session);
    const string& sid = session.get_session_cookie();
    if (sid.empty())
        return ;
    
    SP<IWebSession> pWs;
    SP<IAdminAPI> pAdm = get_Admin();
    auto rcode = pAdm->get_WebSession(sid.c_str(), &pWs);
    if (rcode == E_NotFound)
    {
        // double check user id.
        SP<IUser> pUser;
        UID uid = get_UserId(ctxt.session());
        if (FAILED(get_Admin()->get_User(uid, &pUser)))
        {
            set_UserId(ctxt, 0);
        }
        // initialize and return
        SP<IUserSession> pSession;
        IUserSession::Create(&pSession);
        BuildUserSession(ctxt, pSession._ptr);
        // now register
        pSession->set_UserId(get_UserId(ctxt.session()));
        pSession->set_Activity(UST::UST_SessionInit);
        pAdm->RegisterSessionActivity(pSession._ptr, &pWs);
    }
    set_WebSession(session, pWs);
}

unsigned long CBrowserSession::get_UserId(session_interface& session) 
{
    return strtoll(session.get(BSKeys::UserId, "0").c_str(), nullptr, 10);
}

void CBrowserSession::set_UserId(http::context& ctxt, unsigned long uid) 
{
    ostringstream ostr;
    ostr << uid;
    ctxt.session().set(BSKeys::UserId, ostr.str());
    SP<IWebSession> pws;
    SP<IUserSession> pSession;
    IUserSession::Create(&pSession);
    BuildUserSession(ctxt, pSession._ptr);
    // now register
    pSession->set_UserId(get_UserId(ctxt.session()));
    pSession->set_Activity(UST::UST_SignIn);
    get_Admin()->RegisterSessionActivity(pSession._ptr, &pws);
    set_WebSession(ctxt.session(), pws);
}


void CBrowserSession::Clear(http::context& cctxt) 
{
    // initialize and return
    SP<IUserSession> pSession;
    IUserSession::Create(&pSession);
    BuildUserSession(cctxt, pSession._ptr);
    // now register
    pSession->set_UserId(get_UserId(cctxt.session()));
    pSession->set_Activity(UST::UST_SessionExpired);
    get_Admin()->RegisterSessionActivity(pSession._ptr, nullptr);
    const string& sid = cctxt.session().get_session_cookie();
    Clear(sid);
}

void CBrowserSession::Clear(const string& sessionid)
{
    auto it = _allSessions.find(sessionid);
    if (it != _allSessions.end())
    {
        lock_guard<recursive_mutex> g(_lock);
        _allSessions.erase(it);
    }
}


void CBrowserSession::OnTimeout() 
{
    auto it = _allSessions.begin();
    while (it != _allSessions.end())
    {
        TimedStorage& store = it->second;
        if (store.HasExpired(get_Period()))
        {
            Clear(it->first);
        }
        if (it != _allSessions.end())
            break;
        it++;
    }    
}

bool TimedStorage::HasExpired(int period) 
{
    double d = difftime(::time(0), timeOut);
    return d >= (double)period * 60;
}

void TimedStorage::ExtendLease() 
{
    timeOut = ::time(0);
}

int CBrowserSession::get_Period() 
{
    return _period;
}

bool CBrowserSession::get_IsSessionValid(session_interface& session) 
{
    const string& sid = session.get_session_cookie();
    return !sid.empty();
}


bool CBrowserSession::get_HasExpired(session_interface& session) 
{
    const string& sid = session.get_session_cookie();
    if (sid.empty())
        return true;

    AllSessions::iterator it = _allSessions.find(sid);
    if (it == _allSessions.end())
    {
        return true;
    }
    return it->second.HasExpired(get_Period());
}


void CBrowserSession::set_Period(session_interface& session) 
{
    if (!_period)
    {
        _period = session.browsersession();
//        SP<IApp> papi;
//        SEOpenApp(&papi);
//        const Json::Value& cfg = papi->get_Configurator()->get_ConfigRoot();
//        /*
//             "session" : 
//        {
//            "browsersession" : 3600,
//         */
//        _period = 20;
//        Json::Value v;
//        if (cfg.isMember("session"))
//        {
//            v = cfg["session"];
//            if (v.isMember("browsersession"))
//            {
//                v = cfg["browsersession"];
//                _period = cfg["session"]["browsersession"].asInt();
//            }
//        }
    }
}


const OmniType& CBrowserSession::get_Value(session_interface& session, const string& key) 
{
    const string& sid = session.get_session_cookie();
    return get_Value(sid, key);
}

bool CBrowserSession::get_IsSet(session_interface& session, const string& key) 
{
    const string& sid = session.get_session_cookie();
    return get_IsSet(sid, key);
}

bool CBrowserSession::get_IsSet(const string& key) 
{
    return get_IsSet(GLOBALSESSION, key);
}

bool CBrowserSession::get_IsSet(const string& sid, const string& key) 
{
    if (sid.empty())
        return false;
    AllSessions::iterator it = _allSessions.find(sid);
    if (it==_allSessions.end())
        return false;
    TimedStorage& store = it->second;
    store.ExtendLease();
    SessionStorage::const_iterator it2 = store.find(key);
    return (it2 != store.end());
}



void CBrowserSession::set_Value(session_interface& session, const string& key, const OmniType& value) 
{
    const string& sid = session.get_session_cookie();
    set_Value(sid, key, value);
}

const OmniType& CBrowserSession::get_Value(const string& sid, const string& key) 
{
    if (sid.empty())
        return _empty;
    AllSessions::iterator it = _allSessions.find(sid);
    if (it==_allSessions.end())
        return _empty;
    TimedStorage& store = it->second;
    store.ExtendLease();
    SessionStorage::const_iterator it2 = store.find(key);
    
    if (it2 != store.end())
        return it2->second;
    return _empty;

}

void CBrowserSession::set_Value(const string& sid, const string& key, const OmniType& value)
{
    if (sid.empty())
        return ;
    AllSessions::iterator it = _allSessions.find(sid);
    if (it == _allSessions.end())
    {
        lock_guard<recursive_mutex> g(_lock);
        {
            it = _allSessions.find(sid);
            if (it == _allSessions.end())
            {
                it = _allSessions.insert(make_pair(sid, TimedStorage())).first;
                
            }
        }
    }
    TimedStorage& store = it->second;
    SessionStorage::iterator it2 = store.find(key);
    if (it2 == store.end())
    {
        lock_guard<recursive_mutex> g(_lock);
        {
            if (it2 == store.end())
            {
                it2 = store.insert(make_pair(key, value)).first;
            }
        }        
    }
    else
    {
        store[key] = value;
    }
    
}

const OmniType& CBrowserSession::get_Value(const string& key) 
{
    return get_Value(GLOBALSESSION, key);
}

void CBrowserSession::set_Value(const string& key, const OmniType& value) 
{
    set_Value(GLOBALSESSION, key, value);
}


IAdminAPI* CBrowserSession::get_Admin() {
    if (!_padmin)
        THROWIFFAILED(AdminOpen(&_padmin), "Admin gateway failed.");
    return _padmin._ptr;
}

void CBrowserSession::set_WebSession(session_interface& session, IWebSession* pWs) 
{
    OmniType ot = pWs;
    set_Value(session, BSKeys::WebSession, ot);
}

bool CBrowserSession::get_IsAuthorized(session_interface& session) {
    auto ws = get_WebSession(session);
    if (!ws)
        return false;
    if (ws->get_UserId())
        return true;
    return false;
}


IWebSession* CBrowserSession::get_WebSession(session_interface& session) 
{
    const OmniType& ot = get_Value(session, BSKeys::WebSession);
    if (ot.IsEmpty())
        return nullptr;
    return (IWebSession*)(IObject*) ot;
}

long unsigned int CBrowserSession::BuildUserSession(http::context& cctxt, IUserSession*& pUserSession) 
{
    return CSessionManageMethods::BuildUserSession(cctxt, pUserSession);
}

bool CBrowserSession::get_HasSession(session_interface& session, string* sessionid) 
{
    auto sid = session.get_session_cookie();
    if (!sid.empty())
        if (sessionid)
        {
            *sessionid = sid;
            return true;
        }
    return false;
}

void CBrowserSession::set_LastUrl(session_interface& session, string url) 
{
    auto sid = session.get_session_cookie();
    if (sid.empty())
        return;

    const OmniType& list = get_Value(session, "UrlHistory");
    if (list.IsEmpty())
    {
        lock_guard<recursive_mutex> g(_lock);
        {
            SP<IList<string>> pList;
            const auto& list2 = get_Value(session, "UrlHistory");
            if (list2.IsEmpty())
            {
                CList<string>::CreateList(&pList);
                set_Value(session, "UrlHistory", (IObject*)pList);
                pList->Add(url);
            }
        }
    }
    else
    {
        IList<string>* plist = (IList<string>*)(IObject*)list;
        if (plist->get_Count()>25)
        {
            lock_guard<recursive_mutex> g(_lock);
            {
                if (plist->get_Count()>25)
                {
                    plist->RemoveItem(0);
                }
            }
        }
        plist->Add(url);
    }
}

const string& CBrowserSession::get_Url(session_interface& session, int position) 
{
    auto sid = session.get_session_cookie();
    if (sid.empty())
        return _emptyStr;

    const OmniType& list = get_Value(session, "UrlHistory");
    if (list.IsEmpty())
        return _emptyStr;
    IList<string>* urllist = (IList<string>*)(IObject*)list;
    auto size = urllist->get_Count();
    if (!size or position >= size)
        return _emptyStr;
    if (position < 0)
    {
        auto loc = size - position;
        if (loc < 0)
            return _emptyStr;
        string *url ;
        urllist->get_Item(loc, url);
        return *url;        
    }   
    else
    {
        string* url;
        urllist->get_Item(position, url);
        return *url;        
    }
}
