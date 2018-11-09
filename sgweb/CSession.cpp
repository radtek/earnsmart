/* 
 * File:   CSession.cpp
 * Author: santony
 * 
 * Created on August 31, 2013, 10:31 AM
 */

#include <signal.h>
#include <cppcms/session_interface.h>
#include <cppcms/http_context.h>
#include <cppcms/http_request.h>

#include "CSession.h"
#include "../seglobal/exceptions.h"



namespace sg
{
    namespace web
    {

        CSession::CSession() : _userId(0)
        {
            
        }

        CSession::CSession(const CSession& orig) 
        {
            _userId = orig._userId;
            _sessionId = orig._sessionId;
        }

        CSession::~CSession() 
        {
        }
        
        void CSession::Init(const string& sessionid)
        {
            _sessionId = sessionid;
        }
        
        void CSession::Clear()
        {
            _sessionId.clear();
            _userId = 0;        // anonymous
        }
        
        bool CSession::get_IsAuthenticated()
        {
            return (_userId > 0);
        }

        void CSession::BuildUserSession(http::context& cctxt, IUserSession*& pUserSession) 
        {
            http::context& ctxt = cctxt;
            http::request& req = cctxt.request();
            pUserSession->set_SessionId(ctxt.session().get_session_cookie().c_str());
            pUserSession->set_UserAgent(req.http_user_agent().c_str());
            pUserSession->set_RemoteAddress(req.remote_addr().c_str());
            pUserSession->set_RemoteHost(req.remote_host().c_str());
            pUserSession->set_RemoteUser(req.remote_user().c_str());
            pUserSession->set_RemoteIdent(req.remote_ident().c_str());
            pUserSession->set_HttpReferer(req.http_referer().c_str());
        }

        void CSession::set_UserSession(IUserSession* p) 
        {
            _pUserSession = p;
            _userId = _pUserSession->get_UserId();
        }

        
        bool CSession::Authenticate(string const& userId, string const& password)
        {
            SP<IAdminAPI> papi;
            THROWIFFAILED(AdminOpen(&papi), "Failed to create admin instance.");
            
            if (!_pUserSession)
                IUserSession::Create(&_pUserSession);
            
            SE_CODE c = papi->AuthenticateUser(userId.c_str(), password.c_str(), _pUserSession._ptr);
            if (c == E_SecurityAuthorization)
            {
                _userId = 0;
                return false;
            }
            else if (FAILED(c))
                throw seexception(c, "Internal API exception.");
            _userId = _pUserSession->get_UserId();
            return true;
        }

        void CSession::serialize(archive& a)
        {
            a & _sessionId ;
        }
    }
}